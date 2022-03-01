#include "RSA.h"
#include <boost/integer/mod_inverse.hpp>
#include <algorithm>
#include <stack>
using namespace boost::multiprecision;

cpp_int RSA::generate_prime_number(size_t bitsLen) const
{
	mt11213b base_gen(clock());
	independent_bits_engine<mt11213b, 1024, cpp_int> gen1024(base_gen);
	mt19937 gen2(clock());

	cpp_int n = gen1024();
	while (!miller_rabin_test(n, 64, gen2))
	{
		n = gen1024();
	}

	return n;
}

void RSA::generateKeys()
{
	P(generate_prime_number(1024));
	Q(generate_prime_number(1024));
	calculate_n();
	calculate_phi();
	calculate_e();
	calculate_d();
}

cpp_int RSA::powm(cpp_int base, cpp_int exp, const cpp_int& modulus)
{
	base %= modulus;
	cpp_int result = 1;
	while (exp > 0)
	{
		if (exp & 1) // if exp is even
			result = (result * base) % modulus;
		base = (base * base) % modulus;
		exp >>= 1; // /2
	}
	return result;
}

cpp_int RSA::extendedGcd(cpp_int a, cpp_int b)
{
	cpp_int x = 0, lastX = 1, y = 1, lastY = 0, temp, quotient;
	while (b != 0)
	{
		temp = b;
		quotient = a / b;
		b = a % b;
		a = temp;
		temp = x;
		x = lastX - quotient * x;
		lastX = temp;
		temp = y;
		y = lastY - quotient * y;
		lastY = temp;
	}
	return lastY;
}

RSA::RSA()
{
	generateKeys();
}


void RSA::calculate_n()
{
	setN(P() * q);
}

void RSA::calculate_phi()
{
	phi_n = (P() - 1) * (Q() - 1);
}

void RSA::calculate_e()
{
	unsigned temp = 0x10001;
	while (gcd(temp, phi_n) != 1)
	{
		++temp;
	}
	setE(temp);

	//assert(e >= phi_n);
}

void RSA::calculate_d()
{
	setD(RSA::extendedGcd(phi_n, getE()));
	if (getD() < 0)
		setD(getD() + phi_n);
}

std::string RSA::encrypt(const std::string& msg) const
{
	std::string result;
	std::stack<std::string> stack;

	for (size_t i = 0; i < std::ceil(msg.size() * 1.0 / blockSize); ++i)
	{
		cpp_int block{ 0 };

		for (int j = 0; j < blockSize; ++j) {
			if ((j + i * blockSize) < msg.length()) {
				block += msg[j + i * blockSize];
			}

			if (j != blockSize - 1)
				block <<= 8;
		}

		cpp_int value = RSA::powm(block, getE(), getN());

		std::string temp = boost::lexical_cast<std::string>(value);
		stack.push(temp);
	}

	while (!stack.empty()) {
		result += stack.top() + ' ';
		stack.pop();
	}

	return result;
}

std::string RSA::decrypt(const std::string& msg) const
{
	std::stringstream temp_msg(msg);
	cpp_int m;
	std::string result;

	cpp_int dp = getD() % (P() - 1);
	cpp_int dq = getD() % (Q() - 1);

	while (temp_msg >> m)
	{
		cpp_int m_1 = RSA::powm(m, dp, P());
		cpp_int m_2 = RSA::powm(m, dq, Q());
		cpp_int q_inv = boost::integer::mod_inverse(Q(), P());
		cpp_int h = q_inv * (m_1 - m_2);
		cpp_int mResult = m_2 + h * Q();

		std::string temp;

		std::cout << mResult << '\n';

		for (int i = 0; i < blockSize; ++i) {
			cpp_int x = mResult & 0xff;
			temp += x.convert_to<char>();

			mResult >>= 8;
		}

		result += temp;
	}

	result.erase(std::remove(result.begin(), result.end(), '\0'), result.end());
	std::reverse(result.begin(), result.end());

	return result;
}

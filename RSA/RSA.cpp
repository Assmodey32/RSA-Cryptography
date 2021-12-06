#include "RSA.h"
#include <boost/integer/mod_inverse.hpp>
using namespace boost::multiprecision;

cpp_int RSA::generate_prime_number(size_t bitsLen) const
{
	mt11213b base_gen(clock());
	independent_bits_engine<mt11213b, 1024, cpp_int> gen1024(base_gen);

	//
	// We must use a different generator for the tests and number generation, otherwise
	// we get false positives.
	//
	mt19937 gen2(clock());

	cpp_int n = gen1024();
	while (!miller_rabin_test(n, 64, gen2))
	{
		n = gen1024();
	}

	return n;
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
		exp >>= 1; // *2
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
	p = generate_prime_number(512);
	q = generate_prime_number(512);
	calculate_n();
	calculate_phi();
	calculate_e();
	calculate_d();
}

unsigned RSA::get_e() const
{
	return e;
}

cpp_int RSA::get_d() const
{
	return d;
}

void RSA::calculate_n()
{
	n = p * q;
}

void RSA::calculate_phi()
{
	phi_n = (p - 1) * (q - 1);
}

void RSA::calculate_e()
{
	unsigned temp = 0x10001;
	while (gcd(temp, phi_n) != 1)
	{
		++temp;
	}
	e = temp;

	//assert(e >= phi_n);
}

void RSA::calculate_d()
{
	//cpp_int temp = 1;

	//while ((temp * phi_n + 1) % e != 0)
	//	++temp;
	//d = (temp * phi_n + 1) / e;
	//std::clog << "Temp = " << temp << '\n';

	d = RSA::extendedGcd(phi_n, e);
	if (d < 0)
		d += phi_n;
	//std::cout << "d == d2: " << std::boolalpha << (d == d2) << "\n";

}

std::string RSA::encrypt(const std::string& msg) const
{
	//c = (pow(m, e) % n);

	std::string result;

	for (auto m : msg)
	{
		cpp_int value = RSA::powm(cpp_int(m), e, n);

		result += boost::lexical_cast<std::string>(value) + ' ';
	}
	return result;
}

std::string RSA::decrypt(const std::string& msg)
{
	std::stringstream temp_msg(msg);
	cpp_int m;
	std::string result;

	cpp_int dp = d % (p - 1);
	cpp_int dq = d % (q - 1);

	while (temp_msg >> m)
	{
		cpp_int m_1 = RSA::powm(m, dp, p);
		cpp_int m_2 = RSA::powm(m, dq, q);
		cpp_int q_inv = boost::integer::mod_inverse(q, p);
		cpp_int h = q_inv * (m_1 - m_2);
		cpp_int mResult = m_2 + h * q;

		char ch = mResult.convert_to<char>();

		//result += boost::lexical_cast<std::string>(ch);
		result += ch;
	}

	return result;
}

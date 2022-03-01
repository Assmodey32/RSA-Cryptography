#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <iostream>
#include <iomanip>
#include <chrono>

using boost::multiprecision::cpp_int;

using namespace boost::random;
using namespace boost::multiprecision;

class RSA
{
private:
	cpp_int p; // prime number
	cpp_int q; // prime number
	cpp_int n; // n = p * q
	cpp_int phi_n; // phi_n = (p-1)(q-1)
	unsigned e; // 2^16+1 <= e < phi_n; e and phi_n coprime
	cpp_int d; // (e * d) mod phi_n = 1

	size_t blockSize{128};

	void calculate_n();
	void calculate_phi();
	void calculate_e();
	void calculate_d();

	
public:
	RSA();

	void generateKeys();

	cpp_int generate_prime_number(size_t bitsLen) const;
	static cpp_int powm(cpp_int base, cpp_int exp, const cpp_int& modulus);
	static cpp_int extendedGcd(cpp_int a, cpp_int b);

	std::string encrypt(const std::string& msg) const;
	std::string decrypt(const std::string& msg) const;

	cpp_int getD() const { return d; }
	cpp_int getN() const { return n; }
	unsigned getE() const { return e; }
	cpp_int P() const { return p; }
	cpp_int Q() const { return q; }



	void setE(unsigned val) { e = val; }
	void setD(cpp_int val) { d = val; }
	void setN(cpp_int val) { n = val; }
	void P(cpp_int val) { p = val; }
	void Q(cpp_int val) { q = val; }
};



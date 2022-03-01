#include <iostream>
#include <iomanip>
#include <chrono> // for std::chrono functions
#include "RSA.h"
#include "base64.h"

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_type = std::chrono::steady_clock;
	using second_type = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_type> m_beg;

public:
	Timer() : m_beg{ clock_type::now() }
	{
	}

	void reset()
	{
		m_beg = clock_type::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
	}
};

int main()
{
	namespace mp = boost::multiprecision;
	RSA test;
	std::string originalMsg = "Another test example with some shit 166450172!@#$%^&*()_+tvriuoweobntuwtynoeoryuvbtoywerbyovtyonweroyvnyuitwyouieyubroynbtvoyuweobyorubbyuoynvtyuoweyuryutvyuoyuoweryuyutvyunywuyeyuryuinvtyuoweruyyuvtoonweyuryuvotyuorweyuyuontnewvtyuoweryuvotywueuo";

	std::string encryptedMsg = test.encrypt(originalMsg);

	std::cout << "Encrypted message: " << base64_encode(encryptedMsg) << "\n\n";
	std::cout << "Encrypted message: " << base64_decode(base64_encode(encryptedMsg)) << "\n\n";
	//std::cout << "Encrypted message: " << encryptedMsg << "\n\n";

	std::string restoredMsg = test.decrypt(encryptedMsg);
	std::cout << "Restored message:\t" << restoredMsg << "\n\n";
	std::cout << "Source message:\t\t" << originalMsg << "\n\n";
	std::cout << "Source message size: " << originalMsg.size() << '\n';
	std::cout << "Restored message size: " << restoredMsg.size() << '\n';
	std::cout << "Source == restored: " << std::boolalpha << (originalMsg == restoredMsg) << '\n';

	Timer t;
	std::cout << "3^1020400 % 777 using powm: " << RSA::powm(3, 1020400, 1777) << '\n';
	std::cout << "Time taken: " << t.elapsed() << '\n';
	t.reset();
	std::cout << "3^1020400 % 777: " << mp::pow(mp::cpp_int(3), 1020400) % mp::cpp_int(1777) << '\n';
	std::cout << "Time taken: " << t.elapsed() << '\n';


	return 0;
}

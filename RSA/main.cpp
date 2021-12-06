#include <iostream>
#include <iomanip>
#include <chrono> // for std::chrono functions
#include "RSA.h"

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

	RSA test;
	std::string originalMsg = "Some test message to encrypt.";

	std::string encryptedMsg = test.encrypt(originalMsg);

	std::cout << "Encrypted message: " << encryptedMsg << "\n\n";

	std::string restoredMsg = test.decrypt(encryptedMsg);
	std::cout << "Restored message: " << restoredMsg << "\n\n";


	//std::cout << test.get_d() << '\n';

	return 0;
}

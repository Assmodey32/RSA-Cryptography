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

	RSA test;
	std::string originalMsg = "Another test example with some shit 166450172!@#$%^&*()_+";

	std::string encryptedMsg = test.encrypt(originalMsg);

	//std::cout << "Encrypted message: " << base64_encode(encryptedMsg) << "\n\n";
	std::cout << "Encrypted message: " << encryptedMsg << "\n\n";

	std::string restoredMsg = test.decrypt(encryptedMsg);
	std::cout << "Restored message:\t" << restoredMsg << "\n";
	std::cout << "Source message:\t\t" << originalMsg << '\n';
	std::cout << "Source message size: " << originalMsg.size() << '\n';
	std::cout << "Restored message size: " << restoredMsg.size() << '\n';
	std::cout << "Source == restored: " << std::boolalpha << (originalMsg == restoredMsg) << '\n';


	return 0;
}

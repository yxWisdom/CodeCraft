#include <iostream>
#include <random>
#include <map>

int main(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::weibull_distribution<> d(1, 5);

	std::map<int, int> hist;

	for (int n = 0; n < 10000; ++n)
	{
		++hist[std::round(d(gen))];
	}
}
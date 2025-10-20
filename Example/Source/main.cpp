#include <BinEncoding.h>

#include <array>
#include <iostream>

int main()
{
	std::array data = { uint8_t{0xDE}, uint8_t{0xAD}, uint8_t{0xBE}, uint8_t{0xEF} };

	std::string encoded = ikea400::Bin::Hex::Encode(data);
	std::cout << encoded << std::endl;

	std::string formated = ikea400::Bin::Hex::Format(data);
	std::cout << formated << std::endl;

	auto decoded = ikea400::Bin::Hex::Decode<true>(encoded);
	std::string recoded = ikea400::Bin::Hex::Encode(decoded);

	std::cout << recoded << std::endl;


	return 0;
}
#include <BinEncoding.h>

#include <array>
#include <iostream>

int main()
{
	std::array data = { uint8_t{0xDE}, uint8_t{0xAD}, uint8_t{0xBE}, uint8_t{0xEF} };

	std::cout << ikea400::Bin::FormatHex(data) << std::endl;
	return 0;
}
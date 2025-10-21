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

	std::string base64 = ikea400::Bin::Base64::Encode(data);
	std::cout << base64 << std::endl;

	std::string formatB64 = ikea400::Bin::Base64::Format(data);
	std::cout << formatB64 << std::endl;

	std::vector<uint8_t> decodedB64 = ikea400::Bin::Base64::Decode<true>(base64);
	std::string recodedB64 = ikea400::Bin::Base64::Encode(decodedB64);
	std::cout << recodedB64 << std::endl;


	return 0;
}
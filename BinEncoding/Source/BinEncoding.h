#pragma once
#include "hex.h"

#include <string>
#include <span>
#include <algorithm>
#include <string_view>

namespace ikea400::Bin {

	inline constexpr const std::string_view kHexFormat = "hex:";

	std::string prepareFormat(const std::string_view format, std::size_t size) {
		std::string value(format.size() + size, '\0');
		std::copy(format.begin(), format.end(), value.begin());
		return value;
	}

	template <typename T>
	std::string EncodeHex(const T& value)
	{
		using value_type = T::value_type;
		std::string output(value.size() * sizeof(value_type) * 2, '\0');
#if defined(__AVX2__)
		encodeHexVec((uint8_t*)output.data(), data, size);
#else
		encodeHex((uint8_t*)output.data(), value.data(), value.size() * sizeof(value_type));
#endif
		return output;
	}
	;
	template <typename T>
	std::string FormatHex(const T& value)
	{
		using value_type = T::value_type;
		std::string output = prepareFormat(kHexFormat, value.size() * sizeof(T::value_type) * 2);
		
#if defined(__AVX2__)
		encodeHexVec((uint8_t*)output.data(), data, size);
#else
		encodeHex((uint8_t*)output.data() + kHexFormat.size(), value.data(), value.size() * sizeof(value_type));
#endif
		return output;
	}
}
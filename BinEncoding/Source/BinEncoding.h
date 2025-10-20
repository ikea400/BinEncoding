#pragma once
#include "hex.h"

#include <string>
#include <span>
#include <algorithm>
#include <string_view>
#include <vector>

namespace ikea400::Bin {

	namespace Detail
	{
		inline constexpr const std::string_view kHexFormat = "hex:";

		inline std::string PrepareFormat(const std::string_view format, std::size_t size) {
			std::string value(format.size() + size, '\0');
			std::copy(format.begin(), format.end(), value.begin());
			return value;
		}

		template <typename T>
		inline void EncodeHex(std::string_view dest, const T& value)
		{
#if defined(__AVX2__)
			encodeHexVec((uint8_t*)dest.data(), (const uint8_t*)value.data(), value.size() * sizeof(T::value_type));
#else
			encodeHex((uint8_t*)dest.data(), value.data(), value.size() * sizeof(T::value_type));
#endif
		}


		inline void DecodeHex(std::vector<uint8_t>& dest, const std::string_view value)
		{
#if defined(__AVX2__)
			decodeHexVec((uint8_t*)dest.data(), (const uint8_t*)value.data(), value.size());
#else
			decodeHexLUT4((uint8_t*)dest.data(), (const uint8_t*)value.data(), value.size());
#endif
		}
	} // namespace Detail

	namespace Hex
	{
		template <typename T>
		inline std::string Encode(const T& value)
		{
			using value_type = T::value_type;
			std::string output(value.size() * sizeof(value_type) * 2, '\0');
			if (!value.empty())
				Detail::EncodeHex(output, value);
			return output;
		}

		template <typename T>
		inline std::string Format(const T& value)
		{
			using value_type = T::value_type;
			std::string output = Detail::PrepareFormat(Detail::kHexFormat, value.size() * sizeof(T::value_type) * 2);
			if (!value.empty()) 
				Detail::EncodeHex(std::string_view(output).substr(Detail::kHexFormat.size()), value);
			return output;
		}

		template <bool Throw = true>
		inline std::vector<uint8_t> DecodeFormat(std::string_view value)
		{
			if (!value.starts_with(Detail::kHexFormat)) {
				if constexpr (Throw)
					throw std::exception("Hex decode format: invalid format prefix");
				
				return {};
			}
			return Decode<Throw>(value.substr(Detail::kHexFormat.size()));
		}

		template <bool Throw = true>
		inline std::vector<uint8_t> Decode(std::string_view value)
		{
			if (value.empty()) return {};
			if (value.size() % 2 != 0) {
				if constexpr (Throw)
					throw std::exception("Hex decode: input size is not even");
				
				return {};
			}

			std::vector<uint8_t> output(value.size() / 2);

			Detail::DecodeHex(output, value);

			return output;
		}

	} // namespace Hex


} // namespace ikea400::Bin
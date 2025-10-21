#pragma once
#include "hex.h"
#include "base64.hpp"

#include <string>
#include <span>
#include <algorithm>
#include <string_view>
#include <vector>

namespace ikea400::Bin {

	namespace Detail
	{
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
		inline constexpr const std::string_view kHexFormat = "hex:";

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
			std::string output = Detail::PrepareFormat(kHexFormat, value.size() * sizeof(T::value_type) * 2);
			if (!value.empty()) 
				Detail::EncodeHex(std::string_view(output).substr(kHexFormat.size()), value);
			return output;
		}

		template <bool Throw = true>
		inline std::vector<uint8_t> DecodeFormat(std::string_view value)
		{
			if (!value.starts_with(kHexFormat)) {
				if constexpr (Throw)
					throw std::exception("Hex decode format: invalid format prefix");
				
				return {};
			}
			return Decode<Throw>(value.substr(kHexFormat.size()));
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

	namespace Base64
	{
		inline constexpr const std::string_view kBase64Format = "b64:";

		template <typename T>
		inline std::string Encode(const T& value)
		{
			return base64::encode_into<std::string>(value.begin(), value.end());
		}

		template <typename T>
		inline std::string Format(const T& value)
		{
			return std::string(kBase64Format) + Encode(value);
		}

		inline std::vector<uint8_t> DecodeFormat(const std::string_view value) {
			if (!value.starts_with(kBase64Format)) {
				throw std::exception("Base64 decode format: invalid format prefix");
			}
			return base64::decode_into<std::vector<uint8_t>>(value.substr(kBase64Format.size()));
		}

		template <bool Throw = true>
		inline std::vector<uint8_t> Decode(const std::string_view value)
		{
			if constexpr (Throw) {
				return base64::decode_into<std::vector<uint8_t>>(value);
			}
			else {
				try {
					return base64::decode_into<std::vector<uint8_t>>(value);
				}
				catch (...) {
					return {};
				}
			}
		}
	}

	inline std::vector<uint8_t> DecodeFormat(const std::string_view value)
	{
		if (value.starts_with(Base64::kBase64Format)) {
			return Base64::Decode(value.substr(Base64::kBase64Format.size()));
		}
		else if (value.starts_with(Hex::kHexFormat)) {
			return Hex::Decode(value.substr(Hex::kHexFormat.size()));
		}
		else {
			throw std::exception("Decode format: unknown format prefix");
		}
	}


} // namespace ikea400::Bin
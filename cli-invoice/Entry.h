#pragma once
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

struct Entry
{
public:
	Entry() = default;
	Entry(
		std::string date,
		float hour,
		float rate,
		std::string description,
		std::string gst_code
	);

	// getter
	std::string get_date() const;
	float get_hour() const;
	float get_rate() const;
	std::string get_description() const;
	std::string get_gst_code() const;

	// setter
	void set_date(const std::string& date);
	void set_hour(float hour);
	void set_rate(float rate);
	void set_description(const std::string& description);
	void set_gst_code(const std::string& gst_code);

	// methods
	std::string to_string() const;

	static Entry from_json(const nlohmann::json& j);

private:
	std::string date_;
	float hour_ = 0.0f;
	float rate_ = 0.0f;
	std::string description_;
	std::string gst_code_;
};

#define VALIDATE_AND_SELECT(entry_value, default_value) validate_and_select(entry_value, default_value, #entry_value)

template <typename T>
inline T validate_and_select(const T& entry_value, const T& default_value, const char* field_name)
{
	if (entry_value == T{} && default_value == T{})
	{
		throw std::runtime_error("[Entry] Error: Both entry and default_entry have invalid values for field: " + std::string(field_name) + "!\n");
	}
	return entry_value != T{} ? entry_value : default_value;
}

template <>
inline float validate_and_select<float>(const float& entry_value, const float& default_value, const char* field_name)
{
	constexpr float epsilon = 1e-5;
	if (std::fabs(entry_value) < epsilon && std::fabs(default_value) < epsilon)
	{
		throw std::runtime_error("[Entry] Error: Both entry and default_entry have invalid values for floating-point field: " + std::string(field_name) + "!\n");
	}
	return std::fabs(entry_value) > epsilon ? entry_value : default_value;
}
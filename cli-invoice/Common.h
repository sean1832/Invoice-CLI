#pragma once
#include <string>

class Common
{
public:
	Common() = delete;

	
	/**
	 * \brief generate a string of current date and time
	 * \param format char format of the date and time, see https://en.cppreference.com/w/cpp/chrono/c/strftime
	 * \return formatted string of current date and time
	 */
	static std::string get_current_datetime(const char* format);

	static std::string get_current_pdf_path(const std::string& provider_name, const std::string& customer_name);
};
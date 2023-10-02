#include <string>
#include "Common.h"
#include "JsonHelper.h"
#include <ctime>
#include <sstream>

#include "Config.h"

std::string Common::get_current_datetime(const char* format)
{
	time_t raw_time;
	tm time_info;
	char buffer[80];
	time(&raw_time);
	// Use localtime_s, the thread-safe version of localtime, on Windows
	localtime_s(&time_info, &raw_time);
	strftime(buffer, sizeof(buffer), format, &time_info);
	std::string str_date(buffer);
	return str_date;
}

std::string Common::get_current_pdf_path(const std::string& provider_name, const std::string& customer_name)
{
	std::string modified_customer_name = customer_name;
	modified_customer_name.erase(std::remove(modified_customer_name.begin(), modified_customer_name.end(), ' '), modified_customer_name.end());

	std::string modified_provider_name = provider_name;
	modified_provider_name.erase(std::remove(modified_provider_name.begin(), modified_provider_name.end(), ' '), modified_provider_name.end());

	const std::string datetime = get_current_datetime("%y%m%d");
	std::ostringstream out_filename;
	out_filename << modified_provider_name << "_invoice-" << modified_customer_name << "-" << datetime;
	std::ostringstream out_path;
	out_path << CONFIG.get_global_path("archive_dir") << "/" << out_filename.str() << ".pdf";
	return out_path.str();
}



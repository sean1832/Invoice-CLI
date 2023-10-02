#include "Entry.h"
#include <sstream>

using namespace std;

Entry::Entry(std::string date, float hour, float rate,
             std::string description, std::string gst_code)
    : date_(std::move(date)), hour_(hour), rate_(rate), description_(std::move(description)), gst_code_(std::move(gst_code))
{
}

// getter
string Entry::get_date() const { return date_; }
float Entry::get_hour() const { return hour_; }
float Entry::get_rate() const { return rate_; }
string Entry::get_description() const { return description_; }
string Entry::get_gst_code() const { return gst_code_; }

// setter
void Entry::set_date(const string& date) { date_ = date; }
void Entry::set_hour(float hour) { hour_ = hour; }
void Entry::set_rate(float rate) { rate_ = rate; }
void Entry::set_description(const string& description) { description_ = description; }
void Entry::set_gst_code(const string& gst_code) { gst_code_ = gst_code; }

// methods
Entry Entry::from_json(const nlohmann::json& j)
{
	if (j.empty())
	{
		return {}; // return default Entry
	}
    Entry entry;
    if (j.contains("date") && j["date"].is_string())
        entry.set_date(j["date"]);

    if (j.contains("hours") && j["hours"].is_number())
        entry.set_hour(j["hours"]);

    if (j.contains("rate") && j["rate"].is_number())
        entry.set_rate(j["rate"]);

    if (j.contains("description") && j["description"].is_string())
        entry.set_description(j["description"]);

    if (j.contains("gst_code") && j["gst_code"].is_string())
        entry.set_gst_code(j["gst_code"]);

    return entry;
}

string Entry::to_string() const
{
	ostringstream oss;
    const string indent = "    "; // 4 spaces
    oss << endl
		<< indent << "date: " << date_ << endl
		<< indent << "hours: " << hour_ << endl
		<< indent << "rate: " << rate_ << endl
		<< indent << "description: " << description_ << endl
		<< indent << "gst_code: " << gst_code_;
    return oss.str();
}
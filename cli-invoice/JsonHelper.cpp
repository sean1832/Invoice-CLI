#include "JsonHelper.h"
#include <fstream>
#include <optional>
#include <stdexcept>

using namespace nlohmann;
using namespace std;


JsonHelper::JsonHelper(const std::string& file_path)
{
	read(file_path);
}

void JsonHelper::read(const std::string& file_path)
{
	ifstream file(file_path);
	if (!file.is_open())
	{
		throw runtime_error("[Json::read] Error: File " + file_path + " does not exist.");
	}
	file >> json_object_;
}

void JsonHelper::write(const std::string& file_path, const int& indentation) const
{
	ofstream file(file_path);
	if (!file.is_open())
	{
		throw runtime_error("[Json::write] Error: File " + file_path + " could not be opened.");
	}
	file << json_object_.dump(indentation);
}

void JsonHelper::write(const int& indentation) const
{
	cout << json_object_.dump(indentation);
}

void JsonHelper::set_json(const nlohmann::json& j)
{
	json_object_ = j;
}

template <typename T>
optional<T> JsonHelper::get(const std::string& key) const
{
	if (json_object_.contains(key))
	{
		try
		{
			return json_object_.at(key).get<T>();
		}
		catch (const nlohmann::json::type_error&)
		{
			return std::nullopt;
		}
	}
	return std::nullopt;
}

std::optional<nlohmann::json> JsonHelper::get_json(const std::string& key) const
{
	if (json_object_.contains(key))
	{
		return json_object_.at(key);
	}
	return std::nullopt;
}

vector<nlohmann::json> JsonHelper::get_array(const std::string& key) const
{
	if (json_object_.contains(key) && json_object_.at(key).is_array())
	{
		return json_object_.at(key).get<vector<nlohmann::json>>();
	}
	return {};
}

string JsonHelper::to_string(const int& indentation) const
{
	return json_object_.dump(indentation);
}

// explicit instantiation the get<T> method
template optional<int> JsonHelper::get<int>(const string& key) const;
template optional<double> JsonHelper::get<double>(const string& key) const;
template optional<string> JsonHelper::get<string>(const string& key) const;
template optional<bool> JsonHelper::get<bool>(const string& key) const;
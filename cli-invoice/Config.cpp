#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

using json = nlohmann::json;
using namespace std;

void Config::initialize(const std::string& dir)
{
	if (dir_.empty()) 
	{
		dir_ = dir;
	}
	else 
	{
		throw std::runtime_error("[Config::read_profile] Error: ConfigIO already initialized");
	}
}

// functions
std::vector<Profile> Config::read_profile(const std::string& filepath, const std::string& profile_type)
{
	std::ifstream infile(filepath);
	// check if file is open
	if (!infile.is_open())
	{
		throw runtime_error("[Config::read_profile] Error: cannot open file \"" + filepath + "\"");
	}

	// create a json object
	json j;
	infile >> j;
	infile.close();

	// cast to profiles
	json j_profiles = j[profile_type];
	std::vector<Profile> profiles;
	if (profile_type == "provider")
	{
		const Profile profile = Profile::from_json(j_profiles);

		// add to profiles
		profiles.push_back(profile);
	}
	else if (profile_type == "customers")
	{
		for (const json& profile_json : j_profiles)
		{
			Profile profile = Profile::from_json(profile_json);

			// add to profiles
			profiles.push_back(profile);
		}
	}
	else
	{
		throw runtime_error("[Config::read_profile] Error: invalid profile type \"" + profile_type + "\"");
	}
	return profiles;
}

EmailSender Config::read_email_sender(const std::string& filepath)
{
	std::ifstream infile(filepath);
	// check if file is open
	if (!infile.is_open())
	{
		throw runtime_error("[Config::read_email_sender] Error: cannot open file \"" + filepath + "\"");
	}
	// create a json object
	json j;
	infile >> j;
	infile.close();

	// cast to email sender
	EmailSender email_sender = EmailSender::from_json(j);
	return email_sender;
}

void Config::load_global_config(const std::string& filename)
{
	const std::string path = dir_ + "/" + filename;
	std::ifstream infile(path);
	if (!infile.is_open()) {
		throw std::runtime_error("[Config::load_global_config] Error: config cannot open file \"" + path + "\"");
	}
	infile >> global_config_;
	infile.close();
}

string Config::get_global_config(const std::string& key)
{
	if (!global_config_.contains(key))
	{
		throw std::runtime_error("[Config::get_global_config] Error: config cannot find key \"" + key + "\"");
	}
	return global_config_[key];
}

nlohmann::json Config::get_global_config_json(const std::string& key)
{
	if (!global_config_.contains(key))
	{
		throw std::runtime_error("[Config::get_global_config_json] Error: config cannot find key \"" + key + "\"");
	}
	return global_config_[key];
}

nlohmann::json Config::get_global_config_json()
{
	return global_config_;
}

std::string Config::get_global_path(const std::string& key)
{
	if (!global_config_.at("path").contains(key))
	{
		throw std::runtime_error("[Config::get_global_path] Error: config cannot find key \"" + key + "\"");
	}
	return global_config_.at("path").at(key);
}

std::string Config::get_global_executable(const std::string& key)
{
	if (!global_config_.at("path").at("executables").contains(key))
	{
		throw std::runtime_error("[Config::get_global_executable] Error: config cannot find key \"" + key + "\"");
	}
	return global_config_.at("path").at("executables").at(key);
}

nlohmann::json Config::get_global_excel_worker(const std::string& key)
{
	if (!global_config_.at("settings").at("excel_worker").contains(key))
	{
		throw std::runtime_error("[Config::get_global_excel_worker] Error: config cannot find key \"" + key + "\"");
	}
	return global_config_.at("settings").at("excel_worker").at(key);
}

nlohmann::json Config::get_global_main(const std::string& key)
{
	if (!global_config_.at("settings").at("main").contains(key))
	{
		throw std::runtime_error("[Config::get_global_invoice] Error: config cannot find key \"" + key + "\"");
	}
	return global_config_.at("settings").at("main").at(key);
}




#pragma once
#include <string>
#include "Profile.h"
#include "Email.h"	
#include <vector>
#include <nlohmann/json.hpp>

class Config
{
public:
	// instance
	static Config& instance()
	{
		static Config instance;
		return instance;
	}

	void initialize(const std::string& dir);

	Config(const Config&) = delete; // Deleted copy constructor
	void operator=(const Config&) = delete; // Deleted copy assignment operator

	Config(Config&&) = default; // Default move constructor
	Config& operator=(Config&&) = default; // Default move assignment operator

	~Config() = default; // Default destructor

	// functions
	static std::vector<Profile> read_profile(const std::string& filepath, const std::string& profile_type);
	static EmailSender read_email_sender(const std::string& filepath);

	void load_global_config(const std::string& filename);
	std::string get_global_config(const std::string& key);
	nlohmann::json get_global_config_json(const std::string& key);
	nlohmann::json get_global_config_json();
	std::string get_global_path(const std::string& key);
	std::string get_global_executable(const std::string& key);
	nlohmann::json get_global_excel_worker(const std::string& key);
	nlohmann::json get_global_main(const std::string& key);

private:
	explicit Config() = default;
	std::string dir_;
	nlohmann::json global_config_;
};

#define CONFIG Config::instance()


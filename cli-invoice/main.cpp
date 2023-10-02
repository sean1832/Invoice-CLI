#include <iostream>
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <OpenXLSX/OpenXLSX.hpp>

#include "Profile.h"
#include "ExcelWriter.h"
#include "Config.h"
#include "IO.h"
#include "Common.h"
#include "Entry.h"
#include "Console.h"
#include "JsonHelper.h"
#include"Email.h"
#include "ExcelWorkerWrapper.h"
#include "Helper.h"
#include "ModuleInfo.h"

using namespace std;
using namespace nlohmann;
namespace po = boost::program_options;


int write_(std::vector<int> client_ids, write_mode& write_mode, const po::variables_map& vm, const Entry& entry)
{
	// check if mode is append
	if (vm.count("append"))
	{
		write_mode = append;
	}
	else
	{
		write_mode = overwrite;
	}
	string profile_dir = CONFIG.get_global_path("profiles");
	std::vector<Profile> client_profiles = CONFIG.read_profile(profile_dir, "customers");
	Profile provider_profile = CONFIG.read_profile(profile_dir, "provider")[0];
			

	// start recording time
	auto start = std::chrono::high_resolution_clock::now();

	// write to excel
	ExcelWriter writer(provider_profile);
	for (const auto& id : client_ids)
	{
		Profile* client_profile = Profile::search_by_id(client_profiles, id);
		if (client_profile == nullptr)
		{
			throw runtime_error("[main] Error: client id " + std::to_string(id) + " not found in profile");
		}
		Entry default_entry = client_profile->get_default_entry();

		std::string path = CONFIG.get_global_path("instance_dir") + "/instance_" + std::to_string(client_profile->get_id()) + ".xlsx";

		writer.write(path, entry, client_profile, write_mode);
	}
	// stop recording time
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<std::chrono::microseconds>(stop - start);

	// Convert the duration to seconds for display
	const double durationInSeconds = duration.count() / 1e6; // 1e6 microseconds in a second
	success_message("\nExcel written.");
	std::cout << "=======================================================\n";
	for (auto it = client_ids.begin(); it != client_ids.end(); ++it)
	{
		const auto& id = *it;
		const Profile* client_profile = Profile::search_by_id(client_profiles, id);
		if (client_profile == nullptr)
		{
			throw runtime_error("[main] Error: client id " + std::to_string(id) + " not found in profile\n");
		}
		Entry default_entry = client_profile->get_default_entry();

		std::cout << "client id: " << id << "\n";
		std::cout << "date: " << VALIDATE_AND_SELECT(entry.get_date(), default_entry.get_date()) << "\n";
		std::cout << "hours: " << VALIDATE_AND_SELECT(entry.get_hour(), default_entry.get_hour()) << "\n";
		std::cout << "rate: " << VALIDATE_AND_SELECT(entry.get_rate(), default_entry.get_rate()) << "\n";
		std::cout << "description: " << VALIDATE_AND_SELECT(entry.get_description(), default_entry.get_description()) << "\n";
		std::cout << "gst code: " << VALIDATE_AND_SELECT(entry.get_gst_code(), default_entry.get_gst_code()) << "\n";
		std::cout << "write mode: " << write_mode << "\n";

		// Check if this is the last element in the loop
		if (std::next(it) != client_ids.end())
		{
			std::cout << "-------------------------------------------------------\n";
		}
	}
	std::cout << "=======================================================\n";
	cout << "Time taken: " << durationInSeconds << " seconds\n";
	success_message("Done");
	return 0;
}

int export_(const std::vector<int>& client_ids)
{
	const string profile_path = CONFIG.get_global_path("profiles");
	const std::vector<Profile> client_profiles = CONFIG.read_profile(profile_path, "customers");
	const Profile provider_profile = CONFIG.read_profile(profile_path, "provider")[0];

	for (const auto& id : client_ids)
	{
				
		string customer_name = Profile::search_by_id(client_profiles, id)->get_name();
		string provider_name = provider_profile.get_name();

		const string in_path = CONFIG.get_global_path("instance_dir") + "/instance_" + std::to_string(id) + ".xlsx"; // excel file
		string out_path = Common::get_current_pdf_path(provider_name, customer_name); // pdf file
		ExcelWorker excel_worker(in_path);
		// start recording time
		auto start = std::chrono::high_resolution_clock::now();

		excel_worker.export_to_pdf(out_path, 1);

		// stop recording time
		auto stop = std::chrono::high_resolution_clock::now();

		// Convert the duration to seconds for display
		auto duration = chrono::duration_cast<std::chrono::microseconds>(stop - start);
		const auto duration_in_seconds = duration.count() / 1e6; // 1e6 microseconds in a second
		cout << "Time taken: " << duration_in_seconds << " seconds\n";
		success_message("PDF exported.");
	}
	return 0;
}

int send_(const std::vector<int>& client_ids)
{
	// start recording time
	auto start = std::chrono::high_resolution_clock::now();

	string profile_dir = CONFIG.get_global_path("profiles");
	string credential_dir = CONFIG.get_global_path("credential");

	std::vector<Profile> client_profiles = CONFIG.read_profile(profile_dir, "customers");
	Profile provider_profile = CONFIG.read_profile(profile_dir, "provider")[0];
	EmailSender sender = CONFIG.read_email_sender(credential_dir);
	for (auto id : client_ids)
	{
		// get client profile
		Profile* client_profile = Profile::search_by_id(client_profiles, id);
		EmailRecipient recipient = client_profile -> get_email_recipient();
		string subject_with_tag = recipient.get_subject();
		string subject = Helper::replace_tag(subject_with_tag, "<dt>", Common::get_current_datetime("%d - %m - %y"));
		recipient.set_subject(subject);

		std::string client_name = client_profile -> get_name();
		std::string provider_name = provider_profile.get_name();
		// add attachment
		recipient.set_attachment(Common::get_current_pdf_path(provider_name, client_name));

		Email::send(sender, recipient);
		success_message("Send");
	}
	// stop recording time
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<std::chrono::microseconds>(stop - start);
	const double duration_in_seconds = duration.count() / 1e6; // 1e6 microseconds in a second
	cout << "Time taken by function: " << duration_in_seconds << " seconds" << endl;

	return 0;
}

int open_(const string& config_select)
{
	const string credential_path = CONFIG.get_global_path("credential");
	const string profile_path = CONFIG.get_global_path("profiles");
	const string config_path = CONFIG.get_global_path("config");
	const vector<string> text_editors = CONFIG.get_global_main("text_editor");

	if (!Helper::file_exist(credential_path))
	{
		throw runtime_error("[main] Error: credential file does not exist at " + credential_path);
	}
	if (!Helper::file_exist(config_path))
	{
		throw runtime_error("[main] Error: config file does not exist at " + config_path);
	}
	if (!Helper::file_exist(profile_path))
	{
		throw runtime_error("[main] Error: profile file does not exist at " + profile_path);
	}

	if (config_select == "credential")
	{
		Helper::open_file(credential_path, text_editors);
	}
	else if (config_select == "config")
	{
		Helper::open_file(config_path, text_editors);
	}
	else if (config_select == "profile" || config_select == "profiles")
	{
		Helper::open_file(profile_path, text_editors);
	}
	else
	{
		throw runtime_error("[main] Error: sub-command not recognized. \"" + config_select + "\"");
	}

	return 0;
}

int show_()
{
	const string profile_path = CONFIG.get_global_path("profiles");
	if (!Helper::file_exist(profile_path))
	{
		throw runtime_error("[main] Error: profile file does not exist at " + profile_path);
	}
	JsonHelper profile_j;
	profile_j.read(profile_path);
	const vector<json> customers = profile_j.get_array("customers");

	cout << "customers profiles\n";
	cout << "-------------------------------------------------------\n";
	for (const json& customer : customers)
	{
		/*string output = JsonHelper::to_string(customer);*/
		Profile profile = Profile::from_json(customer);
		string profile_str = profile.to_string();
		cout << profile_str << "\n";
		cout << "-------------------------------------------------------\n\n\n";
	}
	return 0;
}

// main function
int main(int argc, char *argv[])
{
	// initialize variables
	std::string client_id_str, command, config_select, date;
	float hour, rate;

	try
	{
		CONFIG.initialize("assets");
		CONFIG.load_global_config("config.json");
	}
	catch (...)
	{
		error_message("[Main] Failed to load configuration files.");
		return 1;
	}
	

	// parse command line arguments
	try
	{
		write_mode write_mode;
		std::vector<int> client_ids;
		// Define Command Descriptions
		std::map<std::string, std::string> commandDescriptions = {
			{"write", "Execute write operations on the invoice."},
			{"export", "Export details (please replace with actual description)."},
			{"send", "Send details (please replace with actual description)."},
			{"write-export", "Execute write and export (please replace with actual description)."},
			{"write-export-send", "Execute write, export, and send (please replace with actual description)."}
		};

		// Define the Write commands.
		po::options_description writeDesc("Write Operation Options");
		writeDesc.add_options()
			("date,d", po::value<std::string>(&date), "set invoice date")
			("hour,H", po::value<float>(&hour), "set work hour")
			("rate,r", po::value<float>(&rate), "set rate")
			("id,i", po::value<std::string>(&client_id_str), "(int) set customer id")
			("append", "append this line to Excel table")
			("reset", "reset Excel file to original state");

		// Define the Config command.
		po::options_description configDesc("Configuration Options");
		configDesc.add_options()
			("show", "show all profiles")
			("open", po::value<std::string>(&config_select), "open a configuration file [credential, profiles, config]");

		// Define the main commands
		po::options_description mainDesc("Main Commands");
		mainDesc.add_options()
			("help,h", "print help message")
			("command", po::value<std::string>(&command), "command to execute [write, export, send, write-export, write-export-send]");

		// Combine all options into one
		po::options_description combinedDesc;
		combinedDesc.add(mainDesc).add(writeDesc).add(configDesc);

		// Define which options are positional.
		po::positional_options_description p;
		p.add("command", 1);

		// Parse the command-line arguments and store the values in a variables_map object.
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(combinedDesc).positional(p).run(), vm);
		po::notify(vm);

		// If help message is entered or no command entered
		if (vm.count("help") || (!vm.count("command") && !vm.count("show") && !vm.count("open"))) 
		{
			// get version information
			string version;
			ModuleInfo moduleInfo(GetModuleHandle(nullptr));
			unsigned int major, minor, build, revision;
			if (moduleInfo.get_file_version(major, minor, build, revision))
			{
				std::stringstream ss;
				ss << major << "." << minor << "." << build << "." << revision;
				version = ss.str();
			}
			string app_name = moduleInfo.get_string_file_info(L"ProductName");
			string description = moduleInfo.get_string_file_info(L"FileDescription");
			string copy_right = moduleInfo.get_string_file_info(L"LegalCopyright");
			// print help message
			std::cout << app_name << " " << version << "\n";
			std::cout << description << "\n";
			std::cout << copy_right << "\n\n";
			std::cout << "Usage: " << argv[0] << " <command> [options]\n\n";

			std::cout << "Available Commands:\n";
			for (const auto& [cmd, desc] : commandDescriptions) {
				std::cout << "  " << cmd << "    " << desc << "\n";
			}

			std::cout << "\n" << combinedDesc << "\n";
			// You can add examples at the end, if needed.
			std::cout << "Example Usage:\n  " << argv[0] << " write-export-send -i 1 -d \"23/09/2023\"\n";
			return 1;
		}

		// parsing command
		// parsing write mode
		write_mode = vm.count("append") ? append : overwrite;

		// parsing entry inputs
		Entry entry;
		if (vm.count("date")) entry.set_date(date);
		if (vm.count("hour")) entry.set_hour(hour);
		if (vm.count("rate")) entry.set_rate(rate);

		// parsing client id
		if (client_id_str.find(','))
		{
			std::vector<std::string> client_to_mod_vec;
			std::stringstream ss(client_id_str);
			std::string temp;
			while (std::getline(ss, temp, ','))
			{
				// convert string to int and push to vector
				client_ids.push_back(std::stoi(temp));
			}
		}
		else
		{
			client_ids.push_back(std::stoi(client_id_str));
		}

		// Validate the options based on the command.
		if (command == "send" && !vm.count("id"))
		{
			throw runtime_error("[main] Error: --id or -i is required for send command");
		}
		if (command == "write" && !vm.count("id"))
		{
			throw runtime_error("[main] Error: --id or -i is required for write command");
		}


		// ========================================
		// executing operation
		// ========================================
		// send

		// config
		if (vm.count("show"))
		{
			return show_();
		}
		if (vm.count("open"))
		{
			return open_(config_select);
		}

		if (command == "send")
		{
			return send_(client_ids);
		}

		// write
		if (command == "write")
		{
			return write_(client_ids, write_mode, vm, entry);
		}

		// export
		if (command == "export")
		{
			return export_(client_ids);
		}

		// chain command
		if (command == "write-export")
		{
			write_(client_ids, write_mode, vm, entry);
			return export_(client_ids);
		}
		if (command == "write-export-send")
		{
			write_(client_ids, write_mode, vm, entry);
			export_(client_ids);

			char send_choice;
			// prompt user to send
			cout << "Send invoice? [y/n]: ";
			cin >> send_choice;
			send_choice = tolower(send_choice);
			if (send_choice == 'y')
				return send_(client_ids);

			warning_message("Invoice not sent");
			return 0;
		}

		// debug
		if (command == "debug")
		{
			cout << "Debug mode" << endl;
			return 0;
		}

		throw runtime_error("[main] Error: \"" + command + "\" is not a valid command");
	}
	catch (const std::exception& e)
	{
		error_message("[main] Exit with " + std::string(e.what()));
		return 1;
	}
	catch (...)
	{
		error_message("[main] Unknown error!\n Please contact author of this software (sean1832725142@gmail.com).");
		return 2;
	}
}

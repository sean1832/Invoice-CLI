#include "ExcelWorkerWrapper.h"
#include "Common.h"
#include <filesystem>
#include <sstream>
#include <stdexcept>

#include "Config.h"

using namespace std;

ExcelWorker::ExcelWorker(std::string in_path) : in_path_(std::move(in_path))
{
}
string ExcelWorker::get_in_path() const { return in_path_; }
void ExcelWorker::set_in_path(const string& in_path) { in_path_ = in_path; }


void ExcelWorker::export_to_pdf(const std::string& out_path, const int worksheet) const
{
	if (in_path_.empty())
	{
		throw std::runtime_error("[ExcelWorkerWrapper] Error: in_path is empty");
	}
	if (out_path.empty())
	{
		throw std::runtime_error("[ExcelWorkerWrapper] Error: out_path is empty");
	}
	if (worksheet < 1)
	{
		throw std::runtime_error("[ExcelWorkerWrapper] Error: worksheet cannot be less than 1");
	}
	// check if ExcelWorker is installed
	// get current directory
	const filesystem::path current_dir = filesystem::current_path();
	const filesystem::path executable(CONFIG.get_global_executable("excel_worker"));

	// concatenate current directory with executable
	const filesystem::path executable_path = current_dir / executable;

	if (!exists(executable_path))
	{
		throw std::runtime_error("[ExcelWorkerWrapper] Error: " + executable_path.string() + " is not installed");
	}

	std::ostringstream ss;
	ss << "ExcelWorker -c pdf -p \"" << in_path_ << "\" -o \"" << out_path << "\" -w " << worksheet;
	const string cmd = ss.str();
	if (system(cmd.c_str()) != 0)
	{
		throw std::runtime_error("[ExcelWorkerWrapper] Error: failed to execute command");
	}
}

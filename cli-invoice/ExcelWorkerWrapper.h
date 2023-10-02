#pragma once
#include <string>

class ExcelWorker
{
private:
	std::string in_path_;
public:
	ExcelWorker() = default;
	ExcelWorker(std::string in_path);

	// getter
	std::string get_in_path() const;

	// setter
	void set_in_path(const std::string& in_path);

	void export_to_pdf(const std::string& out_path, int worksheet) const;
	void write_to_cell(int worksheet, int row, char* column, const std::string& value) const;
};


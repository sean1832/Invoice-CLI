#include "ExcelWriter.h"
#include <fstream>
#include <iostream>
#include <OpenXLSX/OpenXLSX.hpp>

#include "IO.h"
#include "Common.h"
#include "Config.h"
#include "Entry.h"
#include "Helper.h"
#include "Console.h"

using namespace std;

class CellOperator {
public:
	explicit CellOperator(const OpenXLSX::XLWorksheet& ws) : ws_(ws) {}

	// write
	void write(const std::string& cell, const std::string& value) const {
		ws_.cell(cell).value() = value;
	}

	void write(const std::string& cell, int value) const {
		ws_.cell(cell).value() = value;
	}

	void write(const std::string& cell, float value) const {
		ws_.cell(cell).value() = value;
	}

	void write(const std::string& cell, long long value) const {
		ws_.cell(cell).value() = value;
	}

	// get last row
	int get_last_row(const char column, const int init_row, const int max_row) const
	{
		int row = init_row;
		while (row < max_row)
		{
			std::string cell = ws_.cell(column + std::to_string(row)).value();
			bool is_empty = ws_.cell(column + std::to_string(row)).value() == "";
			if (is_empty)
				break; // Exit the loop if an empty cell is found
			++row; // Go to the next row if the current cell is not empty
		}

		if (row > max_row)
		{
			throw runtime_error("[writer] Error: max row [" + std::to_string(max_row) + "] reached. To expand this limits, adjust both excel template and settings.json.");
			std::exit(1);
		}
		std::cout << "[writer] Last row: " << row << "\n";
		return row;
	}


private:
	const OpenXLSX::XLWorksheet& ws_;
};

ExcelWriter::ExcelWriter(Profile provider)
: provider_(std::move(provider))
{
}

// functions
void ExcelWriter::write(
    const std::string& path,
	const Entry& entry,
	const Profile* customer,
    write_mode mode
) const
{
	const std::string template_path = CONFIG.get_global_path("excel_template");
	if (mode == overwrite) 
	{
		if (Helper::create_instance(template_path, path)) 
		{
			std::cout << "[writer] Created a copy of the template file." << "\n";
		}
		else 
		{
			throw runtime_error("[writer] Error: cannot create a copy of the template file.");
		}
	}
	if (mode == append)
	{
		if (!Helper::file_exist(path))
		{ 
			throw runtime_error("[writer] Error: File " + path + " not found for appending. If creating for the first time, run without --append.");
		}
	}


	OpenXLSX::XLDocument doc;
	try 
	{
		doc.open(path);
	}
	catch (const std::exception& e) 
	{
		throw runtime_error("[writer] Error loading workbook: " + std::string(e.what()) + " " + path);
	}

	if (doc.workbook().sheetExists("client"))
	{
		try
		{
			// get the worksheet named "client"
			const auto ws = doc.workbook().worksheet("client");

			// Get a cell and modify its value
			CellOperator operator_(ws);

			// provider
			operator_.write("B3", provider_.get_name());
			operator_.write("B4", provider_.get_address());
			operator_.write("B5", provider_.get_abn());
			operator_.write("B6", provider_.get_email());
			operator_.write("B7", provider_.get_phone());

			// customer
			operator_.write("B10", customer->get_name());
			operator_.write("B11", customer->get_address());
			operator_.write("B12", customer->get_phone());

			// payment
			operator_.write("A28", provider_.get_payment_methods());
			operator_.write("B28", provider_.get_account_name());
			operator_.write("C28", provider_.get_bsb());
			operator_.write("D28", provider_.get_account_number());

			// get default entry from customer profile
			const Entry default_entry = customer->get_default_entry();
			constexpr float epsilon = 1e-5;
			// invoice
			if (mode == overwrite)
			{
				std::cout << "[writer] Override mode" << "\n";
				float hour = VALIDATE_AND_SELECT(entry.get_hour(), default_entry.get_hour());
				float rate = VALIDATE_AND_SELECT(entry.get_rate(), default_entry.get_rate());
				float amount = hour * rate;

				operator_.write("A18", VALIDATE_AND_SELECT(entry.get_date(), default_entry.get_date()));
				operator_.write("B18", hour);
				operator_.write("C18", rate);
				operator_.write("D18", VALIDATE_AND_SELECT(entry.get_description(), default_entry.get_description()));
				operator_.write("E18", amount);
				operator_.write("F18", VALIDATE_AND_SELECT(entry.get_gst_code(), default_entry.get_gst_code()));
			}
			else if (mode == append)
			{
				std::cout << "[writer] Append mode" << "\n";
				const int last_row = operator_.get_last_row('A', 18, 22);
				const std::string last_row_str = std::to_string(last_row);
				float hour = VALIDATE_AND_SELECT(entry.get_hour(), default_entry.get_hour());
				float rate = VALIDATE_AND_SELECT(entry.get_rate(), default_entry.get_rate());
				float amount = hour * rate;

				operator_.write("A" + last_row_str, VALIDATE_AND_SELECT(entry.get_date(), default_entry.get_date()));
				operator_.write("B" + last_row_str, hour);
				operator_.write("C" + last_row_str, rate);
				operator_.write("D" + last_row_str, VALIDATE_AND_SELECT(entry.get_description(), default_entry.get_description()));
				operator_.write("E" + last_row_str, amount);
				operator_.write("F" + last_row_str, VALIDATE_AND_SELECT(entry.get_gst_code(), default_entry.get_gst_code()));
			}
			std::cout << "[writer] Written to " << path << "\n";


			// save the workbook (this will automatically close it)
			doc.save();
			std::cout << "[writer] Saved to " << path << "\n";

		}
		catch (const std::exception& e)
		{
			throw runtime_error("[writer] Error: " + std::string(e.what()));
		}
	}
	else
	{
		throw runtime_error("[writer] Error: The workbook does not contain a sheet named 'client'.");
	}
}

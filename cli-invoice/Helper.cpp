#include <filesystem>
#include <iostream>
#include "Helper.h"
#include <fstream>
#include <vector>

#include  "Console.h"


namespace fs = std::filesystem;
using namespace std;

void Helper::create_not_exist(const std::string& path, const path_type& type) {
    try {
        fs::path p(path);
        if (fs::exists(p)) {
            if (type == directory) {
                if (fs::is_directory(p)) {
                    std::cout << "[create not exist] Directory " << path << " already exists.\n";
                }
                else if (fs::is_regular_file(p)) {
                    std::cout << "[create not exist] A file exists at " << path << ", but a directory was expected.\n";
                    p = p.parent_path();
                    std::cout << "[create not exist] Using parent directory: " << p.string() << '\n';
                }
            }
            else if (type == file) {
                if (fs::is_regular_file(p)) {
                    std::cout << "[create not exist] File " << path << " already exists.\n";
                }
                else if (fs::is_directory(p)) {
                    throw runtime_error("[create not exist] Error: A directory exists at " + path + ", but a file was expected.");
                }
            }
        }
        else {
            if (type == directory) {
                if (fs::create_directories(p)) {
                    std::cout << "[create not exist] Directory " << path << " has been created.\n";
                }
                else {
                    throw runtime_error("[create not exist] Error: Directory " + path + " could not be created.");
                }
            }
            else if (type == file) {
                throw runtime_error("[create not exist] Error: File " + path + " does not exist.");
            }
        }
    }
    catch (const std::exception& e) {
        throw runtime_error("[create not exist] Error: " + std::string(e.what()));
    }
}

std::string Helper::get_parent_dir(const std::string& filePath) {
	const std::filesystem::path path(filePath);
    if (path.has_parent_path())
        return path.parent_path().string();
    else
        return ""; // return empty string if there is no directory
}

bool Helper::create_instance(const std::string& in_path, const std::string& out_path, const int& buffer_size)
{
    // create directories if not exist
    const std::string out_parent_dir = get_parent_dir(out_path);
    create_not_exist(out_parent_dir, directory);

    std::ifstream infile(in_path, std::ios::binary);
    std::ofstream outfile(out_path, std::ios::binary);

    if (!infile)
    {
        throw runtime_error("[create not exist] Error: cannot open file " + in_path);
    }
    if (!outfile)
    {
        throw runtime_error("[create not exist] Error: cannot open file " + out_path);
    }

    // define buffer
    const size_t size = buffer_size;
    // create a buffer tto hold the data
    std::vector<char> buffer(size);

    // continue reading infile until the end of the file is reached
    while (infile)
    {
        // Read up to bufferSize bytes from the source file into the buffer.
        infile.read(buffer.data(), buffer_size);
        // get the number of bytes actually read
        std::streamsize byte_read = infile.gcount();
        // if bytes have been read, write them to the output file
        if (byte_read > 0)
        {
            outfile.write(buffer.data(), byte_read);
        }
    }

    // close files
    outfile.close();
    infile.close();

    return true;
}

bool Helper::file_exist(const std::string& path)
{
	const std::ifstream infile(path.c_str());
    return infile.good();
}

std::string Helper::get_filename_from_path(const std::string& filepath, const bool exclude_extension) {
    // Find the last occurrence of directory separator
    const size_t pos = filepath.find_last_of("/\\");

    const size_t start_pos = (pos == std::string::npos) ? 0 : pos + 1;
    std::string filename = filepath.substr(start_pos);

    if (exclude_extension) {
        // Find the last occurrence of the dot in the filename
        const size_t dot_pos = filename.find_last_of('.');
        if (dot_pos != std::string::npos) {
            // Return the substring from the start to the position before the last dot
            return filename.substr(0, dot_pos);
        }
    }
    // If no dot was found or exclude_extension is false, return the filename as is
    return filename;
}


bool Helper::open_with(const std::string& filepath, const std::string& program) {
    if (!file_exist(filepath)) {
        std::cerr << "File does not exist: " << filepath << '\n';
        return false;
    }

    const std::string command = program + " " + filepath;
    return system(command.c_str()) == 0;
}

bool Helper::open_file(const std::string& filepath, const vector<std::string>& programs) {
    for (const auto& program : programs) {
        if (open_with(filepath, program)) {
            return true;
        }
    }
    return false;
}
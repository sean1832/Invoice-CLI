#pragma once

enum path_type { file, directory };

class Helper
{
public:
	static void create_not_exist(const std::string& path, const path_type& type);
	static std::string get_parent_dir(const std::string& filePath);
	static bool create_instance(const std::string& in_path, const std::string& out_path, const int& buffer_size = 4096);
	static bool file_exist(const std::string& path);
	static std::string get_filename_from_path(const std::string& filepath, bool exclude_extension = false);

	template <typename T>
	static std::string replace_tag(const std::string& str, const std::string& tag, const T& value);
    static bool open_with(const std::string& filepath, const std::string& program);
    static bool open_file(const std::string& filepath, const std::vector<std::string>& programs);
};

template <typename T>
std::string Helper::replace_tag(const std::string& str, const std::string& tag, const T& value) {
    std::string result = str;
    std::stringstream ss;
    ss << value;
    const std::string valueStr = ss.str();

    std::size_t pos = result.find(tag);
    while (pos != std::string::npos) {
        result.replace(pos, tag.length(), valueStr);
        pos = result.find(tag, pos + valueStr.length());
    }

    return result;
}
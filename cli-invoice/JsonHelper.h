#pragma once
#include <optional>
#include <nlohmann/json.hpp>
#include <string>

#include "Console.h"

class JsonHelper
{
private:
    nlohmann::json json_object_;
public:
    explicit JsonHelper(const std::string& file_path);
    JsonHelper() = default;

    // IO
    void read(const std::string& file_path);
    void write(const std::string& file_path, const int& indentation=4) const;
    void write(const int& indentation=4) const;

    // Method to set Json object directly
    void set_json(const nlohmann::json& j);

    // Method to access Json object
    template <typename T>
    std::optional<T> get(const std::string& key) const;

    std::optional<nlohmann::json> get_json(const std::string& key) const;
    std::vector<nlohmann::json> get_array(const std::string& key) const;

    // convert to string
    std::string to_string(const int& indentation=4) const;
};


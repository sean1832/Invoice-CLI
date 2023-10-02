#pragma once
#pragma comment(lib, "version.lib")
#include <string>
#include <windows.h>

class ModuleInfo
{
public:
	explicit ModuleInfo(HMODULE hModule);

    // Get version information
    bool get_file_version(unsigned& major, unsigned& minor, unsigned& build, unsigned& revision) const;

    // Get other string information if needed
    std::string get_string_file_info(const std::wstring& subBlock) const;

private:
    std::string get_string_file_info(const char* subBlock) const;

    HMODULE m_hModule;
    mutable char* m_pVersionInfoData;
};


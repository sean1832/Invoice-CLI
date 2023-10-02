#include "ModuleInfo.h"
#include <string>

std::string ws2s(const std::wstring& wstr)
{
    if (wstr.empty())
        return {};

    const int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), strTo.data(), size_needed, nullptr, nullptr);
    return strTo;
}

ModuleInfo::ModuleInfo(HMODULE hModule) : m_hModule(hModule), m_pVersionInfoData(nullptr)
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(m_hModule, path, MAX_PATH); // Use wide character version

    DWORD verHandle = 0;
    const DWORD verSize = GetFileVersionInfoSizeW(path, &verHandle); // Use wide character version

    if (verSize > 0) {
        m_pVersionInfoData = new char[verSize];
        if (!GetFileVersionInfoW(path, verHandle, verSize, m_pVersionInfoData)) { // Use wide character version
            delete[] m_pVersionInfoData;
            m_pVersionInfoData = nullptr;
        }
    }
}


bool ModuleInfo::get_file_version(unsigned& major, unsigned& minor, unsigned& build, unsigned& revision) const
{
    if (m_pVersionInfoData) {
        UINT size = 0;
        VS_FIXEDFILEINFO* verInfo = nullptr;

        if (VerQueryValue(m_pVersionInfoData, L"\\", reinterpret_cast<LPVOID*>(&verInfo), &size) && size) {
            major = HIWORD(verInfo->dwFileVersionMS);
            minor = LOWORD(verInfo->dwFileVersionMS);
            build = HIWORD(verInfo->dwFileVersionLS);
            revision = LOWORD(verInfo->dwFileVersionLS);
            return true;
        }
    }

    return false;
}

std::string ModuleInfo::get_string_file_info(const std::wstring& subBlock) const
{
    if (m_pVersionInfoData) {
        UINT size = 0;
        LPVOID buffer = nullptr;
        const std::wstring query = L"\\StringFileInfo\\040904b0\\" + subBlock; //040904b0 represents Unicode language code

        if (VerQueryValueW(m_pVersionInfoData, query.c_str(), &buffer, &size) && size) {
	        const std::wstring value = static_cast<wchar_t*>(buffer);
            return ws2s(value); // convert wide string to string
        }
    }
    return {};
}



std::string ModuleInfo::get_string_file_info(const char* subBlock) const
{
    if (m_pVersionInfoData) {
        UINT size = 0;
        wchar_t* buffer = nullptr;

        if (VerQueryValueW(m_pVersionInfoData, L"\\StringFileInfo\\040904b0\\FileDescription", reinterpret_cast<LPVOID*>(&buffer), &size) && size) { // Use wide character version
            return ws2s(buffer); // Convert to narrow string
        }
    }

    return {};
}



// usage example:
//#include "ModuleInfo.h"
//#include <iostream>
//
//int main()
//{
//    std::string version;
//    ModuleInfo moduleInfo(GetModuleHandle(nullptr));
//    unsigned int major, minor, build, revision;
//    if (moduleInfo.get_file_version(major, minor, build, revision))
//    {
//        std::stringstream ss;
//        ss << major << "." << minor << "." << build << "." << revision;
//        version = ss.str();
//    }
//    std::string app_name = moduleInfo.get_product_name();
//    std::string description = moduleInfo.get_file_description();
//    std::string copy_right = moduleInfo.get_copy_right();
//
//    return 0;
//}
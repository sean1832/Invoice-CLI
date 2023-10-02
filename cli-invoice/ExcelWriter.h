#pragma once
#include <string>
#include "Profile.h"
#include "IO.h"

class ExcelWriter
{
public:
    // constructor
    ExcelWriter(Profile provider); // Pass by const reference

    // functions
    void write(
        const std::string& path,
        const Entry& entry, // Pass by const reference
        const Profile* customer, // Pass by const reference
        write_mode mode
    ) const; // Pass string by const reference, and added const-correctness

private:
    Profile provider_;
};

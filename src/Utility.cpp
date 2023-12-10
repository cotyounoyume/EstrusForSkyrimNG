#pragma once
#include "Utility.h"

namespace Utility {

    std::string GetCurrentExecutableDirectory() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }

}
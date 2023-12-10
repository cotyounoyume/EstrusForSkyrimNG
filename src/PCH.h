#pragma once

// This file is required.

#include "fmt/format.h"

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

namespace logs = SKSE::log;

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

using namespace std::literals;
using namespace RE::BSScript;

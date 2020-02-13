#pragma once

#ifndef NO_COVSCRIPT
#include <covscript/core/core.hpp>
#endif

namespace covscript_process {
#ifndef NO_COVSCRIPT
	using runtime_exception = cs::lang_error;
	using critical_exception = cs::runtime_error;
#else
	using runtime_exception = std::logic_error;
	using critical_exception = std::runtime_error;
#endif
}
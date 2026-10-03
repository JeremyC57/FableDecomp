// printf/scanf family over guest varargs, with MSVC 7.1 semantics
// (%s in wide functions is a wide string, %S the opposite width, %p is 8 hex digits).
#pragma once
#include "host.hpp"

#include <string>

namespace host {
// `ap` is the guest address of the first variadic argument; advanced as consumed.
std::string formatA(const char* fmt, uint32_t& ap);
std::wstring formatW(const wchar_t* fmt, uint32_t& ap);
// sscanf: all conversion targets are guest pointers, passed through.
int scanA(const char* input, const char* fmt, uint32_t ap);
// Copies with _snprintf semantics: returns length, or -1 if truncated (no terminator then).
int copyTruncA(char* dst, uint32_t n, const std::string& s);
int copyTruncW(wchar_t* dst, uint32_t n, const std::wstring& s);
}  // namespace host

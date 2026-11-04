// Behaviour test for CWideString::Format (0x0099BA70), g++ model.
#include <cstdio>
#include <cstdarg>
#include <cwchar>
#include <vector>
#include <string>
static bool Format(std::wstring* result, bool* assigned, const wchar_t* format, ...)
{
    wchar_t local[8000];
    long size = (long)wcslen(format);
    std::vector<wchar_t> heap;
    wchar_t* buffer;
    if (size > 4000) { heap.resize(size + 4000); buffer = &heap[0]; } else buffer = local;
    size += 4000;
    va_list args; va_start(args, format);
    int count = vswprintf(buffer, size - 1, format, args);
    va_end(args);
    buffer[size - 1] = 0;
    *assigned = false;
    if (count <= size) { result->clear(); if (*buffer) { *result = buffer; *assigned = true; } }
    return *assigned;
}
int main()
{
    std::wstring s; bool a;
    Format(&s, &a, L"%d-%ls", 42, L"x");
    int ok = a && s == L"42-x";
    Format(&s, &a, L"");
    ok &= !a && s.empty();
    std::wstring longFmt(4100, L'a');
    Format(&s, &a, longFmt.c_str());
    ok &= a && s.size() == 4100;
    std::printf(ok ? "0099ba70_TEST PASS\n" : "0099ba70_TEST FAIL\n");
    return ok ? 0 : 1;
}

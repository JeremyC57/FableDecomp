// Behaviour test for 0x009C7B70 NGameText::ConstructString(CWideString&, long).
// NF146 (2026-11-20, q4285): rewritten for the PDB overload; links the row object and fakes the 0x009C7A60 callee to record the
//   -1-terminated list and the output reference. CWideString comes from fable_string.h (only a reference is formed).
#include <cstdio>
#include <cstdarg>
#include "fable_string.h"

static long g_args[8];
static int g_count = 0;
static CWideString* g_output = 0;

namespace NGameText {
bool __cdecl ConstructStringInternal(CWideString& output, long first, ...)
{
    g_output = &output; g_count = 0; long value = first;
    va_list ap; va_start(ap, first);
    while (g_count < 8) { g_args[g_count++] = value; if (value == -1) break; value = va_arg(ap, long); }
    va_end(ap);
    return g_count == 2;
}
bool __fastcall ConstructString(CWideString& output, long a);
}

int main()
{
    static double storage[4];
    CWideString& out = *(CWideString*)storage;
    bool r = NGameText::ConstructString(out, 101);
    if (!r || g_output != &out || g_count != 2 || g_args[0] != 101 || g_args[1] != -1) { std::printf("009c7b70_TEST FAIL\n"); return 1; }
    std::printf("009c7b70_TEST PASS\n");
    return 0;
}

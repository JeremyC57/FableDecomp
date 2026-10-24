#include <cstdio>
#include <cstdarg>

class CWideString { public: int Tag; };

static long g_args[8];
static int g_count = 0;
static CWideString* g_output = 0;

namespace NGameText {
bool ConstructStringInternal(CWideString& output, long first, ...)   // stand-in: records the -1-terminated list
{
    g_output = &output;
    g_count = 0;
    long value = first;
    va_list ap;
    va_start(ap, first);
    while (g_count < 8) {
        g_args[g_count++] = value;
        if (value == -1) break;
        value = va_arg(ap, long);
    }
    va_end(ap);
    return g_count == 4;
}

bool ConstructString(CWideString& output, long a, long b, long c)
{
    return ConstructStringInternal(output, a, b, c, -1L);   // -1L: the 64-bit g++ host reads a long from the list
}
}

int main()
{
    CWideString out;
    out.Tag = 7;
    bool r = NGameText::ConstructString(out, 101, 202, 303);
    if (!r || g_output != &out || g_count != 4 || g_args[0] != 101 || g_args[1] != 202 || g_args[2] != 303 || g_args[3] != -1) {
        std::printf("009c7ba0_TEST FAIL\n"); return 1;
    }
    r = NGameText::ConstructString(out, 0, 5, 9);
    if (!r || g_args[0] != 0 || g_args[1] != 5 || g_args[2] != 9 || g_args[3] != -1) { std::printf("009c7ba0_TEST FAIL\n"); return 1; }
    std::printf("009c7ba0_TEST PASS\n");
    return 0;
}
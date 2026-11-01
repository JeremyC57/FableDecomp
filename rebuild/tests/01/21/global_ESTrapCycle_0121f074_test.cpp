// Behaviour test for 0x0121F074 (dynamic initializer of the anonymous-namespace CCharString STrapCycle): the object is built
// from the literal with length -1 (= strlen) before main, and its destructor runs once at exit.
#include <cstdio>
#include <cstring>

static int g_constructed = 0;
static int g_destroyed = 0;

class CCharString {                                      // model: copies the text, length -1 = up to the terminator
public:
    CCharString(const char* text, long length)
    {
        if (length < 0) length = (long)std::strlen(text);
        if (length > 63) length = 63;
        std::memcpy(Text, text, (size_t)length);
        Text[length] = 0;
        Length = length;
        ++g_constructed;
    }
    ~CCharString() { ++g_destroyed; }
    char Text[64];
    long Length;
};

namespace {
CCharString STrapCycle("TRAP_CONTINUOUS", -1);
}

struct CExitCheck {
    ~CExitCheck() { if (g_destroyed != 1) std::printf("0121f074_TEST FAIL (destructor)\n"); }
};

int main()
{
    if (g_constructed != 1 || std::strcmp(STrapCycle.Text, "TRAP_CONTINUOUS") != 0 ||
        STrapCycle.Length != (long)std::strlen("TRAP_CONTINUOUS") || g_destroyed != 0) {
        std::printf("0121f074_TEST FAIL\n"); return 1;
    }
    std::printf("0121f074_TEST PASS\n");
    return 0;
}
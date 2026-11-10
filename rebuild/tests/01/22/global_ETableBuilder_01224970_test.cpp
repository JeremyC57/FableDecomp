// Behaviour test for the dynamic initializer @ 0x01224970: the file-scope TableBuilder is constructed once before main, and its
// constructor fills the 256-entry highest-set-bit table (entry 0 = -1, entry i = floor(log2(i))).
#include <cstdio>
static signed char g_highestSetBitTable[256];
static int g_ctorCalls = 0;
struct CEngineInlineTableBuilder {
public:
    CEngineInlineTableBuilder()
    {
        ++g_ctorCalls;
        for (int i = 0; i < 256; ++i) {
            int r = -1;
            for (unsigned v = (unsigned)i; v; v >>= 1) ++r;
            g_highestSetBitTable[i] = (signed char)r;
        }
    }
};
CEngineInlineTableBuilder TableBuilder;
int main()
{
    bool ok = g_ctorCalls == 1 && g_highestSetBitTable[0] == -1 && g_highestSetBitTable[1] == 0 && g_highestSetBitTable[2] == 1
           && g_highestSetBitTable[3] == 1 && g_highestSetBitTable[128] == 7 && g_highestSetBitTable[255] == 7;
    if (!ok) { std::printf("01224970_TEST FAIL\n"); return 1; }
    std::printf("01224970_TEST PASS\n");
    return 0;
}
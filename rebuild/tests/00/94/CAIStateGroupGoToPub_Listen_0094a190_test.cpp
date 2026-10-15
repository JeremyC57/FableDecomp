// Behaviour test for CAIStateGroup_GoToPub::Listen (0x0094A190): PlayAnims gets the three listening
// animations in INTO / LOOP / OUTOF order with one loop, its result is returned, and all three strings die.
#include <cstdio>
#include <cstring>

static int g_dtors = 0;
static bool g_result = true;
static bool g_argsOk = false;

struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long) : m_pData(text) {}
    ~CCharString() { ++g_dtors; }
};

struct CAIStateGroup_GoToPub {
    bool PlayAnims(const CCharString& into, const CCharString& loop, const CCharString& outOf, long loops)
    {
        g_argsOk = std::strcmp(into.m_pData, "LISTENING_INTO") == 0 && std::strcmp(loop.m_pData, "LISTENING_LOOP") == 0
            && std::strcmp(outOf.m_pData, "LISTENING_OUTOF") == 0 && loops == 1 && g_dtors == 0;
        return g_result;
    }
    bool Listen()
    {
        return PlayAnims(CCharString("LISTENING_INTO", -1), CCharString("LISTENING_LOOP", -1),
            CCharString("LISTENING_OUTOF", -1), 1);
    }
};

int main()
{
    CAIStateGroup_GoToPub g;
    bool ok = g.Listen() == true && g_argsOk && g_dtors == 3;
    g_dtors = 0; g_result = false;
    ok = ok && g.Listen() == false && g_argsOk && g_dtors == 3;
    if (!ok) { std::printf("0094a190_TEST FAIL\n"); return 1; }
    std::printf("0094a190_TEST PASS\n");
    return 0;
}
#include <cstdio>
class CThing;
class CGameScriptInterface { public:
    bool CanThingBe_Seen_ByOtherThing(CThing* thing1, CThing* thing2);
    bool CanSeeImpl(CThing* thing1, CThing* thing2, int mode);
};
static int g_last_mode = -1;
static CThing* g_last_a = 0;
static CThing* g_last_b = 0;
static CGameScriptInterface* g_last_this = 0;
static bool g_ret = false;

bool CGameScriptInterface::CanSeeImpl(CThing* thing1, CThing* thing2, int mode)
{
    g_last_this = this; g_last_a = thing1; g_last_b = thing2; g_last_mode = mode;
    return g_ret;
}

bool CGameScriptInterface::CanThingBe_Seen_ByOtherThing(CThing* thing1, CThing* thing2)
{
    return CanSeeImpl(thing1, thing2, 1);
}

int main()
{
    CGameScriptInterface obj;
    CThing* thing1 = (CThing*)0x1000;
    CThing* thing2 = (CThing*)0x2000;
    g_ret = true;
    bool r = obj.CanThingBe_Seen_ByOtherThing(thing1, thing2);
    if (!r) { std::printf("FAIL: expected true\n"); return 1; }
    if (g_last_this != &obj || g_last_a != thing1 || g_last_b != thing2 || g_last_mode != 1) { std::printf("FAIL: args wrong\n"); return 1; }
    g_ret = false;
    r = obj.CanThingBe_Seen_ByOtherThing(thing1, thing2);
    if (r) { std::printf("FAIL: expected false\n"); return 1; }
    std::printf("CGameScriptInterface_0089cd10_TEST PASS\n");
    return 0;
}
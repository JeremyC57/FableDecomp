#include <cstdio>
#include <cstring>
// Behaviour test for CThingPhysical::OnCreate (0x006A4D20): sound-player component added, base result normalised.
static int g_live = 0;
struct CCharString {
    char text[32];
    CCharString(const char* t, long) { std::strcpy(text, t); ++g_live; }
    ~CCharString() { --g_live; }
};
struct CThingPhysical {
    char added[32]; bool addedActive; void* addedParams; int liveDuringAdd; unsigned char baseResult;
    void AddTC(const CCharString& name, bool activate, void* params)
    { std::strcpy(added, name.text); addedActive = activate; addedParams = params; liveDuringAdd = g_live; }
    unsigned char BaseOnCreate() { return baseResult; }
    bool OnCreate()
    {
        { CCharString name("CTCSoundPlayer", -1); AddTC(name, false, 0); }
        return BaseOnCreate() ? true : false;
    }
};
int main()
{
    CThingPhysical t; t.added[0] = 0; t.addedActive = true; t.addedParams = &t;
    int fails = 0;
    t.baseResult = 7;
    if (!t.OnCreate()) ++fails;
    if (std::strcmp(t.added, "CTCSoundPlayer") != 0 || t.addedActive || t.addedParams || t.liveDuringAdd != 1 || g_live != 0) ++fails;
    t.baseResult = 0;
    if (t.OnCreate()) ++fails;
    if (fails) { std::printf("006a4d20_TEST FAIL\n"); return 1; }
    std::printf("006a4d20_TEST PASS\n");
    return 0;
}

// Test for 004dcd2b CTCKrakenTentacle::~CTCKrakenTentacle: members destroyed in reverse declaration order (def pointer released last), then CTCBase.
#include <cstdio>
#include <string.h>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CDefPointeeBase {
    long RefCount; int Destroyed;
    virtual ~CDefPointeeBase() {}
    virtual void Destroy() { ++Destroyed; }
};
struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
        Log(0x44);
    }
};
struct CTCBase { ~CTCBase() { Log(0x42); } };
template <char C> struct CMember { ~CMember() { Log(C); } };
struct CTCKrakenTentacle : CTCBase {
    CDefPointer PDef;                       // PTentacleDef +0x0c
    CMember<0x4b> PKraken;                  // K +0x10
    CMember<0x4f> PObstruction;             // O +0x18
    CMember<0x52> PRippleEffect;            // R +0x20
    CMember<0x4e> DummyName;                // N +0x48 (CCharString)
};
int main()
{
    CDefPointeeBase def; def.RefCount = 2; def.Destroyed = 0;
    {
        CTCKrakenTentacle t; t.PDef.Object = &def;
    }
    // first instance: count 2 -> 1, no destroy
    if (strcmp(g_log, "NROKDB") != 0 || def.RefCount != 1 || def.Destroyed != 0) { std::printf("004dcd2b_TEST FAIL\n"); return 1; }
    g_n = 0;
    {
        CTCKrakenTentacle t; t.PDef.Object = &def;
    }
    if (strcmp(g_log, "NROKDB") != 0 || def.RefCount != 0 || def.Destroyed != 1) { std::printf("004dcd2b_TEST FAIL\n"); return 1; }
    g_n = 0;
    {
        CTCKrakenTentacle t; t.PDef.Object = 0;            // empty def pointer: nothing released
    }
    if (strcmp(g_log, "NROKDB") != 0 || def.Destroyed != 1) { std::printf("004dcd2b_TEST FAIL\n"); return 1; }
    std::printf("004dcd2b_TEST PASS\n");
    return 0;
}
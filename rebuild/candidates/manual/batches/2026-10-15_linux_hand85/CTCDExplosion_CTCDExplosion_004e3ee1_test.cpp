// Test for 004e3ee1 CTCDExplosion::~CTCDExplosion: members destroyed in reverse declaration order (def pointer released last), then CTCBase.
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
struct CTCDExplosion : CTCBase {
    CDefPointer PDef;                       // PExplosionDef +0x38
    CMember<0x45> Emitters;                 // E +0x40
    CMember<0x48> HitThings;                // H +0x54
    CMember<0x49> ImmuneThings;             // I +0x60
    CMember<0x53> SpecifiedThingsToHit;     // S +0x6c
};
int main()
{
    CDefPointeeBase def; def.RefCount = 2; def.Destroyed = 0;
    {
        CTCDExplosion t; t.PDef.Object = &def;
    }
    // first instance: count 2 -> 1, no destroy
    if (strcmp(g_log, "SIHEDB") != 0 || def.RefCount != 1 || def.Destroyed != 0) { std::printf("004e3ee1_TEST FAIL\n"); return 1; }
    g_n = 0;
    {
        CTCDExplosion t; t.PDef.Object = &def;
    }
    if (strcmp(g_log, "SIHEDB") != 0 || def.RefCount != 0 || def.Destroyed != 1) { std::printf("004e3ee1_TEST FAIL\n"); return 1; }
    g_n = 0;
    {
        CTCDExplosion t; t.PDef.Object = 0;            // empty def pointer: nothing released
    }
    if (strcmp(g_log, "SIHEDB") != 0 || def.Destroyed != 1) { std::printf("004e3ee1_TEST FAIL\n"); return 1; }
    std::printf("004e3ee1_TEST PASS\n");
    return 0;
}

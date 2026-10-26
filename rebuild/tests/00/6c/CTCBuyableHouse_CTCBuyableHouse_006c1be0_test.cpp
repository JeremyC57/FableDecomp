// Test for 006c1be0 CTCBuyableHouse::~CTCBuyableHouse: members destroyed in reverse declaration order (def pointer released last), then CTCBase.
#include <cstdio>
#include <string.h>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
class CDefPointeeBase { public:
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
class CTCBase { public: ~CTCBase() { Log(0x42); } };
template <char C> struct CMember { ~CMember() { Log(C); } };
class CTCBuyableHouse : public CTCBase { public:
    CDefPointer PDef;                       // +0x0c
    CMember<0x4d> MainSignPost;             // M +0x24
    CMember<0x53> DressingSignPost;         // S +0x2c
    CMember<0x57> WifeLivingHere;           // W +0x38
};
int main()
{
    CDefPointeeBase def; def.RefCount = 2; def.Destroyed = 0;
    {
        CTCBuyableHouse t; t.PDef.Object = &def;
    }
    // first instance: count 2 -> 1, no destroy
    if (strcmp(g_log, "WSMDB") != 0 || def.RefCount != 1 || def.Destroyed != 0) { std::printf("006c1be0_TEST FAIL\n"); return 1; }
    g_n = 0;
    {
        CTCBuyableHouse t; t.PDef.Object = &def;
    }
    if (strcmp(g_log, "WSMDB") != 0 || def.RefCount != 0 || def.Destroyed != 1) { std::printf("006c1be0_TEST FAIL\n"); return 1; }
    g_n = 0;
    {
        CTCBuyableHouse t; t.PDef.Object = 0;            // empty def pointer: nothing released
    }
    if (strcmp(g_log, "WSMDB") != 0 || def.Destroyed != 1) { std::printf("006c1be0_TEST FAIL\n"); return 1; }
    std::printf("006c1be0_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x0045c5f6 CThingNoiseDef::~CThingNoiseDef() (compiler-generated destructor), built through
// CThingNoiseDef::Alloc as retail's def TU does: the Noise member's def chain first (~CDefPointeeBase, ~CResource), then the
// compiler-generated ~CThingBaseDef (~CThingComponentSet on Components, then ~CParentDefClassBase), then the def chain of the base.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CResource { public: virtual ~CResource() { Log('R'); } };
class CDefPointeeBase : public CResource { public: virtual ~CDefPointeeBase() { Log('D'); } };
class CDefClassBase : public CDefPointeeBase { };
class CSubDefClassBase : public CDefClassBase { };
class CTCNoiseDef : public CSubDefClassBase { public: int Type; float LifeTime; float DistanceCanBeHeardFrom; };
class CParentDefClassBase : public CDefClassBase { public: virtual ~CParentDefClassBase() { Log('P'); } };
class CThingComponentSet { public: ~CThingComponentSet() { Log('C'); } unsigned char Entries[8]; };
class CThingBaseDef : public CParentDefClassBase { public: CThingComponentSet Components; };

class CThingNoiseDef : public CThingBaseDef {
public:
    static CDefClassBase* Alloc() { return new CThingNoiseDef; }
    CTCNoiseDef Noise;
};

int main()
{
    CDefClassBase* def = CThingNoiseDef::Alloc();
    if (def == 0) {
        std::printf("0045c5f6_TEST FAIL (Alloc)\n");
        return 1;
    }
    delete def;
    if (std::strcmp(g_log, "DRCPDR") != 0) {
        std::printf("0045c5f6_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("0045c5f6_TEST PASS\n");
    return 0;
}

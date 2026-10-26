#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
class CAIBrain { public:}; class CThingAICreature { public:}; class CCharString { public:};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    virtual void Initialise(CAIBrain&, CThingAICreature&, long, long, const CCharString&) { Log("base"); }
    unsigned char _pad[0x10];
};
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char PTargetThing[8];
    virtual void Initialise(CAIBrain& b, CThingAICreature& c, long p, long sp, const CCharString& n) { CAIStateGroupBase::Initialise(b, c, p, sp, n); Init(); }
    virtual void Init() { Log("hook"); }
};
struct CDerived : CAIStateGroup_CreatureUseCoverBase { virtual void Init() { Log("derived-hook"); } };
int main()
{
    CDerived d; CAIBrain b; CThingAICreature c; CCharString n;
    static_cast<CAIStateGroupBase&>(d).Initialise(b, c, 1, 2, n);
    return Check("009705d0", "base derived-hook ");
}
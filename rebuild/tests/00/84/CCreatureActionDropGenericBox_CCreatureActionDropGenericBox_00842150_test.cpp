// behaviour test for 0x00842150 CCreatureAction_DropGenericBox::CCreatureAction_DropGenericBox(CThingCreatureBase&) (model; the base and the anim lookup are recording stubs)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CAnimationEntry { public: int id; };
static CAnimationEntry g_anim;
static char g_name[64];
static float g_a, g_b; static long g_c, g_d, g_s0, g_s1; static const void* g_owner;
class CCharString { public:
    CCharString(const char* text, long length = -1) { std::strcpy(g_name, text); (void)length; }
    ~CCharString() { }
};
class CSoundPair { public: CSoundPair() : FirstSound(0), LastSound(0) { } long FirstSound; long LastSound; };
class CThingCreatureBase { public: virtual ~CThingCreatureBase() { } };
class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound, long c, long d)
    { g_owner = &creature; g_a = a; g_b = b; g_c = c; g_d = d; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; (void)anim; }
    virtual ~CCreatureActionBase() { }
protected:
    static const CAnimationEntry& GetRandomAnimFromName(const CThingCreatureBase&, const CCharString&) { return g_anim; }
};
class CCreatureAction_DropGenericBox : public CCreatureActionBase { public: CCreatureAction_DropGenericBox(CThingCreatureBase& owner); };
CCreatureAction_DropGenericBox::CCreatureAction_DropGenericBox(CThingCreatureBase& owner)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetRandomAnimFromName(owner, "ST_HOLD_BOX_PUT_DOWN"), CSoundPair(), 0, 0)
{
}
int main()
{
    CThingCreatureBase owner;
    CCreatureAction_DropGenericBox action(owner);
    int ok = g_owner == &owner && g_a == -1.0f && g_b == 0.5f && g_c == 0 && g_d == 0 && g_s0 == 0 && g_s1 == 0 &&
             std::strcmp(g_name, "ST_HOLD_BOX_PUT_DOWN") == 0;
    if (!ok) { std::printf("00842150_TEST FAIL\n"); return 1; }
    std::printf("00842150_TEST PASS\n");
    return 0;
}
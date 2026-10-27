// behaviour test for 0x00953530 CCreatureAction_DragonHoverOutOf::CCreatureAction_DragonHoverOutOf(CThingCreatureBase&) (model; the base and the sound lookup record their arguments)
#include <cstdio>
#include <cstring>
class CThing { };
class CThingCreatureBase : public CThing { };
static char g_name[32]; static long g_code = 0; static long g_s0 = 0; static const void* g_owner;
class CCharString { public: const char* p; CCharString(const char* text, long = -1) : p(text) { } };
class CSoundPair { public: long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase& owner, long actionCode, const CSoundPair& sound) { g_owner = &owner; g_code = actionCode; g_s0 = sound.FirstSound; }
    virtual ~CCombatActionBase() { }
protected:
    static CSoundPair GetSoundPair(const CThingCreatureBase&, const CCharString& name) { std::strcpy(g_name, name.p); CSoundPair s; s.FirstSound = 7; return s; }
};
class CCreatureAction_DragonHoverOutOf : public CCombatActionBase { public: CCreatureAction_DragonHoverOutOf(CThingCreatureBase& owner); };
CCreatureAction_DragonHoverOutOf::CCreatureAction_DragonHoverOutOf(CThingCreatureBase& owner) : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")) { }
int main()
{
    CThingCreatureBase owner;
    CCreatureAction_DragonHoverOutOf action(owner);
    int ok = g_owner == &owner && g_code == 250 && g_s0 == 7 && std::strcmp(g_name, "SWORD_SWING") == 0;
    if (!ok) { std::printf("00953530_TEST FAIL\n"); return 1; }
    std::printf("00953530_TEST PASS\n");
    return 0;
}
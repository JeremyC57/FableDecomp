// behaviour test for 0x008E4AD0 CCreatureAction_DragonStrafeOutOf::CCreatureAction_DragonStrafeOutOf(CThingCreatureBase&) (model; the base and the sound lookup record their arguments)
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
class CCreatureAction_DragonStrafeOutOf : public CCombatActionBase { public: CCreatureAction_DragonStrafeOutOf(CThingCreatureBase& owner); };
CCreatureAction_DragonStrafeOutOf::CCreatureAction_DragonStrafeOutOf(CThingCreatureBase& owner) : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")) { }
int main()
{
    CThingCreatureBase owner;
    CCreatureAction_DragonStrafeOutOf action(owner);
    int ok = g_owner == &owner && g_code == 250 && g_s0 == 7 && std::strcmp(g_name, "SWORD_SWING") == 0;
    if (!ok) { std::printf("008e4ad0_TEST FAIL\n"); return 1; }
    std::printf("008e4ad0_TEST PASS\n");
    return 0;
}
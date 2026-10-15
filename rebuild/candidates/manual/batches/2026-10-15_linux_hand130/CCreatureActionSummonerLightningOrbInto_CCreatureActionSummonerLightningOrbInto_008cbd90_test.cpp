// Behaviour test for 0x008CBD90 CCreatureAction_SummonerLightningOrbInto(owner, thing).
#include <cstdio>
#include <cstring>
struct CThing { int id; };
struct CCharString { char m_text[32]; CCharString(const char* t, int) { std::strcpy(m_text, t); } };
struct CSoundPair { int m_first, m_second; CSoundPair() : m_first(0), m_second(0) { } };
struct CBaseIntelligentPointer { CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
struct CCombatActionBase {
    CThing* m_owner; CThing* m_thing; int m_code; CSoundPair m_sound; char m_name[32];
    CCombatActionBase(CThing* owner, CThing* thing, int code, const CSoundPair& sound) : m_owner(owner), m_thing(thing), m_code(code), m_sound(sound) { m_name[0] = 0; }
    void SetName(const CCharString& name) { std::strcpy(m_name, name.m_text); }
};
struct CAction : CCombatActionBase {
    CIntelligentPointer POrb;
    CAction(CThing* owner, CThing* thing) : CCombatActionBase(owner, thing, 150, CSoundPair())
    {
        { CCharString name("LIGHTNING_ORB_INTO", -1); SetName(name); }
    }
};
int main()
{
    CThing owner, thing;
    CAction a(&owner, &thing);
    if (a.m_owner != &owner || a.m_thing != &thing || a.m_code != 150 || a.m_sound.m_first != 0 || a.POrb.m_pItem != 0
        || std::strcmp(a.m_name, "LIGHTNING_ORB_INTO") != 0) { std::printf("008cbd90_TEST FAIL\n"); return 1; }
    std::printf("008cbd90_TEST PASS\n");
    return 0;
}

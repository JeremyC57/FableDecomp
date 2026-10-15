// Behaviour test for 0x00867A70 CCombatAction_GenericStrikeResponseKnockdown(owner, thingThatHit, animName).
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CCharString {
    char text[32];
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
};
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CBaseIntelligentPointer {
    const void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void SetPItem(const void* item) { m_pItem = item; }
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {
    CIntelligentPointer_CThingConst(const CThing* item) { SetPItem(item); }
};
static long g_code = 0;
struct CCombatActionBase {
    CCombatActionBase(CThingCreatureBase*, long actionCode, const CSoundPair&) { g_code = actionCode; }
};
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {
    CCharString AnimName;
    CIntelligentPointer_CThingConst PThingThatHit;
    bool GotPointOfHit, GotDirectionOfHit;
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* owner, const CThing* t, const CCharString& anim)
        : CCombatActionBase(owner, 250, CSoundPair()), AnimName(anim), PThingThatHit(t),
          GotPointOfHit(false), GotDirectionOfHit(false) { }
};
int main()
{
    CThingCreatureBase owner; CThing hitter; CCharString anim("KNOCKDOWN");
    CCombatAction_GenericStrikeResponseKnockdown a(&owner, &hitter, anim);
    if (g_code != 250 || std::strcmp(a.AnimName.text, "KNOCKDOWN") != 0 || a.PThingThatHit.m_pItem != &hitter ||
        a.GotPointOfHit || a.GotDirectionOfHit) { std::printf("00867a70_TEST FAIL\n"); return 1; }
    std::printf("00867a70_TEST PASS\n");
    return 0;
}

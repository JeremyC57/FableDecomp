// Behaviour test for 0x008BBF20 CCreatureAction_CastWillGhostSwordSpell constructor: base gets owner, code 150 and a
// silent sound pair; the spell level starts at 0 and the ghost-sword animation name is set.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { int id; };
struct CThing { int id; };

static int g_liveStrings = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; char m_anim[40];
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s)
        : m_owner(owner), m_code(code), m_sound(s) { m_anim[0] = 0; }
    void SetAnimationName(const CCharString& n) { std::strcpy(m_anim, n.m_pData); }
};
struct CCreatureAction_CastWillGhostSwordSpell : CCombatActionBase {
    long SpellLevel;
    CCreatureAction_CastWillGhostSwordSpell(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 150, CSoundPair()), SpellLevel(0)
    {
        { CCharString name("ST_CAST_SUMMON_GHOST_SWORD", -1); SetAnimationName(name); }
    }
};
int main()
{
    CThingCreatureBase owner = { 1 };
    CCreatureAction_CastWillGhostSwordSpell a(&owner);
    if (a.m_owner != &owner || a.m_code != 150 || a.m_sound.FirstSound != 0 || a.m_sound.LastSound != 0 ||
        a.SpellLevel != 0 || std::strcmp(a.m_anim, "ST_CAST_SUMMON_GHOST_SWORD") != 0 || g_liveStrings != 0) {
        std::printf("008bbf20_TEST FAIL\n"); return 1;
    }
    std::printf("008bbf20_TEST PASS\n");
    return 0;
}

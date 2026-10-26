// Behaviour test for 0x008B8110 CCreatureAction_GoUnderground constructor: base gets the owner, action code 220 and the sound pair looked up
// under (const char*)0; the animation name "GO_UNDERGROUND" is set afterwards and every CCharString temporary is destroyed.
#include <cstdio>
#include <cstring>

class CThingCreatureBase { public: int id; };

static int g_liveStrings = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
static CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    CSoundPair p;
    p.FirstSound = owner->id;
    p.LastSound = name.m_pData == 0 ? 11 : (std::strcmp(name.m_pData, "SWORD_SWING") == 0 ? 77 : -1);
    return p;
}
class CCombatActionBase { public:
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; char m_anim[32]; int m_setCalls;
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_code(actionCode), m_sound(sound), m_setCalls(0) { m_anim[0] = 0; }
    void SetAnimationName(const CCharString& name) { std::strcpy(m_anim, name.m_pData); ++m_setCalls; }
};
class CCreatureAction_GoUnderground : public CCombatActionBase { public:
    CCreatureAction_GoUnderground(CThingCreatureBase* owner) : CCombatActionBase(owner, 220, GetSoundPair(owner, (const char*)0))
    {
        { CCharString name("GO_UNDERGROUND", -1); SetAnimationName(name); }
    }
};

int main()
{
    CThingCreatureBase owner = { 6 };
    CCreatureAction_GoUnderground a(&owner);
    if (a.m_owner != &owner || a.m_code != 220 || a.m_sound.FirstSound != 6 || a.m_sound.LastSound != 11 ||
        std::strcmp(a.m_anim, "GO_UNDERGROUND") != 0 || a.m_setCalls != 1 || g_liveStrings != 0) {
        std::printf("008b8110_TEST FAIL\n"); return 1;
    }
    std::printf("008b8110_TEST PASS\n");
    return 0;
}
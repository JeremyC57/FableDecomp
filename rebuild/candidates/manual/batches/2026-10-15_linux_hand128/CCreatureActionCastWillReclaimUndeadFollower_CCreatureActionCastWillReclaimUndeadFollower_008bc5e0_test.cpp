// Behaviour test for 0x008BC5E0 CCreatureAction_CastWillReclaimUndeadFollower constructor: base gets owner, code 150
// and a silent sound pair; the follower pointer is set through SetPItem; the deliver animation name is set.
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
struct CThingGameObject { int id; };
struct CBaseIntelligentPointer {
    void* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    void SetPItem(void* p) { m_pItem = p; ++m_sets; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
};
struct CCombatActionBase {
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; char m_anim[40];
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s)
        : m_owner(owner), m_code(code), m_sound(s) { m_anim[0] = 0; }
    void SetAnimationName(const CCharString& n) { std::strcpy(m_anim, n.m_pData); }
};
struct CCreatureAction_CastWillReclaimUndeadFollower : CCombatActionBase {
    CIntelligentPointer<CThingGameObject> PUndeadFollower;
    CCreatureAction_CastWillReclaimUndeadFollower(CThingCreatureBase* owner, CThingGameObject* f)
        : CCombatActionBase(owner, 150, CSoundPair()), PUndeadFollower(f)
    {
        { CCharString name("WILL_CAST_FORCE_SPELL_DELIVER", -1); SetAnimationName(name); }
    }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CThingGameObject f = { 3 };
    CCreatureAction_CastWillReclaimUndeadFollower a(&owner, &f);
    if (a.m_owner != &owner || a.m_code != 150 || a.m_sound.LastSound != 0 || a.PUndeadFollower.m_pItem != &f ||
        a.PUndeadFollower.m_sets != 1 || std::strcmp(a.m_anim, "WILL_CAST_FORCE_SPELL_DELIVER") != 0 || g_liveStrings != 0) {
        std::printf("008bc5e0_TEST FAIL\n"); return 1;
    }
    std::printf("008bc5e0_TEST PASS\n");
    return 0;
}

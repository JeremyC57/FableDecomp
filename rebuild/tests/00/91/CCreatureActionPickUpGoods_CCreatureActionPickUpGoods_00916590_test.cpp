// Behaviour test for 0x00916590 CCreatureAction_PickUpGoods constructor: the base receives the owner, speeds -1.0/0.0, the animation looked up
// by name (loaded=true), a silent sound pair and 0/0; the PThingToPickUp pointer member holds the thing argument.
#include <cstdio>
#include <cstring>

class CThing { public: int id; };
class CCharString { public: const char* m_pData; };
class CAnimationEntry { public: const char* name; bool loaded; };
static CAnimationEntry g_entry = { 0, false };
class CThingCreatureBase { public:
    int id;
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loaded)
    { g_entry.name = name.m_pData; g_entry.loaded = loaded; return &g_entry; }
};
static CAnimationEntry* GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name)
{
    return owner->GetRandomLoadedAnimFromName(name, true);
}
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CBaseIntelligentPointer {
    void* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    void SetPItem(void* p) { m_pItem = p; ++m_sets; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }
};
class CCreatureActionBase { public:
    CThingCreatureBase* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; long m_c, m_d;
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& s, long c, long d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(s), m_c(c), m_d(d) { }
};
class CCreatureAction_PickUpGoods : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PThingToPickUp;
    CCreatureAction_PickUpGoods(CThingCreatureBase* owner, CThing* thing, const CCharString& animation)
        : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnim(owner, animation), CSoundPair(), 0, 0), PThingToPickUp(thing) { }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CThing thing = { 2 }; CCharString anim = { "ANIM_X" };
    CCreatureAction_PickUpGoods a(&owner, &thing, anim);
    if (a.m_owner != &owner || a.m_a != -1.0f || a.m_b != 0.0f || a.m_anim != &g_entry ||
        std::strcmp(g_entry.name, "ANIM_X") != 0 || !g_entry.loaded || a.m_sound.FirstSound != 0 || a.m_sound.LastSound != 0 ||
        a.m_c != 0 || a.m_d != 0 || a.PThingToPickUp.m_pItem != &thing || a.PThingToPickUp.m_sets != 1) {
        std::printf("00916590_TEST FAIL\n"); return 1;
    }
    std::printf("00916590_TEST PASS\n");
    return 0;
}
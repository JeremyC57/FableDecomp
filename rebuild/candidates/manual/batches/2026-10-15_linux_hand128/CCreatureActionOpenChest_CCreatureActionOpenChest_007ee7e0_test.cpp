// Behaviour test for 0x007EE7E0 CCreatureAction_OpenChest constructor: the base receives the owner, speeds -1.0/1.0, the animation looked up
// by name (loaded=true), a silent sound pair and 0/0; the PChest pointer member holds the thing argument.
#include <cstdio>
#include <cstring>

struct CThing { int id; };
struct CCharString { const char* m_pData; };
struct CAnimationEntry { const char* name; bool loaded; };
static CAnimationEntry g_entry = { 0, false };
struct CThingCreatureBase {
    int id;
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loaded)
    { g_entry.name = name.m_pData; g_entry.loaded = loaded; return &g_entry; }
};
static CAnimationEntry* GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name)
{
    return owner->GetRandomLoadedAnimFromName(name, true);
}
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CBaseIntelligentPointer {
    void* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    void SetPItem(void* p) { m_pItem = p; ++m_sets; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
};
struct CCreatureActionBase {
    CThingCreatureBase* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; long m_c, m_d;
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& s, long c, long d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(s), m_c(c), m_d(d) { }
};
struct CCreatureAction_OpenChest : CCreatureActionBase {
    CIntelligentPointer<CThing> PChest;
    CCreatureAction_OpenChest(CThingCreatureBase* owner, CThing* thing, const CCharString& animation)
        : CCreatureActionBase(owner, -1.0f, 1.0f, GetRandomLoadedAnim(owner, animation), CSoundPair(), 0, 0), PChest(thing) { }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CThing thing = { 2 }; CCharString anim = { "ANIM_X" };
    CCreatureAction_OpenChest a(&owner, &thing, anim);
    if (a.m_owner != &owner || a.m_a != -1.0f || a.m_b != 1.0f || a.m_anim != &g_entry ||
        std::strcmp(g_entry.name, "ANIM_X") != 0 || !g_entry.loaded || a.m_sound.FirstSound != 0 || a.m_sound.LastSound != 0 ||
        a.m_c != 0 || a.m_d != 0 || a.PChest.m_pItem != &thing || a.PChest.m_sets != 1) {
        std::printf("007ee7e0_TEST FAIL\n"); return 1;
    }
    std::printf("007ee7e0_TEST PASS\n");
    return 0;
}

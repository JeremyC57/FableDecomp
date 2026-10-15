// Behaviour test for 0x00945410 CCreatureAction_GetItemOutOfPocket constructor: ST_GET_ITEM_OUT_OF_POCKET animation, speeds -1/0.5, silent sound, item def index stored.
#include <cstdio>
#include <cstring>

struct CThing { int id; };
static int g_liveStrings = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CAnimationEntry { char name[40]; bool loaded; int liveStrings; };
static CAnimationEntry g_entry;
struct CThingCreatureBase {
    int id;
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loaded)
    { std::strcpy(g_entry.name, name.m_pData); g_entry.loaded = loaded; g_entry.liveStrings = g_liveStrings; return &g_entry; }
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
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
};
struct CCreatureActionBase {
    CThingCreatureBase* m_owner; float m_a, m_b; CAnimationEntry* m_anim; CSoundPair m_sound; long m_c, m_d;
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* anim, const CSoundPair& s, long c, long d)
        : m_owner(owner), m_a(a), m_b(b), m_anim(anim), m_sound(s), m_c(c), m_d(d) { }
};
static bool BaseOk(const CCreatureActionBase& a, CThingCreatureBase* owner, float b, const char* name)
{
    return a.m_owner == owner && a.m_a == -1.0f && a.m_b == b && a.m_anim == &g_entry && std::strcmp(g_entry.name, name) == 0 &&
        g_entry.loaded && g_entry.liveStrings == 1 && g_liveStrings == 0 && a.m_sound.FirstSound == 0 &&
        a.m_sound.LastSound == 0 && a.m_c == 0 && a.m_d == 0;
}
struct CCreatureAction_GetItemOutOfPocket : CCreatureActionBase {
    long ItemDefIndex;
    CCreatureAction_GetItemOutOfPocket(CThingCreatureBase* owner, long idx)
        : CCreatureActionBase(owner, -1.0f, 0.5f, GetRandomLoadedAnim(owner, "ST_GET_ITEM_OUT_OF_POCKET"), CSoundPair(), 0, 0),
          ItemDefIndex(idx) { }
};
int main()
{
    CThingCreatureBase owner = { 1 }; CThing thing = { 2 };
    CCreatureAction_GetItemOutOfPocket a(&owner, 42);
    if (!BaseOk(a, &owner, 0.5f, "ST_GET_ITEM_OUT_OF_POCKET") || a.ItemDefIndex != 42) { std::printf("00945410_TEST FAIL\n"); return 1; }
    (void)thing;
    std::printf("00945410_TEST PASS\n");
    return 0;
}

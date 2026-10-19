// Behaviour test for 0x0069EF80 CCreatureAction_SheatheItemToInventory(owner, name, followUp, c): runs the reconstructed
// constructor over stubbed callees; checks the base arguments, the entry copy (transition 0) and its release, and the
// counted follow-up pointer (block {1, deleter, object}, or null).
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CCharString { const char* m_pData; };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CRefCountedBase { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
static int g_mapDestroyed = 0;
struct CPointeeVirtual { virtual void v00() { } virtual void Destroy() { ++g_mapDestroyed; } };
struct CAnimComponentMap : CPointeeVirtual, CRefCountedBase {
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
static CAnimComponentMap g_map;
static long g_transition = -5;
struct CAnimationEntry {
    long BankIndex, AnimName, GroupName; CAnimComponentMap* Components;
    CAnimationEntry(long b) : BankIndex(b), AnimName(0), GroupName(0), Components(0) { }
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime)
        : BankIndex(other.BankIndex), AnimName(0), GroupName(0), Components(&g_map) { g_map.m_count = 1; g_transition = transitionInTime; }
    ~CAnimationEntry() { if (Components) { Components->ReleaseTemporary(); Components = 0; } }
};
static CAnimationEntry g_entry(77);
static const char* g_name = 0;
const CAnimationEntry& GetAnimationFromName(CThingCreatureBase*, const CCharString& name) { g_name = name.m_pData; return g_entry; }
struct CCreatureActionBase {
    CThingCreatureBase* o; float a, b; long bank; CSoundPair s; long c, d;
    virtual void v00() { }
    CCreatureActionBase(CThingCreatureBase* owner, float a_, float b_, const CAnimationEntry& an, const CSoundPair& snd, long c_, long d_)
        : o(owner), a(a_), b(b_), bank(an.BankIndex), s(snd), c(c_), d(d_) { }
};
void DeleteCreatureAction(void*) { }
struct SCountBlock {
    long m_references; void (*m_pfnDestroy)(void*); void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DeleteCreatureAction), m_pObject(object) { }
};
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object) { if (object) m_pCount = new SCountBlock(m_pObject); else m_pCount = 0; }
};
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {
    CCountedPointer<CCreatureActionBase> PFollowUpAction;
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, const CCharString& animation, CCreatureActionBase* followUp, long c);
};
CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, const CCharString& animation, CCreatureActionBase* followUp, long c)
    : CCreatureActionBase(owner, -1.0f, 0.0f, CAnimationEntry(GetAnimationFromName(owner, animation), 0), CSoundPair(), c, 0)
    , PFollowUpAction(followUp)
{
}
int main()
{
    CThingCreatureBase owner; CCharString name; name.m_pData = "SHEATHE";
    CCreatureActionBase follow(0, 0, 0, g_entry, CSoundPair(), 0, 0);
    CCreatureAction_SheatheItemToInventory a(&owner, name, &follow, 9);
    bool ok = a.o == &owner && a.a == -1.0f && a.b == 0.0f && a.bank == 77 && a.c == 9 && a.d == 0 && a.s.LastSound == 0 &&
              g_name == name.m_pData && g_transition == 0 && g_mapDestroyed == 1 &&
              a.PFollowUpAction.m_pObject == &follow && a.PFollowUpAction.m_pCount != 0 &&
              a.PFollowUpAction.m_pCount->m_references == 1 && a.PFollowUpAction.m_pCount->m_pObject == &follow &&
              a.PFollowUpAction.m_pCount->m_pfnDestroy == DeleteCreatureAction;
    CCreatureAction_SheatheItemToInventory b(&owner, name, 0, 1);
    ok = ok && b.PFollowUpAction.m_pObject == 0 && b.PFollowUpAction.m_pCount == 0 && g_mapDestroyed == 2;
    delete a.PFollowUpAction.m_pCount;
    if (!ok) { std::printf("0069ef80_TEST FAIL\n"); return 1; }
    std::printf("0069ef80_TEST PASS\n");
    return 0;
}

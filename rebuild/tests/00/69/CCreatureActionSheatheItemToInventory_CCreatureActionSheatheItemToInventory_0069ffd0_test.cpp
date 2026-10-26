// Behaviour test for 0x0069FFD0 CCreatureAction_SheatheItemToInventory(owner, followUp, c): runs the reconstructed
// constructor over stubbed callees; checks the base arguments, the entry copy (transition 4, name from GetAnimationToUse) and its release, and the
// counted follow-up pointer (block {1, deleter, object}, or null).
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
static int g_strings = 0;
class CCharString { public: const char* m_pData; CCharString(const char* t) : m_pData(t) { ++g_strings; } CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_strings; } ~CCharString() { --g_strings; } };
CCharString GetAnimationToUse(CThingCreatureBase*) { return CCharString("SHEATHE_TO_USE"); }
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CRefCountedBase { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
static int g_mapDestroyed = 0;
struct CPointeeVirtual { virtual void v00() { } virtual void Destroy() { ++g_mapDestroyed; } };
class CAnimComponentMap : public CPointeeVirtual, public CRefCountedBase { public:
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
static CAnimComponentMap g_map;
static long g_transition = -5;
class CAnimationEntry { public:
    long BankIndex, AnimName, GroupName; CAnimComponentMap* Components;
    CAnimationEntry(long b) : BankIndex(b), AnimName(0), GroupName(0), Components(0) { }
    CAnimationEntry(const CAnimationEntry& other, long transitionInTime)
        : BankIndex(other.BankIndex), AnimName(0), GroupName(0), Components(&g_map) { g_map.m_count = 1; g_transition = transitionInTime; }
    ~CAnimationEntry() { if (Components) { Components->ReleaseTemporary(); Components = 0; } }
};
static CAnimationEntry g_entry(77);
static char g_name[32];
const CAnimationEntry& GetAnimationFromName(CThingCreatureBase*, const CCharString& name) { std::strcpy(g_name, name.m_pData); return g_entry; }
class CCreatureActionBase { public:
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
template <class T> class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object) { if (object) m_pCount = new SCountBlock(m_pObject); else m_pCount = 0; }
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:
    CCountedPointer<CCreatureActionBase> PFollowUpAction;
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* followUp, long c);
};
CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* followUp, long c)
    : CCreatureActionBase(owner, -1.0f, 0.55f, CAnimationEntry(GetAnimationFromName(owner, GetAnimationToUse(owner)), 4), CSoundPair(), c, 150)
    , PFollowUpAction(followUp)
{
}
int main()
{
    CThingCreatureBase owner;
    CCreatureActionBase follow(0, 0, 0, g_entry, CSoundPair(), 0, 0);
    CCreatureAction_SheatheItemToInventory a(&owner, &follow, 9);
    bool ok = a.o == &owner && a.a == -1.0f && a.b == 0.55f && a.bank == 77 && a.c == 9 && a.d == 150 && g_strings == 0 && a.s.LastSound == 0 &&
              std::strcmp(g_name, "SHEATHE_TO_USE") == 0 && g_transition == 4 && g_mapDestroyed == 1 &&
              a.PFollowUpAction.m_pObject == &follow && a.PFollowUpAction.m_pCount != 0 &&
              a.PFollowUpAction.m_pCount->m_references == 1 && a.PFollowUpAction.m_pCount->m_pObject == &follow &&
              a.PFollowUpAction.m_pCount->m_pfnDestroy == DeleteCreatureAction;
    CCreatureAction_SheatheItemToInventory b(&owner, 0, 1);
    ok = ok && b.PFollowUpAction.m_pObject == 0 && b.PFollowUpAction.m_pCount == 0 && g_mapDestroyed == 2;
    delete a.PFollowUpAction.m_pCount;
    if (!ok) { std::printf("0069ffd0_TEST FAIL\n"); return 1; }
    std::printf("0069ffd0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) -- make `action` (its preceding action when it has one, else a clone) the creature's current action when the creature is free to perform it: the new action takes over from the old one and starts; if it is still current afterwards, update it now during a frame update and tell the unfinished old action it was replaced. A refused combat action is offered to the combat component (0x30) as the possible queued action
// Leaf: ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006644F0, length 384 bytes (`ret 4`).
//   Signature from Ego_r.pdb `?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z`; the
//   CCountedPointer overload follows at 0x00664670.
// Callees: CThingCreatureBase::IsFreeToPerformAction @ 0x00662210, CCreatureActionBase::HasPrecedingAction @ 0x00692EC0,
//   CCreatureActionBase::GetPrecedingAction(CCreatureActionBase**) @ 0x00692F30, the action's virtual slot 11 (+0x2c, its
//   counted clone), CCountedPointer::Reset(T*) @ 0x00668D90, the new action's take-over call @ 0x00669280 (manifest
//   "CFrontEndScreen::AddActionOnLeftClicked" is propagated) with the old action's member at +0x74, the action's virtual
//   slot 12 (+0x30, start) and slot 4 (+0x10, replaced), CThingCreatureBase::UpdateAction @ 0x006643C0, the action's
//   virtual slot 26 (+0x68, a combat action test), LowerBound @ 0x0040F020, CTCCombat::SetPossibleQueuedAction
//   @ 0x007332A0, the count block's destroy function pointer, operator delete @ 0x00BFE9BC.
// Layout (retail): interface bitset +0x20 (component 0x30 = bit 16 of +0x24), interface map +0x44,
//   CThingCreatureBase::PCurrentAction +0x100 (Ego_r.pdb +0x11c), CurrentlyFrameUpdating bit 0 of +0x15c (PDB +0x184);
//   CCreatureActionBase +0x61 a finished flag, +0x74 the state handed to the successor. CCountedPointer {object, block},
//   the 12-byte block {references, destroy(object) __fastcall, object}.
// Shape: the old and new actions are local counted copies (inline copy/release); the refused-action branch uses the
//   out-parameter GetInterface (bitset test + lookup in one inline bool) so the component lands in eax like retail.
struct CTCBase;

void __fastcall Destroy_CCreatureActionBase(void* object);
struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};

template<class T> struct CCountedPointer {
    T*           m_pObject;                             // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    void Reset(T* object);                              // 0x00668D90
    T* operator->() const { return m_pObject; }
    T* Get() const { return m_pObject; }
};

struct CActionState { unsigned char _body[4]; };
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OnReplaced();                          // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const;         // slot 11 (+0x2c)
    virtual void Start();                               // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25();
    virtual bool IsCombatAction() const;                // slot 26 (+0x68)
    unsigned char _pad04[0x61 - 4];
    bool          Finished;                             // +0x61
    unsigned char _pad62[0x74 - 0x62];
    CActionState  State;                                // +0x74
    bool HasPrecedingAction() const;                    // 0x00692EC0
    void GetPrecedingAction(CCreatureActionBase** out) const;   // 0x00692F30
    void TakeOver(const CActionState& state);           // 0x00669280
};

struct CTCCombat {
    bool SetPossibleQueuedAction(const CCreatureActionBase* action);   // 0x007332A0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    __forceinline bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing {
    unsigned char _pad4c[0x100 - 0x4c];
    CCountedPointer<CCreatureActionBase> PCurrentAction;   // +0x100
    unsigned char _pad108[0x15c - 0x108];
    unsigned char CurrentlyFrameUpdating : 1;           // +0x15c bit 0
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
    void UpdateAction();                                // 0x006643C0
    bool SetCurrentAction(const CCreatureActionBase& action);
};

bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action)
{
    if (IsFreeToPerformAction(&action)) {
        CCountedPointer<CCreatureActionBase> oldAction = PCurrentAction;
        if (action.HasPrecedingAction()) {
            CCreatureActionBase* preceding = 0;
            action.GetPrecedingAction(&preceding);
            PCurrentAction.Reset(preceding);
        } else {
            PCurrentAction.Reset(action.Clone());
        }
        CCountedPointer<CCreatureActionBase> newAction = PCurrentAction;
        if (oldAction.Get())
            newAction->TakeOver(oldAction->State);
        newAction->Start();
        if (newAction.Get() != PCurrentAction.Get())
            return false;
        if (CurrentlyFrameUpdating)
            UpdateAction();
        if (oldAction.Get() && !oldAction->Finished)
            oldAction->OnReplaced();
        return true;
    }
    CTCBase* combat;
    if (action.IsCombatAction() && GetInterface(0x30, &combat))
        ((CTCCombat*)combat)->SetPossibleQueuedAction(&action);
    return false;
}

void CCreatureActionBase::v00() {}
void CCreatureActionBase::v01() {}
void CCreatureActionBase::v02() {}
void CCreatureActionBase::v03() {}
void CCreatureActionBase::v05() {}
void CCreatureActionBase::v06() {}
void CCreatureActionBase::v07() {}
void CCreatureActionBase::v08() {}
void CCreatureActionBase::v09() {}
void CCreatureActionBase::v10() {}
void CCreatureActionBase::v13() {}
void CCreatureActionBase::v14() {}
void CCreatureActionBase::v15() {}
void CCreatureActionBase::v16() {}
void CCreatureActionBase::v17() {}
void CCreatureActionBase::v18() {}
void CCreatureActionBase::v19() {}
void CCreatureActionBase::v20() {}
void CCreatureActionBase::v21() {}
void CCreatureActionBase::v22() {}
void CCreatureActionBase::v23() {}
void CCreatureActionBase::v24() {}
void CCreatureActionBase::v25() {}
static int g_replaced = 0, g_started = 0, g_destroyed = 0, g_updates = 0, g_takeOvers = 0, g_queued = 0;
static void* g_lastDestroyed = 0; static const CActionState* g_takenFrom = 0; static const CCreatureActionBase* g_queuedAction = 0;
static bool g_free = true, g_combat = false, g_hijack = false; static CCreatureActionBase* g_preceding = 0;
static CThingCreatureBase* g_creature = 0;
void __fastcall Destroy_CCreatureActionBase(void* object) { ++g_destroyed; g_lastDestroyed = object; }
template<class T> void CCountedPointer<T>::Reset(T* object)
{
    if (m_pCount && --m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
    m_pCount = 0; m_pObject = object;
    if (object) { m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = Destroy_CCreatureActionBase; m_pCount->m_pObject = object; }
}
static CCreatureActionBase g_pool[8]; static int g_used = 0; static CCreatureActionBase g_other;
void CCreatureActionBase::OnReplaced() { ++g_replaced; }
CCreatureActionBase* CCreatureActionBase::Clone() const { CCreatureActionBase* c = &g_pool[g_used++]; c->Finished = false; return c; }
void CCreatureActionBase::Start() { ++g_started; if (g_hijack) g_creature->PCurrentAction.Reset(&g_other); }
bool CCreatureActionBase::IsCombatAction() const { return g_combat; }
bool CCreatureActionBase::HasPrecedingAction() const { return g_preceding != 0; }
void CCreatureActionBase::GetPrecedingAction(CCreatureActionBase** out) const { *out = g_preceding; }
void CCreatureActionBase::TakeOver(const CActionState& state) { ++g_takeOvers; g_takenFrom = &state; }
bool CTCCombat::SetPossibleQueuedAction(const CCreatureActionBase* action) { ++g_queued; g_queuedAction = action; return true; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
bool CThingCreatureBase::IsFreeToPerformAction(const CCreatureActionBase* action) { return g_free; }
void CThingCreatureBase::UpdateAction() { ++g_updates; }
#define CHECK(c) if (!(c)) { std::printf("006644f0_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static double cbuf[sizeof(CThingCreatureBase) / 8 + 1]; CThingCreatureBase& c = *(CThingCreatureBase*)cbuf; g_creature = &c; static CTCCombat combat; static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x30; entries[0].component = (CTCBase*)&combat; entries[1].type = 0x7fffffff; entries[1].component = 0;
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 1; c.m_presentBits[1] = 0x10000;
    static CCreatureActionBase a, b, p; a.Finished = false;
    c.PCurrentAction.Reset(&a); c.CurrentlyFrameUpdating = 1;
    CHECK(c.SetCurrentAction(b));                                      // clone of b replaces a
    CHECK(c.PCurrentAction.Get() == &g_pool[0] && c.PCurrentAction.m_pCount->m_references == 1);
    CHECK(g_takeOvers == 1 && g_takenFrom == &a.State && g_started == 1 && g_updates == 1 && g_replaced == 1);
    CHECK(g_destroyed == 1 && g_lastDestroyed == &a);
    g_preceding = &p; c.CurrentlyFrameUpdating = 0; g_pool[0].Finished = true;
    CHECK(c.SetCurrentAction(b));                                      // the preceding action is taken, finished old one not told
    CHECK(c.PCurrentAction.Get() == &p && g_takeOvers == 2 && g_takenFrom == &g_pool[0].State && g_updates == 1 && g_replaced == 1);
    CHECK(g_destroyed == 2 && g_lastDestroyed == &g_pool[0]);
    g_preceding = 0; g_hijack = true;
    CHECK(!c.SetCurrentAction(b));                                     // start replaced the action: refused
    CHECK(c.PCurrentAction.Get() == &g_other && c.PCurrentAction.m_pCount->m_references == 1 && g_destroyed == 4 && g_replaced == 1);
    g_hijack = false; g_free = false;
    CHECK(!c.SetCurrentAction(b) && g_queued == 0);                    // not free, not combat
    g_combat = true;
    CHECK(!c.SetCurrentAction(b) && g_queued == 1 && g_queuedAction == &b);
    c.m_presentBits[1] = 0;
    CHECK(!c.SetCurrentAction(b) && g_queued == 1);
    std::printf("006644f0_TEST PASS\n");
    return 0;
}

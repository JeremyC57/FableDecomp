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
class CTCBase;

void __fastcall Destroy_CCreatureActionBase(void* object);
struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};

template<class T> class CCountedPointer { public:
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
    void Reset_00668d90(T* object);                              // 0x00668D90
    T* operator->() const { return m_pObject; }
    T* Get() const { return m_pObject; }
};

struct CActionState { unsigned char _body[4]; };
class CCreatureActionBase { public:
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

class CTCCombat { public:
    bool SetPossibleQueuedAction(const CCreatureActionBase* action);   // 0x007332A0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CThing { public:
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

class CThingCreatureBase : public CThing { public:
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
            PCurrentAction.Reset_00668d90(preceding);
        } else {
            PCurrentAction.Reset_00668d90(action.Clone());
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
// CTCActionUseBuyOrSteal::~CTCActionUseBuyOrSteal() -- this class's vtable, leave the global update list, destroy PPerformer, release the action-use def (count at +4, virtual slot 1 at zero, pointer cleared), then the CTCBase destructor as the tail jump
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A680, length 54 bytes.
// Callees: CTCBase::RemoveFromGlobalTCs @ 0x00686DF0 (landed), CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10,
// CTCBase::~CTCBase @ 0x00686830 (manifest label "~CChunkedFileChunk" is propagated). Vtable 0x0126679C.
// Shape: the body is RemoveFromGlobalTCs(); the members then die in reverse order. CIntelligentPointer has no destructor
// of its own (the base destructor runs directly, no vtable store), and CTCActionUseBase's IMPLICIT destructor stores no
// vtable either (its out-of-line copy @ 0x004DA967 has none) -- it only runs the CDefPointer member's inline release:
// the def pointer is read into a local (ecx) so `dec [ecx+4] ; jne ; call [eax+4]` reuses it, and the clear sits inside
// the non-null arm. The derived destructor is non-virtual here so the TU emits only `??1`.
struct CThing;
struct CBaseObject;

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    ~CTCBase();                                // 0x00686830
    void RemoveFromGlobalTCs();                // 0x00686DF0
};

struct CDefPointee {                           // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+4)
    long m_refCount;                           // +0x04
};

struct CDefPointer {                           // CDefPointer<T const>
    CDefPointee* m_pDef;
    __forceinline ~CDefPointer()
    {
        CDefPointee* def = m_pDef;
        if (def) {
            if (--def->m_refCount == 0)
                def->Destroy();
            m_pDef = 0;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    ~CBaseIntelligentPointer();                // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
};

struct CTCActionUseBase : CTCBase {            // vtable 0x0123F09C
    CDefPointer PActionUseDef;                 // +0x0c
    bool        Usable;                        // +0x10
};

struct CTCActionUseBuyOrSteal : CTCActionUseBase {   // vtable 0x0126679C
    CIntelligentPointer PPerformer;            // +0x14
    bool WaitingForConfirmation;               // +0x1c
    virtual void v00();
    ~CTCActionUseBuyOrSteal();
};

CTCActionUseBuyOrSteal::~CTCActionUseBuyOrSteal()
{
    RemoveFromGlobalTCs();
}
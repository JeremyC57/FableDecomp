// CTCActionUseBed::~CTCActionUseBed() -- leave the global update list, drop both thing pointers, release the action-use def, then ~CTCBase
// Leaf: ??1CTCActionUseBed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EBDD0, length 62 bytes.
// Callees: CTCBase::RemoveFromGlobalTCs @ 0x00686DF0 (landed; `this` in ecx), CBaseIntelligentPointer::
// ~CBaseIntelligentPointer @ 0x00A01C10 (PThingUser +0x2b, then PBuilding +0x23), the def's virtual slot 1
// (`call [eax+4]`) when the release drops the count to zero, CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Own vtable 0x0123F784 = CTCActionUseBed (rtti-vtables.tsv); the constructor is 0x004DABBC.
//
// Layout = Ego_r.pdb CTCActionUseBed (byte-packed, see the constructor): the two CIntelligentPointer<CThing>
// members sit at the unaligned +0x23 / +0x2b; CTCActionUseBase owns CDefPointer<CActionUseDef>
// PActionUseDef at +0x0c.
// Shape: own vtable store, the body's RemoveFromGlobalTCs, the members in reverse order, then the
// intermediate base's IMPLICIT destructor -- it stores no vtable of its own and only runs the def
// pointer's inline release (`if (p) { if (--p->count == 0) p->Delete(); p = 0; }` through a local copy
// held in ecx) -- and the root destructor as a tail jump.
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase_Calls();                               // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
};

struct CDefPointeeBase {                                    // counted definition object
    virtual void Destroy();                                 // slot 0
    virtual void Delete();                                  // slot 1
    long m_refCount;                                        // +4
};

template<class T>
struct CDefPointer {
    T* m_pDef;
    ~CDefPointer()
    {
        T* def = m_pDef;
        if (def != 0) {
            if (--def->m_refCount == 0)
                def->Delete();
            m_pDef = 0;
        }
    }
};

struct CTCActionUseBase : CTCBase_Calls {                   // 0x14 bytes
    CDefPointer<CDefPointeeBase> PActionUseDef;             // +0x0c
    bool                         Usable;                    // +0x10
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // vtable + item
    void* m_pItem;
    virtual void v00();
    ~CBaseIntelligentPointer();                             // 0x00A01C10
};

struct CTCActionUseBed : CTCActionUseBase {                 // vtable 0x0123F784
    bool                    WaitingForFadeOut;              // +0x14
    bool                    WaitingForFadeIn;               // +0x15
    bool                    InAtomicAction;                 // +0x16
    bool                    WaitingForConfirmation;         // +0x17
    long                    PlayerNumberOfThingUsingBed;    // +0x18
    long                    Cost;                           // +0x1c
    bool                    UseableByHero;                  // +0x20
    bool                    OwnedByHero;                    // +0x21
    bool                    ForceTrespassOnceFadedOut;      // +0x22
    CBaseIntelligentPointer PBuilding;                      // +0x23
    CBaseIntelligentPointer PThingUser;                     // +0x2b
    virtual ~CTCActionUseBed();
};
#pragma pack(pop)

CTCActionUseBed::~CTCActionUseBed()
{
    RemoveFromGlobalTCs();
}

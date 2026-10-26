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
class CThing;

class CTCBase { public:                                      // engine/CTCBase.h layout (0xc bytes)
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                               // 0x00686830
public:
    void RemoveFromGlobalTCs();   // 0x00686DF0
};

class CDefPointeeBase { public:                                    // counted definition object
    virtual void Destroy();                                 // slot 0
    virtual void Delete();                                  // slot 1
    long m_refCount;                                        // +4
};

template<class T>
class CDefPointer { public:
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

class CTCActionUseBase : public CTCBase { public:                   // 0x14 bytes
    CDefPointer<CDefPointeeBase> PActionUseDef;             // +0x0c
    bool                         Usable;                    // +0x10
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CTCActionUseBed : public CTCActionUseBase { public:                 // vtable 0x0123F784
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
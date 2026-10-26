// CTCActionUseBuyOrSteal::`scalar deleting destructor'(unsigned int) -- the inlined destructor (vtable, RemoveFromGlobalTCs, ~PPerformer, the def release, ~CTCBase as a call) then operator delete when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A730, length 75 bytes.
// Leaf: ??_GCTCActionUseBuyOrSteal
// Callees: CTCBase::RemoveFromGlobalTCs @ 0x00686DF0, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10,
// CTCBase::~CTCBase @ 0x00686830, operator delete @ 0x00BFE9BC. Vtable 0x0126679C (rtti-vtables.tsv: slot 0 is this body;
// the chunk label "`vector_deleting_destructor'" is the propagated name, it is the scalar form).
// Shape: the body of 0x0072A680 inlined into `??_G`. The derived destructor is `__forceinline virtual`, the CDefPointer
// release `__forceinline`, and CTCActionUseBase carries an explicit `__forceinline` empty destructor: left implicit, VC7.1
// calls an out-of-line `??1CTCActionUseBase` from inside the forced inline. `__declspec(novtable)` on CTCActionUseBase keeps
// that explicit destructor from storing 0x0123F09C (retail has no such store; the class's real destructor is implicit,
// see 0x004DA967). The inline constructor + `new` factory pull the vtable, and with it `??_G`, into the TU.
class CThing;
class CBaseObject;

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
    virtual ~CTCBase();                                // 0x00686830
    void AddToGlobalTCs();                     // 0x00686ED0
    void RemoveFromGlobalTCs();                // 0x00686DF0
};

struct CDefPointee {                           // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+4)
    long m_refCount;                           // +0x04
};

struct CDefPointer {                           // CDefPointer<T const>
    CDefPointee* m_pDef;
    CDefPointer() : m_pDef(0) { }
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct __declspec(novtable) CTCActionUseBase : CTCBase {   // vtable 0x0123F09C (not stored in this TU)
    CDefPointer PActionUseDef;                 // +0x0c
    bool        Usable;                        // +0x10
    CTCActionUseBase(CThing& owner) : CTCBase(owner), Usable(true) { }
    __forceinline ~CTCActionUseBase() { }
};

struct CTCActionUseBuyOrSteal : CTCActionUseBase {   // vtable 0x0126679C
    CIntelligentPointer<CThing> PPerformer;            // +0x14
    bool WaitingForConfirmation;               // +0x1c
    CTCActionUseBuyOrSteal(CThing& owner)      // 0x0072A630, inline here so the vtable is emitted
        : CTCActionUseBase(owner), PPerformer(0), WaitingForConfirmation(false) { AddToGlobalTCs(); }
    __forceinline virtual ~CTCActionUseBuyOrSteal() { RemoveFromGlobalTCs(); }
};

CTCActionUseBuyOrSteal* Create_CTCActionUseBuyOrSteal(CThing& owner)
{
    return new CTCActionUseBuyOrSteal(owner);
}
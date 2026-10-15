// CTCWillEnflame::`scalar deleting destructor'(unsigned int) -- this class's vtable, ~PTarget (+0x21), ~PItemRemoved (+0x18), the PDef release (+0x10), ~CTCBase as a call, then operator delete when bit 0 of the flag is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A50F0, length 78 bytes.
// Leaf: ??_GCTCWillEnflame
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (twice), CTCBase::~CTCBase @ 0x00686830,
// operator delete @ 0x00BFE9BC. Vtable 0x0126C824 = CTCWillEnflame (rtti-vtables.tsv, slot 0 is this body); the chunk label
// "CTCActionUseBuyOrSteal::`vector_deleting_destructor'" is propagated and rejected.
// Ego_r.pdb CTCWillEnflame (0x2c bytes): EnflameActionInitialised +0x0c, Released +0x0d, PDef +0x10, SpellLevel +0x14, PItemRemoved +0x18, IsZTargetting +0x20, PTarget +0x21 (the 8-byte intelligent pointer sits UNALIGNED right after the bool).
// Shape: the inline virtual destructor has an empty body, so after the vtable store only the member destructors run, in
// reverse declaration order: the two intelligent pointers (no destructor of their own -- the base destructor is called
// directly, no vtable store), then the def pointer's inline release (pointer in a local, clear inside the non-null arm).
// The intelligent pointer classes are declared under `#pragma pack(1)` so the PDB's unaligned member offsets come out;
// the `__forceinline` destructor and the inline model constructor + `new` factory put `??_G` (with the body inlined)
// into the TU. engine/CTCBase.h is a flattened POD, so the polymorphic CTCBase is modelled locally.
struct CThing;
struct CBaseObject;

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
    ~CTCBase();                                // 0x00686830
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

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseIntelligentPointer();                 // 0x00A01B10
    ~CBaseIntelligentPointer();                // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
#pragma pack(pop)

struct CTCWillEnflame : CTCBase {   // vtable 0x0126C824
    bool                EnflameActionInitialised;   // +0x0c
    bool                Released;              // +0x0d
    CDefPointer         PDef;                  // +0x10 CDefPointer<CSpecialAbilitiesEnflameDef const>
    long                SpellLevel;            // +0x14
    CIntelligentPointer PItemRemoved;          // +0x18 CIntelligentPointer<CThing>
    bool                IsZTargetting;         // +0x20
    CIntelligentPointer PTarget;               // +0x21 CIntelligentPointer<CThing> (unaligned)
    CTCWillEnflame(CThing& owner) : CTCBase(owner) { }   // model constructor: pulls the vtable into the TU
    __forceinline virtual ~CTCWillEnflame() { }
};

CTCWillEnflame* Create_CTCWillEnflame(CThing& owner)
{
    return new CTCWillEnflame(owner);
}
// CAIFilter_ShouldStillBeWorkingInWarehouse::CAIFilter_ShouldStillBeWorkingInWarehouse(CIntelligentPointer<CThing> crate, CIntelligentPointer<CThing> warehouse, CTCVillage* village) -- builds the warehouse-work filter from two by-value intelligent pointers and the village
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944470, length 108 bytes (+ int3 padding), `ret 14h`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50,
// ::SetPItem @ 0x00A01B90, ::~CBaseIntelligentPointer @ 0x00A01C10 (the by-value arguments, callee-destroyed).
// Shape: vtable 0x01292A8C (rtti-vtables.tsv: CAIFilter_ShouldStillBeWorkingInWarehouse) then the village
// pointer at +4 (init list; loaded before the pushes), then two CIntelligentPointer members (+8 vtable
// 0x0128B0B0, +0x10 vtable 0x0123E7E0) each copy-constructed with the inline family-44 copy constructor
// (base ctor, own vtable, SetPItem(other.GetPItem())). The two 8-byte by-value pointer arguments are
// destroyed at the end through the base destructor (the derived classes declare none). The members'
// roles come from operator() at 0x009444E0 (+8 has a byte flag at +0x18 and a slot-7 test against +0x10).
struct CBaseObject;
struct CTCVillage;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

struct CIntelligentPointerCrate : CBaseIntelligentPointer {      // vtable 0x0128B0B0
    virtual void v00();
    CIntelligentPointerCrate(const CIntelligentPointerCrate& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CAIFilter_ShouldStillBeWorkingInWarehouse {
    CTCVillage*              m_pVillage;                // +0x04
    CIntelligentPointerCrate m_pCrate;                  // +0x08
    CIntelligentPointerThing m_pWarehouse;              // +0x10
    virtual bool operator()();
    CAIFilter_ShouldStillBeWorkingInWarehouse(CIntelligentPointerCrate crate, CIntelligentPointerThing warehouse, CTCVillage* village);
};

CAIFilter_ShouldStillBeWorkingInWarehouse::CAIFilter_ShouldStillBeWorkingInWarehouse(CIntelligentPointerCrate crate, CIntelligentPointerThing warehouse, CTCVillage* village)
    : m_pVillage(village)
    , m_pCrate(crate)
    , m_pWarehouse(warehouse)
{
}

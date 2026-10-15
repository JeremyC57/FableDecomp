// CTCGatherPointBase::CTCGatherPointBase(CThing* parent) -- CTCBase(parent), own vtable, empty Users vector, MaxOccupation = 1000
// Leaf: CTCGatherPointBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0AAB, length 42 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (vtable 0x0125BE0C = CTCBase; manifest label propagated).
// Vtable 0x012421E4 = CTCGatherPointBase (rtti-vtables.tsv): the chunk label "CTCDHeroSuit" is propagated
// from the byte-similar 0x004E079C. PDB CTCGatherPointBase (0x20 bytes): Users
// (std::vector<CIntelligentPointer<const CThing>>) +0x0c, MaxOccupation +0x1c; the retail vector is 12
// bytes, so MaxOccupation sits at +0x18 in the retail layout.
// Shape: /Os pass-through base argument (`push dword ptr [esp+8]`), `xor eax,eax`, the vtable store,
// the three vector zero stores (inline member constructor) and the 1000 immediate.
#pragma optimize("s", on)
struct CThing;
struct CThingConst;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

template<class T> struct CRetailVector {                    // retail Dinkumware vector: 3 pointers
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CIntelligentPointerThing {                           // CIntelligentPointer<const CThing>, 8 bytes
    void* __vftable;
    void* PData;
};

struct CTCGatherPointBase : CTCBase_Calls {
    CRetailVector<CIntelligentPointerThing> Users;          // +0x0c
    long                                    MaxOccupation;  // +0x18
    virtual void v00();                                     // own vtable 0x012421E4
    CTCGatherPointBase(CThing* parent);
};

CTCGatherPointBase::CTCGatherPointBase(CThing* parent)
    : CTCBase_Calls(parent)
    , Users()
    , MaxOccupation(1000)
{
}

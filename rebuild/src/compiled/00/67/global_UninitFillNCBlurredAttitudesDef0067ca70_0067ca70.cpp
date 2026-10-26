// _Uninit_fill_n<CBlurredAttitudesDef*> -- placement-copy `count` CBlurredAttitudesDef (0x48 bytes) from one value, return the advanced pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067CA70, length 96 bytes.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C, the CVectorMap vector copy constructor @ 0x0046EECF
// (SubDefInfoMap +0x29), the vector<EOpinionAttitudeType> copy constructor @ 0x00471F97 (Attitudes +0x3c). Vtables 0x01230CBC =
// CParentDefClassBase, 0x0125B8AC = CBlurredAttitudesDef (rtti-vtables.tsv). /Gr `__fastcall`: first in ecx, count in edx,
// the value reference and the by-value allocator on the stack (`ret 8`).
// PDB: CDefClassBase is 0x25 bytes (packed), CParentDefClassBase 0x3d (InstantiationName CDefString +0x25, SubDefInfoMap
// CVectorMap +0x29: a 12-byte retail vector, the empty comparator at +0x35 and the sorted flag at +0x36), CBlurredAttitudesDef 0x48 (Attitudes vector +0x3c).
// Shape: the inlined copy constructors under the placement-new null test; the manifest label
// "CDefClassBase::CopyConstructRange" names the helper.
#include <new>
#pragma pack(push, 1)
class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x21];                     // +0x04 .. +0x25: CResource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase(const CDefClassBase& other);     // 0x0046644C
};

struct CSubDefInfoVector {                         // retail 12-byte vector of {unsigned long, CSubDefInfo}
    void* m_begin; void* m_end; void* m_capacity;
    CSubDefInfoVector(const CSubDefInfoVector& other);   // 0x0046EECF
};

struct CKeyPairCompareLess { };                    // empty comparator (1 byte, nothing copied)

struct CSubDefInfoMap {                            // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo>
    CSubDefInfoVector   m_entries;                 // +0
    CKeyPairCompareLess m_compare;                 // +0xc
    unsigned char       m_sorted;                  // +0xd
    CSubDefInfoMap(const CSubDefInfoMap& other) : m_entries(other.m_entries), m_compare(other.m_compare), m_sorted(other.m_sorted) { }
};

class CParentDefClassBase : public CDefClassBase { public:       // vtable 0x01230CBC
    virtual void v00();
    long           InstantiationName;              // +0x25 CDefString
    CSubDefInfoMap SubDefInfoMap;                  // +0x29
    CParentDefClassBase(const CParentDefClassBase& other)
        : CDefClassBase(other), InstantiationName(other.InstantiationName), SubDefInfoMap(other.SubDefInfoMap) { }
};

struct CAttitudeVector {                           // retail 12-byte vector<EOpinionAttitudeType>
    void* m_begin; void* m_end; void* m_capacity;
    CAttitudeVector(const CAttitudeVector& other); // 0x00471F97
};

class CBlurredAttitudesDef : public CParentDefClassBase { public:   // vtable 0x0125B8AC
    virtual void v00();
    unsigned char   _pad[0x3c - 0x37];             // +0x37 .. +0x3c
    CAttitudeVector Attitudes;                     // +0x3c
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other)
        : CParentDefClassBase(other), Attitudes(other.Attitudes) { }
};
#pragma pack(pop)

struct CRetailAllocator { };                       // empty allocator passed by value (`ret 8`: value reference + allocator)

CBlurredAttitudesDef* __fastcall UninitFillN_CBlurredAttitudesDef_0067ca70(CBlurredAttitudesDef* first, unsigned long count, const CBlurredAttitudesDef& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CBlurredAttitudesDef(value);
    return first;
}
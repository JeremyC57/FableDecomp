// CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) -- copy constructor: base CDefClassBase(other), own vtable, the InstantiationName table position and the SubDefInfoMap (vector copy constructor + Dirty flag)
// Leaf: ??0CParentDefClassBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046EE98, length 55 bytes.
// Pragma: #pragma optimize("s", on) (the def module is size-optimised).
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C,
//          vector<pair<unsigned long, CDefClassBase::CSubDefInfo> > copy constructor @ 0x0046EECF.
// Vtable 0x01230CBC = CParentDefClassBase (rtti-vtables.tsv; slots 10/11/16 are the landed
// GetInstantiationName/SetInstantiationName/IsParentDef 0x004303B0/C0/D0).
// Layout (PDB, matches the bytes): the def classes are #pragma pack(1) -- CDefClassBase is 0x25
// bytes, CDefString InstantiationName sits unaligned at +0x25 and the CVectorMap SubDefInfoMap at
// +0x29. The CVectorMap itself is default-packed: a 12-byte retail vector, the empty
// CKeyPairCompareLess at +0xc and the bool Dirty at +0xd (PDB vector 16 bytes, so Dirty is +0x11
// there). The generated header engine/CParentDefClassBase.h keeps SubDefInfoMap opaque and 0x14
// bytes wide (Ego layout), so the members are modelled locally.
// Shape: the CVectorMap copy constructor is inline (`__forceinline`: the size TU would call it
// out of line otherwise) -- its vector member is copied out of line and
// its Dirty byte is copied through the map pointers (`add edi,29h` / `lea ebx,[esi+29h]`, then
// `[edi+0Dh]` -> `[ebx+0Dh]`); the empty comparator copies nothing.
#pragma optimize("s", on)   // the def module is size-optimised

#pragma pack(push, 1)
class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x21];                                            // +0x04..+0x25
    CDefClassBase(const CDefClassBase& other);                            // 0x0046644C
};
#pragma pack(pop)

class CDefString { public:
    long TablePos;
};

struct CSubDefInfoVector {                                                // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CSubDefInfoVector(const CSubDefInfoVector& other);                    // 0x0046EECF
};

struct CKeyPairCompareLess {
};

struct CSubDefInfoMap {                                                   // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>
    CSubDefInfoVector   Vector;                                           // +0x00
    CKeyPairCompareLess Compare;                                          // +0x0c
    bool                Dirty;                                            // +0x0d
    __forceinline CSubDefInfoMap(const CSubDefInfoMap& other)
        : Vector(other.Vector), Compare(other.Compare), Dirty(other.Dirty)
    {
    }
};

#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase { public:                              // vtable 0x01230CBC
    CDefString     InstantiationName;                                     // +0x25
    CSubDefInfoMap SubDefInfoMap;                                         // +0x29
    virtual void v00();
    CParentDefClassBase(const CParentDefClassBase& other);
};
#pragma pack(pop)

CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase& other)
    : CDefClassBase(other)
    , InstantiationName(other.InstantiationName)
    , SubDefInfoMap(other.SubDefInfoMap)
{
}
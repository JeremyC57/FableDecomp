// CRegionScriptDef::operator=(const CRegionScriptDef& other) -- copy a region script def: the def-class base, the instantiation name, the sub-def map, then the two population limits
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D79C0, length 63 bytes (`ret 4`, returns `this`).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the base's implicit assignment emitted out of line; the manifest calls it
// "CThingBuildingDef::operator="), the SubDefInfoMap vector's operator= @ 0x00431F90.
// Label correction: the manifest names this "CRegionScriptDef::Copy" returning void; the body returns `this`
// (`mov eax,esi`) and is the class's implicit member-wise assignment, so it is written as operator=.
// Layout: PDB names with the retail offsets. Retail CParentDefClassBase is byte-packed (CDefClassBase 0x25 bytes, CDefString
// InstantiationName +0x25, CVectorMap SubDefInfoMap +0x29 .. +0x38: vector pointers, the empty comparator byte, the sorted
// flag at +0x0d) and the derived members start at +0x3c: RandomVillagerMax +0x3c, RandomGuardMax +0x40 (the Ego_r.pdb class
// is 0x50 bytes with two more limits; retail copies only these two).
// Shape: CParentDefClassBase's implicit assignment is inlined -- the out-of-line base call, the name word (loaded before
// the map's address arithmetic, stored after), then CVectorMap's implicit assignment through its own `this`/`other`
// registers (ebp/ebx: the vector's out-of-line operator=, the empty comparator copies nothing, the sorted byte) -- and
// then the derived members in declaration order.
// Leaf: ??4CRegionScriptDef
#pragma pack(push, 1)
struct CDefClassBase {
    unsigned char _body[0x21];                          // +0x04 .. +0x24 (PDefManager, GlobalIndex, Setup bits, ...)
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    virtual void v00();
};
#pragma pack(pop)

template<class T> struct CRetailVector {                // begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& other);   // 0x00431F90 (for the sub-def records)
};

struct CKeyPairCompareLess {};

template<class K, class V> struct CVectorMap {          // retail: 16 bytes
    CRetailVector<V>    m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
};

struct CDefString { long TablePos; };
struct CSubDefInfo;

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
};
#pragma pack(pop)

struct CRegionScriptDef : CParentDefClassBase {
    long RandomVillagerMax;                             // +0x3c
    long RandomGuardMax;                                // +0x40
    CRegionScriptDef& operator=(const CRegionScriptDef& other);
};

CRegionScriptDef& CRegionScriptDef::operator=(const CRegionScriptDef& other)
{
    CParentDefClassBase::operator=(other);
    RandomVillagerMax = other.RandomVillagerMax;
    RandomGuardMax = other.RandomGuardMax;
    return *this;
}
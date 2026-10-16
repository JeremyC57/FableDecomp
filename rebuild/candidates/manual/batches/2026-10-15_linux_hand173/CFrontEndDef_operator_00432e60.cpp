// CFrontEndDef::operator=(const CFrontEndDef&) -- def copy with the whole base chain inlined: CDefClassBase (root base, PDefManager, GlobalIndex, three flag bits), CParentDefClassBase (InstantiationName, SubDefInfoMap), then the front-end fields
// Leaf: ??4CFrontEndDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00432E60, 137 bytes.
// Identity: reached through the `jmp` thunk at 0x00432E50 in slot 19 of vtable 0x01230FBC = CFrontEndDef
//   (rtti-vtables.tsv); the manifest label "operator=" (module CThingBuildingDef) is propagated.
// Callees: the CDefClassBase root base's operator= @ 0x00431F60 (copies +0x04..+0x18; manifest
//   "CTCPhysicsBase::ConstructFromParams", propagated), SubDefInfoMap's vector operator= @ 0x00431F90,
//   vector<CCharString>::operator= @ 0x00432EE9 (manifest "CCombatWheel::operator=", propagated).
// Layout (PDB names, retail offsets; retail CParentDefClassBase is byte-packed as in the landed 0x007D79C0):
//   CDefClassBase: root base +0x00..+0x1c, PDefManager +0x1c, GlobalIndex +0x20, Setup/Template/DefaultValsApplied
//   bits 0/1/2 of +0x24; CParentDefClassBase: InstantiationName +0x25, SubDefInfoMap +0x29 (vector +0x29, empty
//   comparator +0x35, sorted flag +0x36); CFrontEndDef: vAttractModeMovie +0x3c (12-byte retail vector, PDB +0x40),
//   ErrorMessageBackgroundGraphic +0x48, ButtonABigGraphic +0x4c, ButtonBBigGraphic +0x50 (PDB +0x50/+0x54/+0x58).
// Shape: this function sits in the CDefClassBase TU (next to the out-of-line copy at 0x00431F10), so the implicit
//   CDefClassBase assignment is inlined here too: the root call, two dwords, then each 1-bit field copied as
//   `x ^ ((x ^ y) & mask)`; CParentDefClassBase and CVectorMap implicit assignments follow inline.
#pragma pack(push, 1)
struct CDefRootBase {                                   // 28-byte polymorphic root of CDefClassBase
    virtual void v00();
    unsigned long m_words[6];                           // +0x04..+0x18
    CDefRootBase& operator=(const CDefRootBase& other); // 0x00431F60
};

struct CDefinitionManager;

struct CDefClassBase : CDefRootBase {
    CDefinitionManager* PDefManager;                    // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char Setup : 1;                            // +0x24 bit 0
    unsigned char Template : 1;                         // +0x24 bit 1
    unsigned char DefaultValsApplied : 1;               // +0x24 bit 2
    __forceinline CDefClassBase& operator=(const CDefClassBase& other)
    {
        CDefRootBase::operator=(other);
        PDefManager = other.PDefManager;
        GlobalIndex = other.GlobalIndex;
        Setup = other.Setup;
        Template = other.Template;
        DefaultValsApplied = other.DefaultValsApplied;
        return *this;
    }
};
#pragma pack(pop)

template<class T> struct CRetailVector {                // begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& other);
};

struct CKeyPairCompareLess {};

template<class K, class V> struct CVectorMap {          // retail: 14 bytes
    CRetailVector<V>    m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
    __forceinline CVectorMap& operator=(const CVectorMap& other)
    {
        m_vector = other.m_vector;
        m_compare = other.m_compare;
        m_sorted = other.m_sorted;
        return *this;
    }
};

struct CDefString { long TablePos; };
struct CSubDefInfo;
struct CCharString { void* m_pStringData; };

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
    __forceinline CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        CDefClassBase::operator=(other);
        InstantiationName = other.InstantiationName;
        SubDefInfoMap = other.SubDefInfoMap;
        return *this;
    }
};
#pragma pack(pop)

struct CFrontEndDef : CParentDefClassBase {
    CRetailVector<CCharString> vAttractModeMovie;       // +0x3c
    long ErrorMessageBackgroundGraphic;                 // +0x48
    long ButtonABigGraphic;                             // +0x4c
    long ButtonBBigGraphic;                             // +0x50
    CFrontEndDef& operator=(const CFrontEndDef& other);
};

CFrontEndDef& CFrontEndDef::operator=(const CFrontEndDef& other)
{
    CParentDefClassBase::operator=(other);
    vAttractModeMovie = other.vAttractModeMovie;
    ErrorMessageBackgroundGraphic = other.ErrorMessageBackgroundGraphic;
    ButtonABigGraphic = other.ButtonABigGraphic;
    ButtonBBigGraphic = other.ButtonBBigGraphic;
    return *this;
}

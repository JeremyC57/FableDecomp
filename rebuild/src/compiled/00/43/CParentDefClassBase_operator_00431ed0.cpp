// CParentDefClassBase::operator=(const CParentDefClassBase&) -- the implicit copy assignment: the def base, the instantiation name, the sub-def map
// Leaf: ??4CParentDefClassBase@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe @ 0x00431ED0, 49 bytes, `ret 4`, returns this; called first by 30+ def assignment operators
// (0x00431E89, 0x00434379, 0x0045A164 ..). Body: CDefClassBase::operator= (0x00431F10, itself compiler-generated: CResource
// copy 0x00431F60, then PDefManager/GlobalIndex and the three bitfields; catalog-labelled CThingBuildingDef::operator=),
// InstantiationName (+0x25, one dword), then SubDefInfoMap (+0x29) through its inlined implicit operator=: the STLport
// vector assignment (0x00431F90, 12-byte elements -- `imul 2AAAAAABh`) and the Dirty byte (+0xd of the map, +0x36).
// Layout: Ego_r.pdb CParentDefClassBase (pack 1 after the 37-byte CDefClassBase; PDB size 61 with a 16-byte vector,
// retail 0x39 with STLport's 12-byte one: Compare +0xc, Dirty +0xd, the map padded to 16). engine/CParentDefClassBase.h flattens the vptr and
// the base into bytes, which an implicit operator= would copy, so the classes are local.
// Shape: /O2; every operator= in the chain is compiler-generated, emitted out of line by an assignment in a helper.
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDefString {
public:
    long TablePos;
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector<T>& operator=(const vector<T>& x);            // 0x00431F90 for pair<unsigned long, CSubDefInfo>
};
}
template <class K, class V> class CKeyPairCompareLess { public:};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                        // +0x0c
    bool Dirty;                                          // +0x0d
};
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
void AssignCParentDefClassBase(CParentDefClassBase& to, const CParentDefClassBase& from)   // emission helper, not a retail function
{
    to = from;
}
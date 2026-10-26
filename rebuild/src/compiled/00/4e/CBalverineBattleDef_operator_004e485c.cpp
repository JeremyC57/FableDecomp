// CBalverineBattleDef::operator=(const CBalverineBattleDef& other) -- the compiler-generated copy assignment of a CParentDefClassBase-derived def: the base part through CParentDefClassBase::operator=, then SecondsBetweenLunges, LungeAttackDamage, TimeInTrees, returning this
// Leaf: ??4CBalverineBattleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E485C, 39 bytes, thiscall, `ret 4`, returns this.
// Identity: CBalverineBattleDef -- proven; evidence: its only caller is the 5-byte `jmp` 0x004E4857 in vtable slot 19 (Copy) of vtable
//   0x01242F74 (RTTI .?AVCBalverineBattleDef@@), i.e. `CBalverineBattleDef::Copy(const CDefClassBase* other) { *this = *(const CBalverineBattleDef*)other; }`
//   (PDB name set: ?Copy@CBalverineBattleDef@@UAEXPBVCDefClassBase@@@Z, ??4CBalverineBattleDef@@QAEAAV0@ABV0@@Z); members per Ego_r.pdb CBalverineBattleDef (PDB +64/+68/+72).
// Label: the manifest's "CParentDefClassBase::operator=" is propagated across byte-identical bodies.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0. Its body looks compiler-generated (landed 0x00431ED0 / 0x00431F10), but every
//   def assignment in retail (30+) calls it out of line, while VC7.1 inlines an implicit CParentDefClassBase::operator=
//   into the derived one (69 bytes, graded), so the bases' assignments are declared here and defined in their own rows.
// Layout: the landed CParentDefClassBase::operator= 0x00431ED0 model -- CDefClassBase and CParentDefClassBase under pack(1)
//   (37 and 0x39 bytes), the derived def at natural alignment, so its first member lands at 0x3C.
// Model: local -- no engine header for this def.
// Shape: a size-optimised def TU (retail packs these functions back to back at odd addresses, no 16-byte alignment):
//   `#pragma optimize("s", on)` gives retail's one-register member copies (/O2 rotates eax/ecx/edx, graded).
#pragma optimize("s", on)
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
    virtual void Copy(const CDefClassBase* other);       // vtable slot 19
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10 (see Callees)
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
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0 (see Callees)
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)

class CBalverineBattleDef : public CParentDefClassBase {
public:
    float              SecondsBetweenLunges;                     // +0x3c
    float              LungeAttackDamage;
    float              TimeInTrees;
    virtual void Copy(const CDefClassBase* other);       // 0x004E4857
};
void CBalverineBattleDef::Copy(const CDefClassBase* other)
{
    *this = *(const CBalverineBattleDef*)other;
}
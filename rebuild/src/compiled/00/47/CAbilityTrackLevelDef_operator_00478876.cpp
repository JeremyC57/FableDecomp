// CAbilityTrackLevelDef::operator=(const CAbilityTrackLevelDef& other) -- the compiler-generated copy assignment of a CParentDefClassBase-derived def: the base part through CParentDefClassBase::operator=, then ExperienceCost, AbilityUnlocked, Description, returning this
// Leaf: ??4CAbilityTrackLevelDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478876, 39 bytes, thiscall, `ret 4`, returns this.
// Identity: CAbilityTrackLevelDef -- proven; evidence: its only caller is the 5-byte `jmp` 0x00478871 in vtable slot 19 (Copy) of vtable
//   0x01236504 (RTTI .?AVCAbilityTrackLevelDef@@), i.e. `CAbilityTrackLevelDef::Copy(const CDefClassBase* other) { *this = *(const CAbilityTrackLevelDef*)other; }`
//   (PDB name set: ?Copy@CAbilityTrackLevelDef@@UAEXPBVCDefClassBase@@@Z, ??4CAbilityTrackLevelDef@@QAEAAV0@ABV0@@Z); members per Ego_r.pdb CAbilityTrackLevelDef (PDB +64/+68/+72).
// Label: the manifest's "operator=" is propagated across byte-identical bodies.
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
class CDefinitionManager;
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
// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,
//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0);
//   FABLE_PDCB_TYPED_MEMBERS: InstantiationName / SubDefInfoMap are typed with this row's CDefString / CVectorMap.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
enum EHeroAbility { };

class CAbilityTrackLevelDef : public CParentDefClassBase {
public:
    long               ExperienceCost;                     // +0x3c
    EHeroAbility       AbilityUnlocked;
    long               Description;
    virtual void Copy(const CDefClassBase* other);       // 0x00478871
};
void CAbilityTrackLevelDef::Copy(const CDefClassBase* other)
{
    *this = *(const CAbilityTrackLevelDef*)other;
}
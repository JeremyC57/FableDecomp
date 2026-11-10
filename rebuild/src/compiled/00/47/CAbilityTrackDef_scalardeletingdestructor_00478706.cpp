// CAbilityTrackDef::`scalar deleting destructor'(unsigned int flags) -- run ~CAbilityTrackDef, free the def when bit 0 of flags is set
// Leaf: ??_GCAbilityTrackDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478706, 28 bytes, `ret 4`.
// Identity (proven): slot 0 of the CAbilityTrackDef vtable 0x0123649C (engine/CAbilityTrackDef.h, RTTI .?AVCAbilityTrackDef@@);
//   the body calls ~CAbilityTrackDef 0x00478722 (FableWin.pdb ??1CAbilityTrackDef@@UAE@XZ) and then operator delete.
// Callees: CAbilityTrackDef::~CAbilityTrackDef 0x00478722, operator delete 0x00BFE9BE.
// Shape: VC7.1 emits the `??_G` with the class's vftable in the TU that defines the virtual destructor (retail places it right
//   before 0x00478722); the destructor here is the 0x00478722 row's source (out of line, called). Size TU.
#include "fable_compiler.h"
#pragma optimize("s", on)

class CAbilityTrackLevelDef;                     // 0x48-byte polymorphic element

namespace std {
template <class _Tp> class allocator;
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);   // 0x00478751
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    ~_Vector_base();                             // 0x00478743 (out of line)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    FABLE_FORCEINLINE ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

#include "engine/EHeroExperienceStatCategory.h"

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CAbilityTrackDef : public CParentDefClassBase {
public:
    EHeroExperienceStatCategory        ExperienceStat;        // +0x3c
    std::vector<CAbilityTrackLevelDef> Abilities;             // +0x40
    long                               MaxedOutDescription;   // +0x4c
    virtual ~CAbilityTrackDef();
};

CAbilityTrackDef::~CAbilityTrackDef()
{
}

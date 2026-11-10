// CAbilityTrackDef::~CAbilityTrackDef() -- destroy the track's Abilities vector (range destroy of the 0x48-byte level defs, then the storage), then the def base destructor as the tail jump
// Leaf: ??1CAbilityTrackDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478722, 33 bytes, thiscall; size-optimised def TU.
// Name (agentNF3, 2026-11-10): FableWin.pdb ??1CAbilityTrackDef@@UAE@XZ (virtual through CParentDefClassBase; engine/CAbilityTrackDef.h
//   FABLE_RETAIL_CABILITYTRACKDEF_MEMBERS declares it at this address); its callers are the scalar deleting destructor 0x00478706 (vtable
//   0x0123649C slot 0) and 0x0047A3BE (the default element of TransferVectorOfSubComponents<CAbilityTrackDef>). The body destroys
//   Ego_r.pdb CAbilityTrackDef::Abilities (vector<CAbilityTrackLevelDef>, PDB +68 = retail +0x40 behind the 0x3C-byte def base and the
//   ExperienceStat enum). Was landed as a free `_Dest_val_CSymbolInfo___(SymPair*)` stand-in (catalog label propagated).
// Callees: std::_Destroy<CAbilityTrackLevelDef*> 0x00478751 (__fastcall, first/last in ecx/edx; landed spelling), the Abilities
//   vector's out-of-line storage release std::_Vector_base<CAbilityTrackLevelDef>::~_Vector_base 0x00478743 (the 14-byte
//   `if (_M_start) free(_M_start)` body, identical to the landed _Vector_base<CEngineLocalDetailObjectDef> one 0x004774AB; see
//   NAME_FIXES), the def base destructor ~CParentDefClassBase 0x00430300 (tail jump).
// Model: as the landed ~CEngineLocalDetailLayerDef 0x0047747C: the STLport ~vector (`_Destroy(_M_start, _M_finish)`) inline over an
//   out-of-line ~_Vector_base, `__declspec(novtable)` (no vtable store of its own).
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

class __declspec(novtable) CAbilityTrackDef : public CParentDefClassBase {
public:
    EHeroExperienceStatCategory        ExperienceStat;        // +0x3c
    std::vector<CAbilityTrackLevelDef> Abilities;             // +0x40
    long                               MaxedOutDescription;   // +0x4c
    ~CAbilityTrackDef();
};

CAbilityTrackDef::~CAbilityTrackDef()
{
}

// NUISystem::CUIDef::operator=(const CUIDef& def) -- implicit member-wise assignment of a UI def: the parent-def base, then every member in declaration order
// Leaf: ??4CUIDef@NUISystem@@QAEAAV01@ABV01@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00631720, 1337 bytes, thiscall `ret 4`, returns this, no EH frame.
// Identity: Ego_r.pdb (0x00675050) and FableWin.pdb ??4CUIDef@NUISystem@@QAEAAV01@ABV01@@Z. CUIDef::Copy (vtable 0x01259F8C
//   slot 19, 0x00631710) is the 5-byte `jmp 0x00631720`, i.e. `*this = *static_cast<const CUIDef*>(def)` with this
//   assignment kept out of line. The catalog name "CUIDef::operator=" is right.
// Shape: the compiler-generated assignment (CUIDef declares none). It expands the implicit CParentDefClassBase::operator=
//   (0x00431ED0 is that implicit member's COMDAT): call CDefClassBase::operator= 0x00431F10, InstantiationName +0x25, then the
//   SubDefInfoMap CVectorMap assignment (vector operator= 0x00431F90, the Dirty byte +0x36 through ebx/ebp). Then every
//   CUIDef member in declaration order: containers through their STLport operator= (vector<CUIStateDef> 0x006315E0, map
//   _Rb_tree operator= 0x00631220 / 0x006312E0 / 0x00432A35, vector<long> 0x00454886, vector<unsigned long> 0x0045BC09,
//   vector<float> 0x0045EF2D), CWideString::operator= 0x0099B7D0, boxes and vectors as dword blocks, no padding bytes.
//   The copy constructor 0x006303C0 by contrast is hand-written (default bases, a subset of fields in its own order).
// Headers: engine/NUISystem_CUIDef.h (FABLE_RETAIL_CUIDEF, no _pad_ members), engine/CParentDefClassBase.h
//   (FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN: operator= left implicit), engine/CVectorMap.h (FABLE_RETAIL_CVECTORMAP_ASSIGN),
//   FABLE_RETAIL_STLP_VECTOR_ASSIGN (out-of-line STLport vector::operator=).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CUIDef.h"

namespace NUISystem {

// Slot 19 (0x00631710): copies another UI def into this one through the implicit assignment above.
void CUIDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUIDef*>(def);
}

}
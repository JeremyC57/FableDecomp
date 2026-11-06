// NUISystem::CUIMiscThingsDef::operator=(const CUIMiscThingsDef& def) -- implicit member-wise assignment of the UI misc-things def: the parent-def base chain, then every member in declaration order
// Leaf: ??4CUIMiscThingsDef@NUISystem@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004321F0, 2117 bytes, thiscall `ret 4`, returns this, no EH frame, /O2.
// Identity: Ego_r.pdb ??4CUIMiscThingsDef@NUISystem@@QAEAAV01@ABV01@@Z; CUIMiscThingsDef::Copy (vtable 0x01230EEC slot 19,
//   0x004321E0, FableWin ?Copy@CUIMiscThingsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z) is the 5-byte `jmp 0x004321F0`, i.e.
//   `*this = *static_cast<const CUIMiscThingsDef*>(def)` with the assignment kept out of line. The members are exactly
//   engine/NUISystem_CUIMiscThingsDef.h (size 0x260). The catalog label "CSpecialAbilitiesBerserkDef::operator=" is wrong
//   (that one is landed at 0x0045F854; the Berserk def is 0x11C bytes with float vectors, this one 0x260 with wide strings).
// Shape: the compiler-generated assignment (CUIMiscThingsDef declares none):
//   - the base chain expanded in place: CResource::operator= 0x00431F60 (the only base call), PDefManager +0x1c, GlobalIndex
//     +0x20, Setup/Template/DefaultValsApplied as xor/and/xor merges of +0x24 (implicit CDefClassBase assignment), then
//     InstantiationName +0x25 and SubDefInfoMap +0x29 (vector operator= 0x00431F90 through ebx/ebp, Dirty byte +0x36);
//   - then each member: CWideString::operator= 0x0099B7D0, CCharString::operator= 0x0099EFB0, the two maps through
//     _Rb_tree::operator= (0x00432A35 map<unsigned long,unsigned long> MapPaths +0x188, 0x00432B38 map<CCharString,long>
//     MiniMapGraphics +0x21c; map's own operator= is implicit and inline), scalars and C2DVectors as dword copies.
//   This TU (the def TU holding 0x00431ED0 / 0x00431F10 / 0x00431F60) expands both base assignments; the CUIDef TU
//   (0x00631720) calls 0x00431F10. Header opt-ins: FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN (engine/CParentDefClassBase.h: an
//   implicit or plain-inline PDCB assignment is called as the 0x00431ED0 COMDAT here, DIFFER 2031v2117),
//   FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN (engine/CDefClassBase.h) and FABLE_RETAIL_CRESOURCE_ASSIGN (engine/CResource.h).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN
#define FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN
#define FABLE_RETAIL_CRESOURCE_ASSIGN
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"

namespace NUISystem {

// Slot 19 (0x004321E0): copies another misc-things def into this one through the implicit assignment above.
void CUIMiscThingsDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUIMiscThingsDef*>(def);
}

}
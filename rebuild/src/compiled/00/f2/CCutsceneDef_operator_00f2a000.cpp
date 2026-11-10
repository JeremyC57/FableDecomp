// CCutsceneDef::operator=(const CCutsceneDef& other) -- the implicit member-wise assignment of a cutscene def: the parent-def base chain, then the eight cutscene string lists
// Leaf: ??4CCutsceneDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2A000, 203 bytes, thiscall `ret 4`, returns this, no EH frame, /O2 (`push esi ; push edi`).
// Split (chunk 900): NEW function start. The gap-scan row 0x00F29FF0 (219 bytes) is Copy (5-byte `jmp 0x00F2A000`), 11 int3, then
//   this function, which only Copy reaches.
// Identity: Ego_r.pdb ??4CCutsceneDef@@QAEAAV0@ABV0@@Z (both PDBs list it next to ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z); the
//   tail-jump target of CCutsceneDef::Copy (vtable 0x012FB6E0 slot 19). CCutsceneDef declares no operator=: this is the implicit
//   one, emitted out of line because Copy calls it.
// Shape (same family as the landed NUISystem::CUIMiscThingsDef::operator= 0x004321F0):
//   - the parent-def chain expanded in place: CResource::operator= 0x00431F60 (the only base call), PDefManager +0x1c, GlobalIndex
//     +0x20, Setup / Template / DefaultValsApplied as xor/and/xor merges of +0x24, InstantiationName +0x25;
//   - SubDefInfoMap +0x29 through the implicit CVectorMap assignment, called out of line here (0x006313A0, FableWin.pdb
//     ??4?$CVectorMap@KVCSubDefInfo@CDefClassBase@@...@@QAEAAV0@ABV0@@Z: vector operator= 0x00431F90 + the Dirty byte);
//   - then the eight STLport vector<CCharString>::operator= calls (0x00432EE9) in declaration order, Macro .. Answer1.
//   Header opt-ins: FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN, FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN, FABLE_RETAIL_CRESOURCE_ASSIGN
//   (the expanded base chain, as 0x004321F0), FABLE_RETAIL_STLP_VECTOR_ASSIGN (vector operator= out of line) and the new
//   FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE (engine/CVectorMap.h, chunk 900: the CVectorMap assignment declared, so it is the
//   0x006313A0 call; with the implicit one VC7.1 expands it through ebx/ebp, DIFFER 215v203; with the implicit PDCB assignment
//   instead of the forced one the whole PDCB chain is called as 0x00431ED0, 129v203). Model: engine/CCutsceneDef.h (new).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN
#define FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN
#define FABLE_RETAIL_CRESOURCE_ASSIGN
#include "engine/CCutsceneDef.h"

// Slot 19 (0x00F29FF0): copy another cutscene def into this one through the implicit assignment above.
void CCutsceneDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CCutsceneDef*>(other);
}
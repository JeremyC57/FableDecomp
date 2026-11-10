// CCutsceneDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign another cutscene def to this one (a tail jump into the implicit CCutsceneDef::operator=)
// Leaf: ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29FF0, 5 bytes: `jmp 0x00F2A000` (`this` stays in ecx and `other` in the caller's argument
//   slot, so VC7.1 turns the operator= call into a tail jump), then 11 int3 to 0x00F2A000.
// Split (chunk 900): the gap-scan row 0x00F29FF0 (219 bytes) is this 5-byte function, 11 int3, and the separate 203-byte
//   CCutsceneDef::operator= at the new start 0x00F2A000 (the same translation unit text as that row; agent897 drafted the 5 bytes).
// Identity: slot 19 (Copy, cell 0x012FB72C) of the no-RTTI CCutsceneDef vtable 0x012FB6E0 (slot 18 Transfer 0x00F2A1D0 and slot 20
//   GetSizeofClass 0x00F29D90 landed); Ego_r.pdb and FableWin.pdb ?Copy@CCutsceneDef@@UAEXPBVCDefClassBase@@@Z, listed next to
//   ??4CCutsceneDef@@QAEAAV0@ABV0@@Z (the jump target). Same shape as the landed NUISystem::CUIMiscThingsDef::Copy 0x004321E0.
// Shape: CCutsceneDef declares no operator=, so the assignment is the implicit one; it stays out of line (0x00F2A000) and the
//   call becomes the tail jump. The TU flags are 0x00F2A000's: the parent-def chain opt-ins (FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN,
//   FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN, FABLE_RETAIL_CRESOURCE_ASSIGN), FABLE_RETAIL_STLP_VECTOR_ASSIGN and
//   FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE (engine/CVectorMap.h, chunk 900). Model: engine/CCutsceneDef.h (new, chunk 900).
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

// Slot 19: copy another cutscene def into this one through the implicit assignment (0x00F2A000).
void CCutsceneDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CCutsceneDef*>(other);
}
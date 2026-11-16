// CPersistContext::TransferVector<ETutorialCategory>(const char* name, std::vector<ETutorialCategory>& value) -- persist one vector member ("GivenTutorialCategories"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: ??$TransferVector@W4ETutorialCategory@@@CPersistContext@@QAEXPBDAAV?$vector@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@@Z
// Grade: RELOCATION_MATCH 180/180 on the corrected span 0x00412E80+180 (code + embedded jump table; bounds audit) -- catalog row 224 B also held two unlisted 9-byte stubs at 0x00412F40/0x00412F50
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412E80 (the switch table follows the code), `ret 8`. Called from CUserProfileManager::SaveGameState (0x00409FD0) with the key "GivenTutorialCategories".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0122E044), GFSerialiseVectorBinaryIn<T>
//   0x00412D20 and GFSerialiseVectorBinaryOut<T> 0x0040FD40 (both `__fastcall` stream, vector), and for the reset mode
//   `value.clear()`, emitted out of line in this module (0x00411980).
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
// Port (NF40, round 4): the PDB name is the named member template CPersistContext::TransferVector<ETutorialCategory> (Ego_r.pdb /
//   FableWin.pdb ??$TransferVector@W4ETutorialCategory@@...); the row is now the explicit instantiation of engine/CPersistContext.h's
//   FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY on stlport/fable_containers.h (clear() out of line: FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/fable_nscript_forward.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVector<ETutorialCategory>(const char*, std::vector<ETutorialCategory, std::allocator<ETutorialCategory> >&);

// CPersistContext::TransferVector<CActionInputControl>(const char* name, std::vector<CActionInputControl>& value) -- persist one vector member ("AssignedInputsPassive"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: ??$TransferVector@VCActionInputControl@@@CPersistContext@@QAEXPBDAAV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412DB0 (the switch table follows the code), `ret 8`. Called from CUserProfileManager::SaveGameState (0x00409FD0) with the key "AssignedInputsPassive".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0122E010), GFSerialiseVectorBinaryIn<T>
//   0x00412C70 and GFSerialiseVectorBinaryOut<T> 0x0040FCA0 (both `__fastcall` stream, vector), and for the reset mode
//   the inline STLport `clear()` = `erase(begin(), end())`, i.e. `__copy(last, finish, first, random_access_iterator_tag(), (ptrdiff_t*)0)` (0x0040E2D0) with the tag temporary in the dead value slot.
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
// Port (NF40, round 4): the PDB name is the named member template CPersistContext::TransferVector<CActionInputControl> (Ego_r.pdb /
//   FableWin.pdb); the row is now the explicit instantiation of engine/CPersistContext.h's FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
//   on stlport/fable_containers.h and engine/CActionInputControl.h (clear() = erase(begin(), end()) inline, the __copy 0x0040E2D0 call).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CActionInputControl.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVector<CActionInputControl>(const char*, std::vector<CActionInputControl, std::allocator<CActionInputControl> >&);

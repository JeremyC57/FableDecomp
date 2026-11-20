// CPersistContext::TransferVector<float>(const char* name, std::vector<float>& value) -- persist one vector member ("ScoresVec"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: ??$TransferVector@M@CPersistContext@@QAEXPBDAAV?$vector@MV?$allocator@M@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569FE0 (the switch table follows the code), `ret 8`. Called from QuestData_PersistActivitiesAndScores (0x00560720) with the key "ScoresVec".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0124C5E0), GFSerialiseVectorBinaryIn<T>
//   0x00464D23 and GFSerialiseVectorBinaryOut<T> 0x00464CED (both `__fastcall` stream, vector), and for the reset mode
//   `value.clear()`, emitted out of line in this module (0x0049BAE0).
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
// NF113 (PRIORITY q4512): was the generated stand-in Transfer<CQuestScoreRecord> on local CPersistContext / vector models (leaf bare
//   "Transfer"). Identity: Ego_r.pdb + FableWin.pdb ??$TransferVector@M@CPersistContext@@QAEXPBDAAV?$vector@MV?$allocator@M@std@@@std@@@Z;
//   the GFSerialiseVectorBinaryIn/Out callees 0x00464D23 / 0x00464CED are the float instances (landed binder 0x00464CBF binds
//   ??$GFSerialiseVectorBinaryIn@M@@ / ...Out@M@@), and every landed caller (0x00560720 "ScoresVec", 0x0057E2F1, 0x00684610, 0x00775C60,
//   0x00F2D6E0) binds this TransferVector<float> spelling. Body: the shared header text FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
//   (the 0x00F2D6E0 / 0x006C9EE0 recipe; FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE: value.clear() is the out-of-line 0x0049BAE0), emitted by an
//   explicit instantiation.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVector<float>(const char* name, std::vector<float, std::allocator<float> >& value);

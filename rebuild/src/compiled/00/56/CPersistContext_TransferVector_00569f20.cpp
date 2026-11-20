// CPersistContext::TransferVector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>(const char* name, std::vector<EOnlineScoreboardActivity>& value) -- persist one vector member ("ActivitiesVec"): CRC-check the key, then by mode text load / text save / binary load / binary save / reset to empty
// Leaf: ??$TransferVector@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@CPersistContext@@QAEXPBDAAV?$vector@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$allocator@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569F20 (the switch table follows the code), `ret 8`. Called from CTCHeroOnlineScoreboard's serialiser 0x00560720 with the key "ActivitiesVec".
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorLoadText 0x009BB110 and TransferVectorSaveText
//   0x009BAC80 (both take the CPersistContext_TransferableVector<T> wrapper, vtable 0x0124C5CC), GFSerialiseVectorBinaryIn<T>
//   0x00569B30 and GFSerialiseVectorBinaryOut<T> 0x005665B0 (both `__fastcall` stream, vector), and for the reset mode
//   `value.clear()`, emitted out of line in this module (0x00567AC0).
// Shape: the persist mode (CPersistContext +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset) is a
//   closed enum, so the switch ends in `default: __assume(0)` and VC7.1 emits the bare `jmp [eax*4+table]` with no range
//   check; VC7.1 lays the case bodies out in source order, and retail has them 4,3,2,1,0, so the source lists them in that
//   order. The 8-byte frame is the {vtable, vector*} wrapper shared by the two text arms; the key stays in edi from the
//   CheckCRC push. CPersistContext layout from Ego_r.pdb (0x2C bytes): UseDefaults +0x14, Mode +0x18, PLoadStream +0x24,
//   PSaveStream +0x28. The element type comes from the PDB member that owns the key where one exists.
// NF113 (PRIORITY q4511): was the generated stand-in Transfer<CQuestActivityRecord> on local CPersistContext / vector models (leaf bare
//   "Transfer"). Identity: the member template ??$TransferVector@<T>@CPersistContext@@QAEXPBDAAV?$vector@<T>...@Z of Ego_r.pdb + FableWin.pdb
//   (the float instance 0x00569FE0 follows it); T = the nested CTCHeroOnlineScoreboard::EOnlineScoreboardActivity: the only caller
//   0x00560720 transfers the scoreboard's activity keys ("ActivitiesVec", map<EOnlineScoreboardActivity, float> LocalScoreboard +0x1C)
//   and already binds this spelling (RELOCATION_MATCH); Ego_r.pdb nests the enum (?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z).
//   Body: the shared header text FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY (the 0x006C9EE0 recipe; FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE:
//   value.clear() is the out-of-line 0x00567AC0), emitted by an explicit instantiation.
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY
#include "engine/CPersistContext.h"
#include "engine/CTCHeroOnlineScoreboard.h"

template void CPersistContext::TransferVector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>(const char* name,
    std::vector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, std::allocator<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity> >& value);

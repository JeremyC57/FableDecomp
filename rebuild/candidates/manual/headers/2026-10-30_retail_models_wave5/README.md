# Retail class-model migration, wave 5 (2026-10-30): 1,103 landed rows; six new retail headers; bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (181 entries / 176 rows, each with residue); HEADERS.md (evidence per slot and
member, unproven items); NEXT.md (wave 6 plan); LESSONS.md; bind_add.tsv (the 7 binding additions).

Headers (slot counts probe-matched against retail, each proven on 5+ rows before its family moved):
CBaseObject.h retail branch FABLE_RETAIL_BASE_OBJECT (vtable 0x0129A7D8, 2/2, 0x8); CScriptGameResourceObjectBase.h new
(0x0126008C, 2/2; user-written dtor because retail keeps the class's own vtable store); CScriptGameResourceObjectScriptedThingBase.h
retail branch (0x0127094C, 30/30, 0x10; IsValid() const 0x00CD23B9, SetImplementation 0x008AB960); CScriptGameResourceObjectMovieBase.h
new (0x01260EF4, 2/2; SetImplementation unproven, candidate 0x008AB970); CIsThingConscious.h new (0x012C2FE8, 3/3);
CSpawnedFunc.h / CSpawnedFunc1.h new templates (7/7, 0x3C / 0x40; the ctor is __forceinline: 238/240 rows match vs 224 with
plain inline; what retail declared is unproven); CGameScriptInterface.h gains protected GetMinWorldFrameForMessages 0x006E7510
/ GetMaxWorldFrameForMessages 0x006E7530 (Min named from PDB order).

Rows (overlapping families): CBaseObject 449, the three CScriptGameResourceObject classes 134, CSpawnedFunc/1 239,
CIsThingConscious 1, CGSI compile-failure fixes 54, CGSI filler macros 51, CGSI blocks with data members (checked by offset)
229, CGameScriptInterfaceBase 144. Agent recheck 1,103/1,103 PASS (37 MATCH); mcheck vs HEAD 964 SAME + 139 same-bytes
renames (109 leaves now the PDB UAE/UBE spelling, 16 to the same bound address, 7 $L labels, 2 others). Coordinator
independent spot regrade after cherry-pick onto 499e7dd19 (reloc_bindings.tsv merged by set-union): 156/156 PASS
(coordinator_spot_recheck.log); bindings multi-check 0.

Reverted 176 rows: mixed inlining 56 (retail inlines the implicit ScriptedThingBase ctor/dtor at most sites but calls
0x007E72A0 / 0x007E74D0 at others; the 50 callers of 0x007E72A0 are the test set for that question), float setters 8 (PDB
slot takes float, row stores long), invented GetPlayer 4 (0x00449970 is CPlayerManager::GetMainPlayer returning CPlayer&),
CBaseClass.cpp TU rows, one DirectShow row, compile failures.
Decisions queued in NAME_FIXES (naming wave): 0x006E7510 GetMinWorldFrameForMessages (72 rows), 0x00CD23B9 IsScripting ->
IsValid (130 rows), 0x00449970 GetMainPlayer, the MovieBase / ScriptedThingBase dtor and ctor spellings, ScriptedThingBase
slots QAE -> UAE. GetTextIDFromSymbol 0x006E77F0 (38 rows) is in neither PDB and stays out of the header.
Remaining CGameScriptInterface local models 301 (from 648); PDCB 67 and BC 21 remainders and CScriptBase/CLevelScriptBase
not started.

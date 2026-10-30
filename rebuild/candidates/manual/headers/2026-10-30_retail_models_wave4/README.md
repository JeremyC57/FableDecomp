# Retail class-model migration, wave 4 (2026-10-30): 1,228 landed rows onto shared retail headers, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (124, each with residue); HEADERS.md (the 11 hand-modelled def rows);
NEXT.md (wave 5 plan); LESSONS.md (11 items). No header added or changed: every row moved onto a header waves 1-3 proved.

Rows: CGameScriptInterface.h 636 (CGSI 584 via mig_cgsi.py --rename with every called slot checked against the header
spelling, logs/cgsi_sigcheck.txt; 52 after their local CBaseClass went), CBaseClass.h 591 (518 rows whose invented slot-0
names v00/v000/b0/VTableAnchor stood in for the dtor, mig_bc_v.py; 21 inline-ctor rows whose ctor never reaches the graded
bytes), CParentDefClassBase.h 41 (11 of wave 2's reverts redone by hand: base pads dropped, Ego_r members at offset - 4,
novtable def dtors, CDefString class, 3-parameter CVectorMap; 30 typed-member rows via mig_pdcb_typed.py), CDefClassBase.h 12.
Agent recheck 1,228/1,228 PASS (51 MATCH, 1,177 RELOCATION_MATCH); mcheck vs HEAD 1,213 SAME + 15 byte-identical renames
(jump-table labels, one anonymous-namespace hash, moves to already-bound dtor spellings). Coordinator independent spot
regrade after cherry-pick onto 59d8b9cdf: 114/114 PASS (coordinator_spot_recheck.log). Bindings unchanged, multi-check 0.
Two script bugs found and fixed (mig_bc_copyinit lookahead backtracking; mirror removal stripping v000 from a nested class).

Reverted 124: CGSI 82 (compile failures from the row's own types: 33 pass a local vector<T>, 26 pass an int to a header
enum, 11 local CScriptThing stand-ins, 9 clashing CCountedPointer, 3 other); BC 19 (5 in CBaseClass.cpp's own TU where
retail inlines the members, 9 whose inline ctor reaches the bytes, 2 empty models, noinline model, converting ctor, 00a10dc0
whose test harness lacks dtor stubs); PDCB 17 (6 local by design: four /O2 expanders, 0x004304A0, the PDCB ctor 0x00430370;
11 need fixes); DCB 6.
NAME_FIXES: 0x004585BD is ??1CArmourDef@@UAE@XZ (landed as CAggregate_004585bd). Open: 0x004F9CFC bound only as
??1CCardPositionsDef@@QAE@XZ (FableWin has UAE; identity of the out-of-line dtor unproven).
Remaining local models: CGameScriptInterface 700, CDefClassBase 647, CParentDefClassBase 67, CBaseClass 21; largest without a
header: CBaseObject 652, CScriptGameResourceObjectScriptedThingBase 390, CSpawnedFunc 232.

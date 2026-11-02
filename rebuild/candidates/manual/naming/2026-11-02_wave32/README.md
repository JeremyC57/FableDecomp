# Naming pass wave 32 (2026-11-02): 103 landed rows re-authored + engine/CAIStateGroupBase.h -- the CAIStateGroup base ctors, the DirectShow strmbase renderer family

Inputs: brief `naming/briefs/BRIEF_W32.md`, QUEUE_W32_input.tsv (wave 31 deferred / partial rows + c703 / c704). One Opus 5.5 agent authored the overlay
in `$S/naming/w32` (AGENT_README.md; scripts/ here; it merged chunk 705 into its index and reran its chain). Shared header edited:
engine/CAIStateGroupBase.h (opt-in FABLE_RETAIL_CAISTATEGROUPBASE; RETAIL_MARK kept; default branch unchanged). The coordinator recomposed the
overlay against the current tree (identical to ov_final, 104 files), applied the 4 RENAMES (git mv + build_candidates.ps1), BINDINGS (31 RETIRE /
25 ADD / 2 KEEP; the table equals the agent's after-table) and LABELS (29; multiple-inheritance vtables in the undname form
``Class::`vftable'{for `Base'}``), and rechecked 175 rows -- the 103 family rows, the header's landed includers and the strmbase funclets -- against
HEAD (`logs/coordinator_recheck.log`: 168 rows = 2 MATCH + 166 RELOCATION_MATCH, all PASS, plus the 10 strmbase funclets below). `bindings_multi_check.py` 0.

Families (`runs/<fam>.rows.txt`): aisg 73 (the CAIStateGroupBase opt-in; 69 generated CAIStateGroup_* ctors on it with the chunk 703 CBaseIntelligentPointer /
CIntelligentPointer<T> model; own rows ??0CAIStateGroupBase 0x008FD255, the protected ??0CAIStateGroup_UsesSeat 0x008FF1F8, 0x00900241, 0x009031BA /
0x0090310E on CAIStateGroup_EnemyCreatureBase; retired CBase_008fd255, CAIStateGroupBase_Calls, CMemberBase_00a01b10 and the other address
stand-ins; 0x00900B81 binds ??0CAIStateGroup_MinionReactToCrime), aisd 25 (derived-only CAIStateGroup ctors, base chain from RTTI), dshow 5 (the DX9
strmbase hierarchy, UNICODE: CBaseVideoRenderer ctor 0x00CA7360, CTextureRenderer 0x00A3B510 on it, CBaseRenderer ctor 0x00CA61D0 -- now two
sources -- CAMEvent ctor 0x00CB0190, ~CBaseRenderer 0x00CA6370; the vtables carry the compiler-emitted ??_7CBaseVideoRenderer / ??_7CBaseRenderer
spellings, so the chunk 704 double spelling of 0x012BF5D8..0x012BF54C is gone; the CVolume* / CEvent_* models retired).

Queue outcomes (QUEUE_W32.tsv): 10 respelled, 13 partial, 134 deferred / carried. Decisions (`DECISIONS_W32_RESOLVED.tsv`): D1 / D2 / D7 accepted; D3,
D5, D12 held and D4, D6 deferred to the naming pass after matching (owner 2026-11-02: no standing naming waves while matching runs).

Coordinator fix at landing: the 4 renames broke 10 EH-funclet sources (0x012169C0 / C8 / D3 and 0x012169F0 .. 0x01216A32) that `#include` their parent
by file path (`../src/compiled/00/ca/CVolume_CVolume00ca61d0ctor_00ca61d0.cpp` / `..._00ca6370.cpp`); the agent's recheck passed them only because its
overlay still carried the old file names, and 3 of them were in no rows file. The coordinator repointed the includes at the renamed parents and
regraded the 10 with `lane_tools/recheck.py` (funclet offsets from the sources): 10/10 RELOCATION_MATCH + PASS (committed with family dshow).


# Naming pass wave 23 (2026-11-01): 38 landed rows respelled -- the CThingSearchTools templates call filter(*p), CAmbientEmitter / CAmbientEvent, the libpk CString own rows, ~CBaseClassNonCopyable

Inputs: brief `naming/briefs/BRIEF_W23.md`, QUEUE_W23_input.tsv (20 rows: the wave 22 open rows + the three DECISIONS_W22 families), wave 22
DEFERRED.tsv. One Opus 5.5 agent authored 38 rows in `$S/naming/w23` (AGENT_README.md; scripts/ here; its own recheck `logs/recheck_final.log`
38/38). Its base predated the chunk 691 landing (no file overlap). The coordinator copied ov_final (40 files), applied the 8 RENAMES (git mv +
build_candidates.ps1), BINDINGS (44 RETIRE / 21 ADD / 13 KEEP, 0 CONFLICT; the table equals the agent's after-table plus the 13 rows chunk 691 added)
and LABELS (22), and rechecked the 38 changed rows together with the 7 chunk 691 rows against HEAD (`logs/coordinator_recheck.log`: 45/45, 2 MATCH +
43 RELOCATION_MATCH, all PASS). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): filt 29 (DECISIONS_W22 item 2: the generic CThingSearchTools templates call
`filter(*p)`; CFilter_Bind2 / Bind3 hold their filters by pointer under the PDB members F1 / F2 / F3 and forward a reference through the PDB member
template operator()(const T&); every bound CThingFilter_X::operator() takes its PDB const T&; no forwarding overload. The 10 HasTC rows, including
the three that were 3 bytes off in wave 22, and 19 rows of the same templates binding In2DRange / InMap / InRegion ...; 0x004A9DA0 re-identified as
Bind2<HasTC, InRegion>; 25 of 29 own instantiations now exact PDB symbols), emit 3 (the CAmbientEmitter ctor 0x00C44EB0 with AcquireSounds, the
CAmbientEvent ctor 0x00C61C70 -- landed under the junk CThingFilter_SolidToThing label -- and 0x00C44C50 moved to CAmbientEmitter with its member
stand-in kept), cstrown 4 (Mid / Right / GetFileNameWithoutExtension / DescribeBitset on the libpk CString, int Mid / Right / Left / AllocCopy;
0x00C0D7C0 called as TrimRight, its own row's name), bcnc 2 (0x0099A300 = `??1CBaseClassNonCopyable@@MAE@XZ` with ~CBaseClass in the same TU as
retail's BaseClass.cpp; 0x00B3B8E0 on engine/CBaseClassNonCopyable.h).

Queue outcomes (QUEUE_W23.tsv, 20 rows): 3 respelled, 2 partial (00c0dec0: four views left; 0099a300: 39 per-row callers left, 319 rows already
bind MAE), 1 hold (0x00AD3A10), 14 deferred (0x00C28690 x3, the CBankFile rows, and the per-row STLport re-models not reached). Decisions:
`DECISIONS_W23_RESOLVED.tsv`. Deferred (DEFERRED.tsv, 459 rows) -> wave 24 with the c691 queue rows: the six wrong-filter rows, 0x00C17260, 0x00C0C0C0
as CString::operator=, 0x007ACC50 / vector<long> / ~tree per-row re-models, PeekTypeList, the destructor and mega families, cg10.

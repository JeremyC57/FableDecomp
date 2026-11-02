# Naming pass wave 29 (2026-11-02): 396 landed rows respelled + 2 shared headers -- engine/CSpawnedFuncBase.h retail layout, CSpawnedFunc ??_G rows, CWorld::GetWorldSeed, zlib _z_ names, CEngineWorldMap members

Inputs: brief `naming/briefs/BRIEF_W29.md`, QUEUE_W29_input.tsv (80 rows: wave 28 deferred / partial rows plus the c698 and c699 rows appended
mid-wave), the DECISIONS_W28_RESOLVED approvals, wave 28 DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w29` (AGENT_README.md;
scripts/ here; own recheck `logs/recheck_final.log`: 697 rows = the 396 edited rows + 301 unchanged landed includers of the two edited headers).
The coordinator copied ov_final (397 sources / tests + engine/CSpawnedFuncBase.h + engine/CWorld.h), applied the 31 RENAMES (git mv +
build_candidates.ps1), BINDINGS (9 RETIRE / 2 ADD, 0 CONFLICT; the table equals the agent's after-table) and LABELS (33), added GetWorldSeed to
gen_class_headers.py's CWorld members (a regenerate emits the same member line), confirmed that every landed `#include` of either header (692) is in
the recheck set, and rechecked the 697 rows against HEAD in two parts (`logs/coordinator_recheck.log`: 697/697 = 10 MATCH + 687 RELOCATION_MATCH, all PASS). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): sfbh 346 (engine/CSpawnedFuncBase.h: the retail implicit dtor and Ego_r member types
CCharString Name / AssociatedRegionName and CCountedPointer<const NScript::CPredicateBase> PPredicate; includers define CCharString / CCountedPointer
first -- `runs/sfbh_fixes.tsv`: 221 add engine/CCountedPointer.h, 121 fable_string.h, 3 a dtor-only CCountedPointer, 1 moves its template), sfbr 14 (the
invented CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair replaced by the header's CSpawnedFuncBase), sfg 29 (CSpawnedFunc1 / CSpawnedFunc2 rows
re-authored as the compiler's own ??_G; each T proven, `runs/sfg_table.tsv`), gws 2 (+ engine/CWorld.h: ?GetWorldSeed@CWorld@@QBEPAKXZ replaces the
GetPRandomSeed stand-ins), zlib 2 (_z_inflateEnd / _z_deflateEnd, both PDBs; the invented CCompressorZlib_Retail gone), ewm 3
(CEngineWorldMap::CloseStaticMapFile, private DeleteMap, the invented CEngineWorldMapRetail gone). The 301 unchanged includers ride in the sfbh / gws
commits' recheck, not as file changes.

Queue outcomes (QUEUE_W29.tsv): 7 respelled, 5 partial, 68 deferred (the ~CScriptThing / ~CTCBase binder families, the c697-c699 destructor families
and scope 4 not reached). Decisions (`DECISIONS_W29_RESOLVED.tsv`): the 121 CActiveEntityScript<T> ??_G rows stay on their byte-identical stand-ins
(retail inlines ~CActiveEntityScriptBase where VC7.1 RTM always calls it; no flattening -- an open codegen question); the 41 entity-script ??_G rows
and CSpawnedFunc2.h go to wave 30 once their member destructors are proven; a ??_G's ctor is defined in the row's TU only where retail's came from the
same object; the header contract ratified; the two weaker sfg identities confirmed (TU placement + FableWin's instantiation).

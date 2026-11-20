# NF115 notes (2026-11-20; PRIORITY reconciliation slice 5, 31 entries; EARLY HAND-BACK)

Base: the repo tree with the nf111 landing. Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.
**Entries (31): 12 done, 0 done-already, 0 rejected, 2 blocked (port: q4495, q4497), 0 deferred, 17 open (14 not started; q3885, q4491,
q4742 partly done).** q5218 is done with an amended spelling (REJECTED.tsv records the posed form). **Rows: 24 ported (rows/;
recheck_final.log: 2 MATCH + 22 RELOCATION_MATCH, all PASS), 24 not-a-binder, 2 blocked (port: 0x0090C1A0, 0x0065F6A6), 7 not attempted
(open, need ports: 0x006CDA00, 0x0093DB10, 0x006D50C0, 0x007BB660, 0x00692960, 0x00856A20, 0x008C0FB0; reasons and the classes to move
first in STATUS).** Header opt-ins: 2 (hdr/README.md). selfcheck.log: 0 problems.

## Recheck warnings (recheck_final.log)
24/24 PASS. WARN kinds: SHARED_HEADER_AVAILABLE (75: the pre-existing local models of the in-place rows, none added) and RELOC_NAME (17:
catalog labels that are stale or demangled, e.g. the __find_if / CThingCompare_Nearest labels, PSoundManager, IS_THING_ALIVE_*,
CBaseIntelligentPointerCtor in untouched callees). No HARD / RELOC_CONFLICT line.

## 0x00659800 (q4495 follow-up)
It already bound the claim, but it also emits the ??$GetPBestThing<HasTC,Nearest,CThingAICreature> COMDAT from the header body (no null
test, filter inline): a second body under the owner's name. It now defines BESTTHING_BODY_NULLTEST + CTHINGFILTER_HASTC_OUTOFLINE, so its
COMDAT is the 0x0065F744 owner body; RELOCATION_MATCH, no binding pair changed.

Open (not started, 0 grades; done in REFS-size order, the large families were left): q3864 (0x006925A0, 14 REFS rows), the CTCCarrying
family q4469 q4470 q4471 q4472 q4473 q4984 (0x006A1060 / 0x006A1490 / 0x006A1120 / 0x006A19B0 / 0x006A11F0 / 0x006A1540: they share
rows 005c3a95 005c552c 005cb128 005cb2b0 006647f0 006648d0 ...; must move as one family), q6158 (0x00662110, 14), q6231 + q6232 (0x00669D30 /
0x00669580, the same 93 rows), q6371 + q6406 (0x0066E630, one address) with q6407 (0x0066E370, 18 rows), q6393 (0x00662A00, 24 rows).
Column trap for the next pass: for q6231 / q6232 / q6371 / q6393 / q6406 / q6407 / q6411 / q6540 the claim is the symbol after
"PRIORITY (chunk N):" in the evidence column; column 4 holds the wrong / current spellings (q6231's claim is the `AAJ` form).

## Descriptive names
None made up. Library spellings (descriptive-by-library, exact STLport 4.6 text; both PDBs are Dinkumware builds):
0x00671BE0 std::__unique_copy<ENavigatorType*, ENavigatorType*, equal_to<ENavigatorType> > (forward_iterator_tag overload);
0x0065F9E0 / 0x0065FA0A / 0x0065FA34 / 0x0065FA5E / 0x0065FA88 / 0x0065FAB2 std::find_if<list<CCountedPointer<CVillageTask*> >::iterator, CIsTask>
(FableWin.pdb has the six Dinkumware find_if<..., CIsTask> YI instances at 0x0217F756.., checked);
0x006604EB std::list<CIntelligentPointer<CThingAICreature> >::push_back.
0x006724B0 CPersistContext::TransferVector<ENavigatorType>(vector&): the one-argument form is the repo's descriptive spelling of the
retail-only unnamed transfer family (the PDBs carry only the named PBD form); unchanged, only its Leaf line was completed.

## Leaf-line-only rows
0x006724B0, 0x00661D20, 0x006627F0 already emitted the claimed symbol; their `// Leaf:` lines were partial (what the PRIORITY table read as
"self leaf differs"). The lines are completed (rows/ + STATUS). They are NOT in RENAMES section 1: the emitted symbol does not change, and
the gate scan would otherwise treat them as renamed targets (old leaf = the partial Leaf text).

## Pre-existing splits (reduced, none created)
Strict reading of the no-split rule: in these entries the self row moves to the claim while REFS rows that bound OTHER wrong spellings
stay blocked. Spellings per address go down, none is added; the coordinator can hold the self rows if the rule is read strictly.
- q4495 0x0065F744: was 3 spellings (self CThingSearchToolsMethods stand-in, the claim in 0x00659800, the non-template stand-in in
  0x0090C1A0) -> 2. 0x0090C1A0 blocked (STATUS: form 2 matched only by defining the real COMDAT with the fake body; not handed back).
- q4491 0x006604EB: self stand-in ListWrapper removed; 0x006D50C0 (CAICreaturePointerList) and 0x007BB660 (CResultList) still bind
  stand-ins. Also 0x005151D0 (list insert): its only caller (this row) now binds that row's own STLport spelling (the stand-in
  InsertBefore spelling is gone).
- q3885 0x006627F0 (no rename): 00693030 / 008579d0 moved QAE -> QBE; 00692960, 00856a20, 008c0fb0 left.
- q4742 0x00661D20 (no rename), q4497 0x0065F152 (no rename): nothing changed for the blocked binders.
- 0x0049C770 PeekClassList: 0x0065F744 now binds the catalog / header spelling (was the CThingSearchToolsMethods::PeekAllThings stand-in).

## Fold checks
- 0x00662010 GetProperty: not a fold. The same body as the virtual CThingAICreature::GetProperties sits apart at 0x00830140 (retail kept
  both), so the GetCurrentStateGroupType / GetLifeState / GetVillagerState spellings were guesses for this one identity.
- 0x0065F744: T = CThingAICreature (mask 0x1F at 0x0065F76F), not the CThingCreatureBase instance both PDBs also list.

## Checks
- Callers (exe xref, tools/xdis.py x): 0x00671BE0 only from 0x0067178A (inside CThingCreatureDef::Transfer, not landed); 0x0065F9E0 only
  from 0x00660B38; 0x006A9960 only from the vtable (.rdata 0x0124580C); 0x006861C0 none; 0x005151D0 only from 0x006604FC.
- EH funclets: no landed .cpp under rebuild/src or rebuild/tests includes any of the 24 changed row files (include scan at hand-back).
- Every FABLE_RETAIL_* macro in rows/ resolves in rebuild/include or hdr/ (six inert defines copied from 0x00659800's preamble were dropped
  from 0x0065F744, which was regraded).
- No row in rows/ adds a local model with a base list or a libpk/ model (selfcheck 0; the edits to 0x006A9960, 0x006861C0, 0x008648A0,
  0x00864B30, 0x00693030, 0x008579D0 are in-place respellings inside the landed local models).

## Flags lines
None new. 0x00671BE0 carries `// Flags: /Gr`, which its build entry already has (CompilerFlags '/O2 /Oy /W3 /Gr').
The blocked draft work/0065f6a6_blocked.cpp would need `// Flags: /Gr` (not handed back).

## Claimed extra rows
00864b30, 00937d20, 00659800 (ported); 008579d0 (lock said `released`; set to NF115, ported); read-only claims (not changed): 0065f6a6, 0065f9ab,
006aa2e0, 006ea770, 00684610, 00683c10, 0065b854, 00716de0, 00747330, 008c3e90. 009378f0's lock says `released` (read only).

## Rule slips
- One g++ behaviour-test binary was built at /tmp/t_ff (outside the scratchpad) and deleted right after; later test builds use work/.
- Early hand-back: seven REFS rows were not attempted (STATUS `not attempted`, entries q3885 / q4491 / q4742 left `open`, partly
  done, as NF111's q4333 precedent) instead of reaching a terminal row state.

## Gate scan (work/gate/)
`$S/w22/nftools/gate.py scan` over this hand-back (myrows.txt = the 24 rows): 25 targets, 112 other landed rows referencing them
(work/gate/gate_rows.txt). The three leaf-line-only rows are not targets (no section-1 line). The pairs/eval step is the coordinator's.
- An empty `D:\Documents\FableTLC/rebuild/build/landverify` tree appeared in work/ (a tool's default work path resolved against a work/ cwd); removed. Nothing was written under /home/user/FableDecomp.

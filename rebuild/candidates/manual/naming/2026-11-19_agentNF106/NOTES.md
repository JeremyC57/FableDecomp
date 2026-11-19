# NF106 notes (round 10, 2026-11-19): resume of NF103's search-tools ports (q11280 / q11283; q11281)

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries 3: blocked: port 2 (q11280, q11283 = same claim; 16 binders ported, 40 rows left); open 1 (q11281, not started);
  done 0, done-already 0, rejected 0, deferred 0 (HELD.txt empty).
- Rows in STATUS.tsv 58 (rows.txt): ported 16 (rows/: 16 sources, no test changed); not-a-binder 2 (self rows 0x0049C6F0 / 0x00436500,
  already the PDB spelling, NF94); blocked 40 (5 tried: 0x00969FB0 / 0x0095DB90 after 3 forms, 0x009147E0 / 0x00942390 / 0x007512E0
  after 1 form; 35 not attempted, 0 grades).
- recheck_final.log: one vland --recheck-landed run over the 16 changed rows: 16 RELOCATION_MATCH + PASS. selfcheck: 0 problems.
- Header opt-ins: 0 (no overlay; hdr/README.md).

## What changed (all explicit instantiations of the shared engine/CThingSearchTools.h templates; generator t/gen.py + t/spec*.py)
1. Vector instances of GetAllThingsInRoughArea (family K016), define set of the landed 0x0095C7C0 / 0x00959380 (ORIGINAL_ROUGHAREA +
   ROUGHAREA_THISEND + ROUGHAREA_CELL_BYTEOFFSET, STLP_VECTOR_OVERFLOW_OUTOFLINE): 0x007197C0, 0x006B2720, 0x0074D710, 0x008AD710
   (CThingAICreature with the 0x00830350 include set). All first form.
2. std::list instances, define set of the landed 0x0085B2A0 / 0x006FE7C0 (ORIGINAL_ROUGHAREA + ROUGHAREA_EGOD + PEEKCELL_INLINE):
   0x0079BF30, 0x007C3C20, 0x0078DB20, 0x00785DD0 (+ CTHINGFILTER_OVERLAPPINGSPHERE_OUTOFLINE, second form), 0x0091A750
   (+ CTHINGFILTER_HASTC_OUTOFLINE), 0x00792E50, 0x007930B0, 0x007A5190, 0x007A63D0, 0x0091ADB0, 0x0080E8E0. Filters without a shared
   header (CThingFilter_CanBeHitByEpicSpell, CThingFilter_ObjectHasProperty, CThingFilter_IsWithinArea) keep the landed row's own
   declaration verbatim (marked "Kept from the landed row"); CFilter_Bind2 / Bind3 and every filter with a header come from the header.
3. 0x0091BA40 (a CThingSearchTools_Calls row): GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<CThingFilter_IsPerceivingThing,
   CThingFilter_IsNot>, std::list<CTCPerceiveThing*> > (Ego_r.pdb 0x0093F0D0 + FableWin.pdb 0x02BD4990; body: interface 52, the two
   filters in Bind2 order, list push_back) on 0x00518340's TCSINROUGHAREA_EGOD define set + CTCPERCEIVETHING_SMELL / _AWAREOFTHING +
   CTHINGFILTER_ISPERCEIVINGTHING. First form. Self leaf moves off the stand-in; catalog name CThingSearchTools_Calls::GetAllTCsInRoughArea
   becomes CThingSearchTools::GetAllTCsInRoughArea<...> (RENAMES s1: its file slug CThingSearchToolsCalls_ moves).
- Every changed row now binds ?GetAllThingMapsInArea@CThingSearchTools@@IBE... (0x0049C6F0) and
  ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (0x0049C700); no other callee spelling changed (work/fin_diff.txt,
  before = work/b0_pairs.json from a baseline recheck of all 56 rows, after = work/fin_pairs.json).
- Self leafs: 14 change (13 QAE -> the PDB QBE spelling, 0x0091BA40 stand-in -> PDB); 0x00792E50 / 0x0091A750 were already PDB.
  Gate check: no landed row binds any of the 14 old leafs (reloc_bindings witness scan, work/rb56.txt: the QAE leafs are unique per
  instance; 0x0091BA40 has no landed caller in the exe scan, work/calls_refs.txt). Six of the new leafs are what the landed callers
  already bind (0x006B1F30, 0x00715A70, 0x0074A5D0, 0x008A86C0, 0x007C36A0, 0x0091B010): existing splits narrowed, none created.

## Rows left (40, work/q11280_rows_left.txt) and what they need
- GetPBestThingInRoughArea (6: 0x0079A050, 0x007F5D20, 0x009147E0, 0x00937690, 0x0093CDB0, 0x00942390): the header's BESTROUGH_BODY
  is not their shape (0x009147E0 DIFFER 558v567, work/try/009147e0.cpp); several local filters take `const CThing*` while the shared
  CFilter_Bind2 passes a reference (0x00942390 SRC_FAIL). 0x0093CDB0's local T (CThingMask10007) has no PDB instance name.
- GetPFirstThingInRoughArea (8: 0x007313B0, 0x007512E0, 0x0079A280, 0x007FCEF0, 0x007FD110, 0x007FE730, 0x0096F260, 0x00987430):
  FIRSTROUGH_BODY differs (0x007512E0 tried; NF59 found the same at 0x007FCEF0: two frame slots, one SIB order). Next step: a FITTED
  first-rough body opt-in in CThingSearchTools.h (CELL_BYTEOFFSET-style cell expression + the guarded y loop of the local models), or
  the 0x009871C0 EGOD body moved into the header. 0x0096F260 / 0x00987430 have no PDB instance for their local spelling.
- Vector/list leftovers: 0x009595C0 / 0x00937080 (T = CThingDeadCreature: engine/CThingDeadCreature.h has no StatGetTypeFlags opt-in yet).
- Calls rows (24): identities from their filter callees (work/calls_callees.txt, from the baseline work/b0_pairs.json): several use anonymous-namespace filters (0x0080E670, 0x00809A70, 0x0080A550, 0x007F4500, 0x0078971D), CIntelligentPointer
  containers (0x0078954D, 0x008786E0) or a GetPBest body (0x007F70D0, 0x007F7340, 0x00937420). 0x00969FB0 / 0x0095DB90 are blocked
  after 3 forms, but only one compiled: ROUGHAREABODY WITHOUT STLP_VECTOR_OVERFLOW_OUTOFLINE (752v602; retail calls 0x00436AC0 out of
  line). The K016 vector define set that matched 0x0074D710 was never tried on them: it is the first form for them and for every Calls
  row whose baseline pairs show an `_M_insert_overflow@ThingVector` callee (vector instances: 0x0077ADF0, 0x007A9300, 0x007A9580,
  0x007F4500, 0x007F9C00, 0x007FDB10, 0x00809A70, 0x0080A550, 0x00851490, 0x008786E0, 0x008E4100, 0x008E60D0, 0x00905D10, 0x009793D0).
  The per-row callee survey is work/calls_callees.txt. None has a landed caller that binds its stand-in leaf (exe scan,
  work/calls_refs.txt), so a port creates no split.
- q11281: 0x0078954D / 0x0078971D (also q11280 rows) call GetMapwhoArea 0x00436500 out of line; not started.

## EH funclets
- None: no file in rebuild/src/compiled #includes any of the 16 changed sources (grep of their file names / addresses; the only hit,
  0x006B1F30, names 0x006B2720 in a comment-level stand-in, not an include). 0x0091BA40's file moves with its catalog rename (RENAMES s1).
- Gate by symbol: none of the 14 old self leafs is bound by any row in rebuild/oracles/reloc_bindings.tsv (exact-symbol grep, 0 hits).
- Kept header-less filters: `grep -rE '(class|struct) <name>' rebuild/include` finds no definition of CThingFilter_CanBeHitByEpicSpell,
  CThingFilter_ObjectHasProperty or CThingFilter_IsWithinArea in any header (multi-class headers included).

## Locks
- All 58 rows were already locked as NF106 (rows.txt); no extra row claimed. No row of UNMATCHED_ROWS / Account B touched;
  0x0063F4B0 untouched; no header changed (its compile is unchanged).

## Descriptive names
- None added.

## Rule slips
- Several read-only orientation calls ran without the env line; the first t/pairs.py run failed for want of FABLE_OBJDUMP (no grade).
- One `sleep 60` call was refused by the harness (nothing ran).
- 0x009147E0 / 0x00942390 / 0x007512E0 were tried once each in work/try/ and then left (budget); they are recorded blocked with the
  exact result, not with 3 failed forms.

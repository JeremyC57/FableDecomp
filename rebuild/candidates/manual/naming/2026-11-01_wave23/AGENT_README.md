# Naming pass wave 23 (2026-11-01): 38 landed rows respelled, all byte-identical (38/38: 37 RELOCATION_MATCH + 1 MATCH, all PASS)

Inputs: brief naming/briefs/BRIEF_W23.md, QUEUE_W23_input.tsv (20 rows: 520 x3, w20 2, c689 4, w21 1, c690 7, w22 3; no c691 rows appeared
during the wave), DEFERRED_W22_INPUT.tsv (461).
Index: idx/full.jsonl = HEAD (wave 22 idx/after.jsonl with the 7 held-out hastc rows and the renamed 0x00C44F80 re-indexed from the repo,
idx/delta_hastc.jsonl; re-checked against HEAD before hand-back: no later change under rebuild/src|include|tests, build_candidates.ps1,
reloc_bindings.tsv or the catalog, no labels_chunk691), idx/after.jsonl = HEAD + ov_final.
Commands (from $S/naming/w23, lane env sourced): `python3 scripts/compose23.py ov_final` (stages on fresh repo copies; runs/<fam>.rows.txt and
ov_<fam>/), `scripts/ovrows.py ov_final > runs/final.rows.txt`, `bash scripts/recheck.sh final ov_final $PWD/runs/final.rows.txt 3`,
`scripts/mkindex.py $PWD/idx/ovfinal.jsonl $PWD/ov_final runs/final.rows.txt` (merged into idx/after.jsonl), then qscan23 (base + final) ->
mkq23 (+ mkq23_notes) -> mkdef23 -> mkbind23 (runs/rk_final.pairs.jsonl idx/after.jsonl runs/final.rows.txt) -> mkren23 idx/after.jsonl -> mklabels23.

Families (commit order; runs/<fam>.rows.txt, cumulative ov_<fam>/):
- filt 29 (scripts/fam_filt23.py via hand23/h23_filt.py; DECISIONS_W22_RESOLVED item 2): the CThingSearchTools templates call `filter(*p)`
  (`filter(*static_cast<T*>(thing))` in the RoughArea forms), CFilter_Bind2/3 hold their filters by pointer (Ego_r.pdb members F1/F2/F3) and
  forward a reference through the member template operator()(const T&) (FableWin.pdb ??$?RV<T>@@@?$CFilter_Bind2@...@@QBE_NAB...), every bound
  CThingFilter_X::operator() takes its PDB `const T&` (CThing / CThingObject / CThingAICreature ...); namespace stand-ins N_<addr> become the global
  class. The 10 HasTC rows of the decision plus 19 rows of the same templates (In2DRange, InMap, InRegion, IsInQuest, HasScriptName, ...). No
  forwarding overload. 0x004A9DA0 re-identified Bind2<HasTC, InRegion> (callee 0x006A7960; FableWin instantiates it). 25 of the 29 own
  instantiations are exact PDB symbols once the scope is CThingSearchTools (DECISIONS_W23 item 3).
  Held out (DEFERRED, 42 rows): six rows whose filter classes do not match their callee addresses (0x006156E0 0x006D3FB0 0x00777000 0x007EE660
  0x009223A0 0x0093F930), four compile failures (0x0090E090 0x0093CDB0 0x00942390 0x0096F260), 0x004A9EF0, and rows of other shapes.
- emit 3 (hand23/h23_emit.py; item 1): ctor 0x00C44EB0 = ??0CAmbientEmitter@@QAE@PAUCResourceWorld@@PAX@Z (Init2 -> the landed AcquireSounds),
  0x00C61C70 = ??0CAmbientEvent@@QAE@PAX_N@Z (was the junk CThingFilter_SolidToThing), 0x00C44C50 on CAmbientEmitter with its stand-in member name
  (DECISIONS_W23 item 1). Test of 0x00C61C70 respelled (standalone model).
- cstrown 4 (hand23/h23_cstrown.py; item 3): Mid / Right (int) / GetFileNameWithoutExtension / DescribeBitset_00c0ad10 on the library CString with
  the own rows' g_pfnAssertHook / _EmptyString / CDebugBreak / LIB_ASSERT; callees AllocCopy(int), Left(int), ReverseFind, TrimLeft, TrimRight
  (0x00C0D7C0's own row; 0x00C0E360 had called it Remove), SafeStrlen, operator<<.
- bcnc 2 (hand23/h23_bcnc.py): 0x0099A300 = ??1CBaseClassNonCopyable@@MAE@XZ (PDB protected virtual; the queue's UAE was a hand claim) with
  ~CBaseClass in the same TU as retail's BaseClass.cpp; the c690 caller 0x00B3B8E0 takes engine/CBaseClassNonCopyable.h.
Outputs: BINDINGS.tsv (44 RETIRE / 21 ADD / 13 KEEP, 0 CONFLICT; bindings_multi_check 0 on runs/reloc_bindings.after.tsv), LABELS.tsv 22,
RENAMES.tsv 8, QUEUE_W23.tsv 20 (respelled 3, respelled-partial 2, deferred 14, hold 1), DEFERRED.tsv 459, DECISIONS_W23.tsv 9.

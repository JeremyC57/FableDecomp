# NF440 (Account A naming queue, 2026-11-25): the R-NF428-1 CCountedPointer<T> family under the best-guess rule

Input: `pending/2026-11-25_nf428_handback` (FAMILY_TABLE, T3, hold_r1), NF428's scratch evidence (occ / occ_tu / gate / census objects, read
only). Tools copied to `tools/` and repointed to this directory. Tracker re-run here (`status/rec.json`, naming_status.py --dump-rec).

## Headline
- **R-NF440-1 blocks every best-guess name in this family** (RULINGS_NEEDED.tsv). The owner rule names "one PDB name set" and "a layout
  predictor" as valid bases, but `descriptive_check.py` fails PDB_NAME_ELSEWHERE on ANY symbol in egor / fablewin_pdb_names.tsv, and every
  plausible name for an uncalled `CCountedPointer<T>` copy is a FableWin.pdb name (419 `??1`, 420 `ReleaseData`, 185 `ShareData`; their
  addresses are FableWin's own debug VAs, not retail ones). The reverse also bites: naming_status grades such a self `agree_pdb` with no
  registry line (name_gate prints `OK self agree_pdb` for a guessed name), so landing without the registry would count guesses as proven.
  The guess rows are therefore fully gated and HOLD "R-NF440-1 only" (bg batches); nothing was dropped from BEST_GUESS to slip the gate.
- Landable now (ACCEPT): rows whose name the binding table already binds at the address (a landed caller). They rest on ONE source (the
  caller binding, a basis EXTRA_RULES lists) but add no new claim. The gate rejects their registry lines (ADDRESS_PROVEN + PDB_NAME_ELSEWHERE,
  R-NF440-1 (c)), so they ship without one; the lines are in evidence/BEST_GUESS_acc_side.tsv for the lander once ruled:
  - `nf440_acc1` (5): the D / SH stand-ins whose callers already bind a PDB name.
  - `nf440_acc2` (9): NF428's hold_r1 Reset rows whose only gap was the RTTI second source; the callers' binding is the one source
    (comment rewritten to say so; code byte-identical to NF428's measured text).

## How T was chosen (D53 / D44 / SH67 copies)
Every name is a FableWin.pdb member name that no retail address binds or defines (reloc_bindings + self_symbols), used once
(`tools/assign440.py` -> `evidence/assign.tsv`, one global pass).
- **T1 BOUND (5):** a landed caller binds a PDB D / RD / SH name at this address: take it.
- **T2 TU order (192 named):** the copy sits in a retail TU segment whose FableWin module instantiates the family (NF428 gate.json 'same'
  segments, anchors = nearest plain agree_pdb functions). The calibration below covers ANCHORED segments only; 165 of the 170
  batched T2 rows sit in segments with no named anchor (the alignment is then fixed by counts alone). Count-only calibration: re-aligning every anchored segment with ALL its anchors removed
  predicts the named copies 19 right, 2 wrong (9 ambiguous, 5 no fit): about 90% where it decides. The module's pointee types in FableWin order are aligned monotonically with the
  segment's copies in address order (each T once, or up to twice -- ReleaseData then ~ -- when the segment has more copies than types),
  with the already-named copies as fixed anchors; T2 only where EVERY alignment consistent with the anchors gives this copy one T (DP count,
  `tools/assign440.py`). Calibration: tc_village's 7 named copies follow FableWin's T order exactly; leave-one-out on the 20 named anchors in
  consistent segments (`tools/loo440.py`): 18 right, 0 wrong, 2 ambiguous; 15 anchors sit in segments whose anchors contradict the order
  (lib_3d_primitive_2, lib_3d_mesh_file_submesh, lib_3d_animation_2, lib_3d_anim_file): those segments get no T2.
- **T3 neighbour (134 named, NOT batched):** a unique T from the functions around the run (`tools/pred440.py`: the expander after the run,
  the function before it, an adjacent named member; inside the TU module set when there is one). Leave-one-out on named copies: 13 right,
  11 wrong (54%): too weak to batch; kept as candidates in assign.tsv.
- **Member (~ vs ReleaseData):** retail emits this body under both names (0x0041BB30 RD / 0x0041BC90 ~ CTexture), and capacity forces it
  (384 free `??1` + 388 free `ReleaseData` names for ~720 open D copies). Rule: a T with two copies -> ReleaseData on the earlier, ~ on the
  later (named pairs: RD first 5 of 6); one copy -> the shape's base rate among named copies (D53: ~ 23 / RD 5; D44: RD 29 / ~ 7); forced
  when the other member name is taken. The RD form is the landed one (0x00403750 / 0x004190B6: `#define
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` + explicit instantiation); bytes identical to the ~ form (objsec ALL_EQ).
- **Collisions:** one symbol, one address throughout (assign.tsv: 0 duplicate symbols; descriptive_check across all BEST_GUESS files: 0
  DUP_*). Higher tier wins; a loser takes the other member when free, else HOLD (e.g. 24 copies predicted CEnginePrimitiveManagerWeatherMask,
  7 CDriveManager: two kept, the rest HOLD "no free member name left").
- Evidence mix in the batches: 14 one-source caller bindings (5 T1 + 9 Reset), 170 one-source TU-order guesses (T2), 0 T3; 0 two-source.

## Forms
- D / RD rows: the HEAD explicit instantiation of the shared header's body with T renamed (and the RD opt-in for ReleaseData). No header
  change; no local class body. Class-scope T (nested in a class with a layout) needs that class's header: 15 T2 rows set aside.
- SH rows: also `#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA`. The ShareData leaf is unchanged (the release's zero stores are
  overwritten), and the pick-any `ReleaseData<T>` copy the TU emits becomes retail's Data-first RD body instead of the header's Info-first
  default: with a real T that copy is a retail name, and it is EQ at its retail address where one is known (pk: 008aa8e0's copy EQ at
  0x007E70E0) and equal to the RD rows' leaf (objsec440 checks it byte- and reloc-equal).
- Tests: new, one per row: shared header with the member declaration-only (DTOR / RELEASEDATA / SHAREDATA _DECL_ONLY), T only declared,
  linked with the row object (hn426: a self-contained link would be reported). They print the catalog PassPattern. Leaf bytes are unchanged,
  so no mutation control is needed (NF419 r1); NF428's 0x004441A0 control shows the same test shape fails on a mutated body.

## Gates (per batch: evidence/, CHECKS.tsv)
hn426 (HEAD + new compile, leaf bytes, test linked with row) / objsec440 (428 for acc2) / name_gate --test / pk_all HEAD v new with
--addrs = every assigned name (work/newaddrs.tsv) / linkprep440 + /FORCE link (definers = NF428's 151 census objects that DEFine a batch
name + the HEAD objects of the rows whose relocations bind one; control: a duplicated strong witness object adds its LNK2005) / vland
HEAD v new at one LANDVERIFY_WORK path, LANDVERIFY_STRICT_LEAF=1 / descriptive_check verbatim (evidence/batch_gates.txt) / apply_table
--dry / reserved_check (RESERVED_REFS="").

## Result
| batch | rows | verdict | names |
|---|---:|---|---|
| nf440_acc1 | 5 | ACCEPT | T1: the name a landed caller already binds here (00535EBB RD<CListArrow>, 00578A8D ~<CComponentContainer>, 007044D0 / 008AA8E0 ShareData, 00CDD4F0 ~<const CPredicateBase>) |
| nf440_acc2 | 9 | ACCEPT | NF428 hold_r1 Reset rows on the callers' binding (one source); code byte-identical to NF428's |
| nf440_bg1..bg3 | 50 each | HOLD "R-NF440-1 only" | T2 best guesses, every other gate passes |
| nf440_bg4 | 20 | HOLD "R-NF440-1 only" | T2 best guesses, every other gate passes |
All 184 rows: hn RELOCATION_MATCH both sides, leaf bytes SAME, test PASS linked with the row object; objsec ALL_EQ (+ the listed SH_RD /
PICKANY / R2 notes; acc1 0x007044D0 keeps the plain SH form: its ReleaseData<CScriptThing> copy has no retail address and
386 sources name the type, so the Data-first opt-in was not risked there; 0x008AA8E0 keeps it: its copy is EQ at 0x007E70E0); name_gate --test PASS; vland HEAD v new RELOCATION_MATCH PASS, 0 new warnings (HEAD acc2 9 STANDIN warnings -> 0),
0 LITERAL_DIFF; pk_all 0 NEQ, 0 reach flips; /FORCE link duplicates never up (acc1 0->0, acc2 4->4 pre-existing, bg 0->0), new unresolved
0 (CCountedPointer unresolved: acc1 4->2, acc2 10->7, bg1 6->6, bg2 3->3, bg3 0->0, bg4 2->2); control adds exactly its LNK2005; apply_table
--dry 0 misses / no new multi-target (TABLE_CHANGES header only); reserved_check 0. descriptive_check (repo, verbatim): acc 0 lines 0 FAIL;
bg 170 FAIL, all PDB_NAME_ELSEWHERE; across all BEST_GUESS files concatenated, the private copy without that kind: 170 lines, 0 FAIL
(evidence/desc_filtered.txt): no DUP_ADDRESS / DUP_SYMBOL / SYMBOL_ELSEWHERE / OTHER_SPELLING / ADDRESS_PROVEN / PLACEHOLDER.
Best-guess vs proven: 0 proven by two sources; 14 on one source, a caller's existing binding (ACCEPT, no new claim); 170 one-source
TU-order guesses (held for R-NF440-1).
RENAMES / CATALOG_CHANGES: catalog names and build_candidates Module follow the new T (the stand-in labels `_global::CCountedPointer_Release_<a>`,
wrong-T modules such as 008AA8E0's CScriptThing); new file names `CCountedPointer<flat T>_<Member>_<addr>.cpp`.

## Exposed / existing debt (not regressions)
`evidence/exposed_debt.tsv` (per batch): for acc1 only 008AB960 / 00570332 / 00D6D670, all names already bound in HEAD (pre-existing
NEQ); acc2 none. The bg entries appear only when the bg batches land. Landed objects that already emit a pick-any copy of a name these batches give an address, with a different
body (mostly the header's Info-first default ReleaseData v retail's Data-first body): census 004A6550, 004FBF60, 004FF080, 00506D40, 00507C30,
0051CCD0, 00714D30, 00771660, 00771B30, 008AB960, 009E2E30, 00B94BF0; witnesses 00570332 (~<CComponentContainer>, already bound at 00578A8D
in HEAD) and 00D6D670 (RD<ScriptedThingBase>, already at 0x007E70E0). None is reached from a leaf. NF419 r2: existing debt made visible;
queue those rows' ports (the same RELEASEDATA opt-in fixes the RD ones). Census coverage: NF428's 151 objects only (sources naming
CCountedPointer + Reset); the other landed definers of these names were not compiled (CScriptThing alone is named by 386 sources) --
unchecked for body equality, though they cannot add duplicates (pick-any) and no strong definition of a batch name exists in any landed
source (grep of every `template<>` specialization: only declarations).

## Not done (resume)
- T3 candidates (134) and T2 class-scope (15, need the outer class's engine header: `_hdr` batch), T2 template / odd T (2).
- bound_other (10): callers bind stand-in names (`??1CSplinePointer@@QAE@XZ`, `?DestructMember_<a>@@...`); the T2 name needs caller
  respells (TABLE_CHANGES) -> own batch.
- HOLD with no candidate / collision / TU conflict (526, reason per line in T3.txt): no TU module set and no named neighbour.
- Reset rows: 3 hold_r1 rows stay held (NF428's pick-any ReleaseData reasons); the other 38 Reset + 13 ctor(T*) rows of the R-NF428-1
  "one source" line and the 42 other shapes were not started.
- Locks: acc rows stay NF440; the 170 bg rows were released (6 of them -- 0050ECC0 0050F140 008231A0 008231F0 00AAAF90 00AAAFE0 -- were
  never locked: a word-join in my lock loop; nobody else held them).
- Tools: tools/assign440.py (assignment), pred440.py, loo440.py, gen440.py, drive440.sh / drive440r.sh, objsec440.py, linkprep440.py,
  finalize440.py, t3_440.py, desc_nopdb440.py (private gate copy, supporting evidence only).
- acc2 history: an earlier acc2 run added FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA to the 6 form-b rows (their pick-any ReleaseData<T> copy
  then EQ at the bg guesses' addresses), but against the repo as it stands that copy would differ from landed definers (00506D40 / 0051CCD0,
  00771660 / 00771B30): NF428's hold condition. Reverted; acc pk now uses --addrs = the acc names only (work/accaddrs.tsv). When the bg
  batches land, those six rows' default-body copies become visible NEQ debt at the bg addresses (fix: that opt-in, then).
- Sibling / pending names: my 184 symbols v every pending/*/SELF_SYMBOLS + TABLE_CHANGES and NF437-439's SELF_SYMBOLS / TABLE_CHANGES /
  BEST_GUESS (222 files): 1 hit, nf222's applied delete of a stale ~<NGameText::CDataBank> binding at 0x00403750 (no longer in the table):
  no conflict.
- SH rows in bg carry the RELEASEDATA opt-in: re-check their ReleaseData<T> copies against all definers at ruling time (same exposure as
  0x007044D0; census coverage partial).
- runs/pilot, runs/shx, runs/acc2x are measurement dirs (form trials), not for landing.

## Resume round (after R-NF440-1 granted; acc1/acc2 landed at 03aac77523; gate fix abcd45ab83)
Emitter check for bg1..bg4 (`tools/emitcand440.py`, `emit440.py`, `pkemit440.py`, `mkp440.py`):
- Candidates: every landed row whose own text, or any header it includes (transitively), spells `CCountedPointer<[const] [ns::]T>`
  once comments are stripped (CCountedPointer.h's comments named CAFile and pulled in 14,473 rows before stripping). Tests are excluded
  because they are never linked into the exe. Union over the four batches: 4,909 rows, all compiled at HEAD with catalog flags (0 errors).
  An object counts as an emitter only if it DEFines the guessed `??1` / `?ReleaseData` / `?ShareData` symbol.
- pk_all ran on each emitter object with its own leaf and `--addrs` = that batch's own SELF_SYMBOLS (the names its landing registers).
- Result, all four batches: 0 reached NEQ; 0 emitters whose leaf relocations map a guessed name to another retail address; every
  emitter was measured. So 170 of 170 rows are landable: 119 ACCEPT (emitted copies all EQ, or no other emitter) and 51 ACCEPT+DEBT
  (unreached NEQ copies, mostly the header's Info-first ReleaseData default and 5-byte `jmp ReleaseData` destructors in
  RELEASEDATA_DECL_ONLY TUs). There are 145 debt lines in total, in `batches/nf440_bg*p/DEBT.tsv`, each tagged to the row's registry line
  and noted in its BEST_GUESS line.
- No retail 5-byte jmp thunk targets a guessed or bound ReleaseData<T> of a ~ guess, so there is no thunk-destructor evidence against
  the ~ guesses.
- An EQ emitter copy is not evidence for T: every 53-byte copy has the same body.
- Batches `nf440_bg1p..bg4p` (50/50/50/20) have verdicts flipped to ACCEPT and carry DEBT.tsv. descriptive_check --extra: 0 FAIL each,
  and 0 FAIL over all four together (184 lines including the 14 registered). reserved_check 0; apply_table --dry: no new multi-target.
- The six never-locked rows were free (no lock dir); all 170 bg rows are now locked NF440.
- Accuracy per batch (T / member):
  - bg1p: anchored 4, unanchored 46; member: pair 32, base rate 1, TU-positional 1, ShareData 16.
  - bg2p: anchored 1, unanchored 49; member: pair 35, base rate 1, forced 1, ShareData 13.
  - bg3p: unanchored 50; member: pair 28, base rate 7, ShareData 15.
  - bg4p: unanchored 20; member: pair 10, ShareData 10.
  - Figures: T about 90% unanchored (19/21 decided) and 18/18 anchored; member pair rule about 83% (5/6), base rate about 81-82%.

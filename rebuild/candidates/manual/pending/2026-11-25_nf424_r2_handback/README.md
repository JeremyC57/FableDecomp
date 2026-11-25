# NF424 round 2 hand-back (2026-11-25): deleting destructors, S0 identity, ctor-owned units, R-NF424-1 (b), COPYCTOR test

Base: FullDecomp e6af6da0e9 (round-1 landing). Nothing was written to the repo and no git command was run. Every grade is VC7.1 RTM 13.10.3077
on the Linux lane. Retail bytes appear in no file here; oracle.tsv stays in work/.

**ACCEPT: 208 rows in 5 batches** (`acc_*/`, at most 50 each). **HOLD: 217 rows with sources** (`hold_*/`). All other rows of the
family keep their round-1 T3 line, and `T3_r2.txt` gives each one's status after round 2. ACCEPT rows stay locked as NF424. Every other lock
taken in round 2 is released (902 rows).

## 1. S0 (371 rows, shared stand-ins `?CLandscapeBackgroundPatch_vector_deleting_destructor` / `?CThingSwitch_CompositeScalarDeletingDestructor`)

**Answer to the question.** These rows are mostly not the non-virtual `??_G<T>@@QAEPAXI@Z` form:

| kind | rows | form |
|---|---:|---|
| slot of an RTTI-less vftable (data reference in .rdata, vptr stored by a ctor or inline-ctor function) | 315 | virtual `UAEPAXI@Z` |
| called as `push 0; call ??_G` (VC7.1's explicit destructor call through ??_G with flags 0) | 4 | non-virtual `QAEPAXI@Z` |
| jump target (0x00CAFCC0 / 0x00CAFD70, from 0x00CAFCB3 / 0x00CAFD63 in the DirectShow strmbase region; form not checked) | 2 | unverified |
| no reference anywhere in the image (raw dword scan of every section) | 50 | undecidable: callee ~ only |

Tools: `r2/tools/vtof.py` (vftable start and slot), `vtstores.py` (every `mov [reg+d], imm32` into .rdata over the catalog: 16,307 vptr
stores), `s0join.py`, `vtslots.py`, and `s0cls.py` (class attribution). Evidence: `evidence/s0_vt.tsv` and `evidence/s0_nonvt.tsv`.

**Class attribution for the 313 vftable rows that have a vptr store** (2 more have their .rdata site in no stored vftable; T3_r2 counts them with the 56 under "S0 not in a stored vftable"): A name needs three sources:

- **V:** a slot function referenced by exactly one .rdata dword, i.e. only by this vftable, carries the most-derived class's own override name (landed or Ego_r).
- **P:** FableWin / Ego_r name sets hold `??_G` / `??_E` / `??1` of that class fragment with a virtual dtor.
- **C:** the vptr-storing function names the class: `??0C`, `?Alloc@C`, or `CEntityScriptBinding<T>::CreateActiveEntityScript` for `CActiveEntityScript<T>`.

Results:

- **147 named.** 110 have V+P+C and 37 have V+P only.
- **120 of the named rows are `CActiveEntityScript<NScript::*>`.** Their header (`engine/CActiveEntityScript.h`) already records slot 0 as this scalar deleting destructor.
- **166 unnamed.** 163 are in the libpk / CODA region (no PDB, no RTTI; the only landed names there are placeholders such as `CVtable_*`, `CMap_00c2be10`, `CHelper_*`) and 3 in the DirectShow base-class region (0x00CA71E0, 0x00CA7340 `CBaseRenderer`, 0x00CAB750). The DirectShow rows go the vendor-library route (strmbase), not naming.

**S0 outcomes:**

- **7 RELOCATION_MATCH, all V+P+C; 3 ACCEPT.** ACCEPT: 0x00D005C0 `NScript::CQ_AwakeningTheOracleScript` (acc_pkdebt_00), 0x00F25E20 `CArenaCreatureDef` and 0x00F29CE0 `CRegionScriptDef` (acc_03). Held: 0x00F270B0 `CHeroSoulsCreatureDef` (census), and `CQ_HeroSoulsBriarScript`, `CQ_OrchardFarmRaidScript` and `CV_ArcheryCompetitionScript` (gate 9: their own `??1` has no table line). Each is a ctor-owned thin row (section 2).
- **120 `CActiveEntityScript<T>` HOLD, RULINGS_NEEDED R-NF424-2.**
  - The name is proven, but the owner TU's ??_G calls `??1CActiveEntityScriptBase` out of line (30 bytes).
  - Retail expands it: two member destructors, then `~CSpawnedFuncBase` 0x00CDD4C0 (48 bytes); see `evidence/aes_disasm.txt`.
  - The existing opt-in `FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY` takes effect but does not fix the expansion. The owner rows stay RELOCATION_MATCH with it.
  - Following the advisor, I stopped after that single probe.
- **20 other named rows held.** In 11 the owner TU emits no ??_G of the class. In 9 the body differs (`T3_r2.txt`).
- **4 QAE rows held, R-NF424-3.** 0x004509D9 and 0x00CD22C0 are byte-identical and both are called from `_Rb_tree<long, pair<const long, CCharString>>::_M_erase` copies, while 0x00CD22C0 already carries `??_G?$pair@$$CBJVCCharString@@@std@@QAEPAXI@Z`. Two addresses cannot take one name.

## 2. Ctor-owned units for S1-S3: thin rows (the nf159 TU-emitter fold, as landed by nf251 for 0x00B2AB10 under 0x00B260B0 and for 0x00BEB4D0 under 0x00BEAAA0)

VC7.1 emits `??_G<T>` as a pick-any COMDAT beside `??_7<T>`, in the TU whose code stores the vptr. `owners.py` finds, for each row's
retail vftable, the catalog functions that store it and have a landed source. The row's source is then a thin row: its comments plus
`#include "../src/compiled/<owner>.cpp"`. Generator: `r2/tools/gen_thin.py`. Owner order: the class's `??0`, then other storers, then a
`??1`; initializers are skipped. Graded with the owner's flags by `cc2.py`, and a second owner was tried where one existed (`--alt 1`).

**Pilot: 20 S1 header-port rows, 11 RELOCATION_MATCH.** Full run over every S0-named / S1 / S2 / S3 / G1-CS row that has a landed storer
(1,099 rows):

| shape | tried | RELOCATION_MATCH | DIFFER_LEAF | NO_LEAF |
|---|---:|---:|---:|---:|
| G1-CS port | 62 | 0 | 0 | 62 |
| S0 | 147 | 7 | 94 | 46 |
| S1 header port | 311 | 171 | 14 | 126 |
| S1 local model | 123 | 48 | 14 | 61 |
| S1u callee identity | 130 | 42 | 55 | 33 |
| S1x identity | 71 | 33 | 3 | 35 |
| S2 G2 | 107 | 93 | 5 | 9 |
| S3 G3 | 148 | 22 | 77 | 49 |
| **all** | 1099 | 416 | 262 | 421 |

NO_LEAF means the owner TU emits no ??_G of the class: its model gives the class no virtual destructor, or no vtable at all. DIFFER_LEAF
means it emits the leaf, but the model's member / base destructor expansion differs from retail. Both are listed per address in
`T3_r2.txt`.

**Gates for the 416 RELOCATION_MATCH rows** (`r2/work/gate.tsv`, CHECKS.tsv per batch):

1. **Identity.**
   - Non-S0 rows: `llvm-undname` class of the leaf == the retail RTTI class of the row's own vftable, and a FableWin / Ego_r entry for the class. All 416 pass.
   - S0 rows: V+P+C.
   - Every row's address is referenced by exactly one .rdata dword (no ICF-shared ??_G).
2. **Owner hygiene.**
   - No thin-includer chains (the root owner is the chosen owner for all 416). UNITS.tsv kind `fold` names the owner (the nf251 fold, not an Option S unit: Option S's .obj-membership conditions were not tested), and CHECKS gives the owner source's sha1. The owner lock / claim / queue check and sha1 were rerun just before hand-back; nothing changed.
   - 29 rows held: their owner is in T3_PORT_QUEUE (27, one of them also NF425-locked) or under NF425's lock alone (1); 2 owners are under NF425's lock in all.
3. **Census.**
   - The pick-any leaf was compared across every other landed storer of the vftable (345 HEAD objects, `r2/cen/`), every round-2 owner object and the 579 round-1 census objects (defscan).
   - 42 rows held: another landed TU emits the same `??_G<T>` with a different body.
   - After review: for the 35 ACCEPT rows matched on the second owner (`--alt 1`), the first owner's object was compared by path (defscan keys objects by stem, so both owner objects read as the row's own). In 4 rows (0x00459767, 0x004E391D, 0x004E3BF2, 0x00B3AB80) the first owner's TU emits the leaf with a different body, and they are held (`evidence/alt0_census.tsv`). The first owner's TU emits no leaf in 31.
4. **pk_all on each new object** (`evidence/pk/R2N_*`): no NEQ entry at all in 167 ACCEPT rows (acc_00..03). In 41 ACCEPT rows the owner TU's own object carries NEQ pick-any / strong entries (existing HEAD debt of the owner, e.g. `??_GCIVCountedPointeeBase` x35): batched apart as acc_pkdebt_00 under RULINGS_NEEDED R-NF424-4 (the ruling text counts 65 objects before gate 9 below removed 24 of them).
5. **Flags.** 5 owners carry flags (/Os x3, /Gr, /d2inlT200). Those thin rows carry `// Flags:` and a CATALOG_CHANGES CompilerFlags line.
6. **Old self symbols.** No row's old self symbol is referenced by another row (reloc_bindings users).
7. **vland --recheck-landed, HEAD v new** (`r2/tools/runpair2.sh`, batches of ≤50, no header overlay): 354 rows (345 thin + the 9 scaffold rows of sections 3-4): HEAD RELOCATION_MATCH/PASS and NEW RELOCATION_MATCH/PASS for all 354, strict ??_G leaf on the new side (3 graded under the owner's /Os, shown as RELOCATION_MATCH["s"] in the logs).
8. **Callee binding (added after the vland run).**
   - RELOCATION_MATCH compares relocation positions, not targets. vland's `RELOC_NAME` warning reports a callee symbol whose retail target the catalog labels differently.
   - Every new-side `RELOC_NAME` was checked against reloc_bindings. A callee bound at that retail target passes: the catalog label is stale, and the line goes to NAME_FIXES.
   - 94 rows are held because the callee is unbound or bound elsewhere. Examples: `??1CParentDefClassBase` (bound 0x00430300) at `CMaterialDef::~CMaterialDef` 0x00455D44, and `??1CBaseClass` at `CCombatActionBase::~CCombatActionBase` 0x0062E900. This was round 1's S1u / S1x callee-identity concern. The byte match does not cure it, and these rows need the class's own destructor modelled.

9. **Every leaf relocation (added after review, `r2/tools/calleechk.py`, `evidence/calleechk.tsv`).** vland is silent when a callee simply has no table line, so each relocation of each thin ACCEPT row's leaf was resolved against retail and had to be bound at that target by reloc_bindings or be the target's own self symbol. 203 of 251 pass. 48 are held: mostly the class's own `??1T` (S2) at an address with no self line, plus one UAE/QAE spelling clash (`??1CGamePlayerInterface@@UAE@XZ` against the landed self line `??1CGamePlayerInterface@@QAE@XZ` at 0x00446BE0).

Two rows have code-absolute references (0x006F8030 x1, 0x008E8D00 x18). Neither is in ACCEPT.

**One object per TU.** A thin row's object is its owner's object, so link and census sets take one of them. A naive per-row link shows LNK4006 duplicates with identical bodies (same TU), as nf251 recorded.

**Limits stated plainly.**
- **Tests.** The thin-row tests are self-contained behaviour models (as round 1's) and never touch the row's object. This round has no HEAD-v-new leaf comparison and no mutation control; the leaf's own bytes are graded by vland against retail.
- **The 71 owner / census holds** (hold_*) were not run through vland or gate 9, and they are missing from head/payload.json.
- **The (b) rows' pk and vtcheck evidence** is round-1 data (`batches/g1_r1/evidence/N_*`), copied under the `R2N_` names.

## 3. R-NF424-1 (b): the 9 held G1 rows

**My reading of "agree":** compile the class's Clone (`return new C(*this);`) against the row's own prelude and shared header, graded on
the Clone leaf against the landed Clone row's retail bytes (`r2/tools/probe_b.py`).

- **RELOCATION_MATCH** means the header's implicit copy constructor reproduces the copy retail expands inline in Clone. That makes the scaffold's out-of-line COMDAT of the same constructor the faithful one.
- **Binding.** `nf xr`-style resolution of each matching probe binds the base-copy call to 0x0062F3B0 `??0CCombatActionBase@@QAE@ABV0@@Z`, as retail does (table: landed, RELOCATION_MATCH); see `evidence/clone_xr.txt`.

| row | Clone | result |
|---|---|---|
| 0x008CD280 BalverineCorneredJumpIntoSky | 0x008CD330 | agree: ACCEPT |
| 0x008CEE50 CastSkillMultiArrow | 0x008CF260 | agree: ACCEPT |
| 0x008F5700 DragonRearUpInto | 0x008F5850 | agree: ACCEPT |
| 0x00925A50 TentacleStrikeInto | 0x00925C40 | agree: ACCEPT |
| 0x008C9670 BanditKingAttack | 0x008C97E0 | DIFFER(85v71): held |
| 0x008C96D0 BanditKingUnstuck | 0x008C98E0 | DIFFER(85v71): held |
| 0x008CECC0 CastTimeSpell | 0x008CEE70 | DIFFER(83v59): held |
| 0x0097D130 VillagerAttackKnockdown | 0x0097D1C0 | DIFFER(83v59): held |
| 0x008CA960 NymphTeleportLimboBase | none | no Clone (header declares none): held |

For the 4 ACCEPT rows, the differing pick-any copy in the landed local-model Clone rows (round 1's C_DIFF) stays at HEAD until those
Clone rows are ported to the shared header. pk reads NOADDR for that copy (no retail home), and nothing is bound to it.

## 4. CCreatureActionBase COPYCTOR test

The opt-in already exists: `FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR` in `engine/CCreatureActionBase.h` declares the copy ctor (retail
0x0062EBC0, landed RELOCATION_MATCH), and 31 landed rows use it. The header is therefore unchanged, and so are other includers. I added
the define to the prelude of the 7 round-1 G1 holds whose reason was CCreatureActionBase's special members.

- **Effect on the leaf.** All 7 stay RELOCATION_MATCH on the leaf. `??0CCreatureActionBase@@QAE@ABV0@@Z`, `??_7CCreatureActionBase@@6B@` and `??_GCCreatureActionBase@@UAEPAXI@Z` leave the object, and the copy call becomes UND to the landed 0x0062EBC0 (`evidence/copyctor_syms.txt`).
- **5 cleared:** 0x0062D7D0 StartSneaking, 0x0062D870 StopSneaking, 0x0084AB80 YellOverHere, 0x0084B4A0 PlayAnimationFromIndex, 0x0087FE80 StabbedToDeath.
  - Each class's landed Clone row already uses the shared header with COPYCTOR, and the Clone probe grades RELOCATION_MATCH with the call bound to 0x0062EBC0.
  - defscan shows the scaffold's `??0C@@QAE@ABV0@@Z` byte-and-reloc identical to the Clone row's copy, which meets R-NF424-1 (a). `??_GC` is identical across all definers.
  - vtcheck: VT_EQ. pk: 0 NEQ.
  - These are ACCEPT.
- **2 not cleared:** 0x006A0970 / 0x006A0990 (SheatheItemToInventory_ForDivineWrath / _ForUnholyPower) still emit `CCreatureAction_SheatheItemToInventory`'s copy ctor, `??_7` and `??_G`, plus the `CCountedPointer<CCreatureActionBase>` members, with no retail home. They are held.

## 5. Files

- **`acc_NN/`, `hold_NN/`:** `rows/`, `tests/`, `orig/rows` + `orig/tests` (HEAD copies), `head/payload.json` (HEAD payload subset, vland HEAD run), RESULTS.tsv (col 4 = the row's own `??_G` self symbol), SELF_SYMBOLS.tsv (status from the vland NEW run), CATALOG_CHANGES.tsv (`address field old new reason`: name relabel, Module from the `CVtable_*` placeholder, CompilerFlags), CHECKS.tsv, UNITS.tsv, TABLE_CHANGES.tsv (empty), logs/vland_lines.txt.
- **`T3_r2.txt`:** per-address status after round 2 of every T3 line.
- **`RULINGS_NEEDED.tsv`:**
  - R-NF424-2: CActiveEntityScript<T> destructor expansion.
  - R-NF424-3: two QAE copies, one name.
  - R-NF424-4: owner-TU pk debt in acc_pkdebt_00.
- **`NAME_FIXES.tsv`:** catalog labels a bound callee contradicts.
- **`evidence/`:** calleechk.tsv, alt0_census.tsv, pk_neq.tsv, clone_probes.txt, s0_vt.tsv, s0_nonvt.tsv, s0_aes.tsv, aes_disasm.txt, clone_xr.txt, copyctor_syms.txt, copyctor_defscan.txt, census_thin.tsv, gate.tsv, vsum.tsv, histogram.txt, and pk/.
- **Tools:** `r2/tools/` (copied to `tools/`).

## 6. Histogram (all 1,625 rows, after round 2)

| shape (round-1 T3) | rows | unchanged | r2 ctor-unit: no leaf | ACCEPT | HOLD | r2 ctor-unit: body differs | S0 not in a stored vftable | landed | HOLD R-NF424-1 (b) | S0 no name | HOLD R-NF424-2 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| S0 identity | 371 |  | 11 | 3 | 4 | 9 | 58 |  |  | 166 | 120 |
| S1 header port | 314 | 3 | 126 | 110 | 61 | 14 |  |  |  |  |  |
| S3 G3 | 148 |  | 49 | 12 | 10 | 77 |  |  |  |  |  |
| S1u callee identity | 130 |  | 33 | 4 | 38 | 55 |  |  |  |  |  |
| S1 local model | 125 | 2 | 61 | 18 | 30 | 14 |  |  |  |  |  |
| S5 | 108 | 108 |  |  |  |  |  |  |  |  |  |
| S2 G2 | 107 |  | 9 | 39 | 54 | 5 |  |  |  |  |  |
| S1x identity | 71 |  | 35 | 13 | 20 | 3 |  |  |  |  |  |
| skip | 63 | 63 |  |  |  |  |  |  |  |  |  |
| G1-CS port | 62 |  | 62 |  |  |  |  |  |  |  |  |
| G1 hold (batches/g1_r1, R-NF424-1) | 56 |  |  | 4 |  |  |  | 47 | 5 |  |  |
| G1 hold (batches/g1_h1) | 34 | 27 |  | 5 | 2 |  |  |  |  |  |  |
| S7 | 11 | 11 |  |  |  |  |  |  |  |  |  |
| S6 | 8 | 8 |  |  |  |  |  |  |  |  |  |
| S4 | 5 | 5 |  |  |  |  |  |  |  |  |  |
| G1 precheck | 3 | 3 |  |  |  |  |  |  |  |  |  |
| G1 hold | 2 | 2 |  |  |  |  |  |  |  |  |  |
| **all T3 lines** | 1618 | 232 | 386 | 208 | 219 | 177 | 58 | 47 | 5 | 166 | 120 |

Plus the 7 round-1 ACCEPT rows (landed before round 2): 1,625 = 1,618 T3 lines + 7. Landed if this hand-back lands: 7 + 47 + 208 = 262 of 1,625.


## 7. Next

- **S4-S8 (about 140 rows).** The thin-row pipeline is shape-agnostic and was not run on these shapes (out of this round's scope). Run `owners.py` → `gen_thin.py` → `cc2.py` → `gate.py` → `runpair2.sh` on them.
- **The 94 + 48 callee-binding holds.** Each needs the class's own out-of-line destructor (S2-style model `virtual ~T();` bound to retail's target) in the owner's header.
- **R-NF424-2 unlocks 120 rows mechanically.**

## Coordinator landing notes (2026-11-25)
- **Landed:** all 208 ACCEPT rows, in `batches/2026-11-24_landing_nf424r2_{acc_00,acc_01,acc_02,acc_03,acc_pkdebt_00}`, with 0 held.
  - Self symbols applied: 201 updated, 7 added.
  - Catalog changes applied: 195 names and 3 Modules.
  - The NF424 locks were released before landing.
- **R-NF424-4** (the 41 `acc_pkdebt_00` rows): accepted by the coordinator on precedent (NF422 Q1, GOTCHAS NF419 r2). The NEQ entries are the owner TU's existing HEAD debt, not new copies.
- **Still open:**
  - **R-NF424-2** (120 `CActiveEntityScript<T>` rows): open for the next round.
  - **R-NF424-3** (4 QAE rows): left on the stand-in, as proposed.
- **Thin rows and `s_unit_check`:** the thin rows' text says "Option S", so `s_unit_check` now treats them as members of their owners. 21 owners then failed because their `// Leaf:` was missing or plain:
  - 18 now carry the measured self symbol from `self_symbols.tsv` (a comment-only change);
  - 3 dynamic-initializer owners keep `_$E1`, which `s_unit_check` now accepts as a complete symbol.
  - Result: 126 units checked, 0 failed.
- **T3:** the queue's `nf424 family` lines are replaced by one line per round-2 status (1,363 addresses). The per-address list is `T3_r2.txt`.
- **Not committed:** the hold batches, which stay in the agent's scratch.

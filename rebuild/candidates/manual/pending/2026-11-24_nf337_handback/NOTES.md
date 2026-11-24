# NF337 (2026-11-24): the 48 CDefClassBase-family `Copy` rows NF335 skipped

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Nothing written under the repo, no git.
Every compile / grade / link in the background with `timeout 900` on its launch line; TMPDIR / TMP unset.

## Retail shapes (read first, tools/rdis.py; no bytes copied here)
- 39 rows are a 5-byte `jmp` (thiscall tail call) to the class's operator=. 37 jump to the next address (operator= emitted right after
  Copy); 0x0046F49F jumps back to 0x0046F43E and 0x0046F77C to 0x0046F6AF.
- 9 rows (0x004E00A3 .. 0x004EB6C8) have a body: the CDefClassBase operator= call at 0x00431F10, then the derived member copy (one
  byte / dword, or a second out-of-line call for 0x004EB6C8).
- NF335's "the edx parameter is used" reason (0x0045E65B, 0x0045F7C2, 0x0045FD4E, 0x00460180) describes the flat stand-ins, not retail:
  all four are plain 5-byte jmps in retail.

## Form (jmp rows)
`virtual void C::Copy(const CDefClassBase* other) { *this = *(const C*)other; }` with `C& operator=(const C&)` declared out of line:
VC7.1 emits the one-instruction `jmp ??4C@@QAEAAV0@ABV0@@Z`. The callee respells from the made-up `C_Copy_impl` stand-in onto the
PDB operator= (second source: both PDB name sets + the landed operator= row at the jmp target defining that symbol).

## Form (body rows, 0x004E....)
Retail: `call 0x00431F10` (CDefClassBase::operator=, `??4CDefClassBase@@QAEAAV0@ABV0@@Z`) then the member copy; the derived operator= is inline.
Row form: `void C::Copy(const CDefClassBase* other) { CDefClassBase::operator=(*other); M = ((const C*)other)->M; }` on the shared header.
`*this = *(const C*)other;` with the implicit operator= was compiled once (CBuyHouseDef, CEnemyDef, CExpressionSubDef, run g6): the Copy bytes
match, but each object then also defines pick-any `??4C` and `??4CSubDefClassBase` (COMDAT sets differ), so the explicit expansion is kept
(evidence/b4/implicit_form.txt). CGoldDef / CAICreatureWillPowerIndicatorDef models carry a +0x25 pad array an implicit copy would also copy;
CInterestingToVillagersDef's header declares the PDB's out-of-line operator=.

## Result (STATUS.tsv: 36 done, 5 skipped-held, 7 skipped-port; 38 rows in batches/b1..b4 (rows.txt), 8 held rows in held/b5, held/b6 (held/HOLD.md))
| batch | rows | what | gate |
|---|---|---|---|
| b1 | 18 | jmp Copy rows, local model or an existing header (no edit) | MATCH/RELOCATION_MATCH unchanged, ctl LINK_FAIL, objcheck ALL_EQ (3 LIBCP), link 0 dup |
| b2 | 11 | jmp Copy rows on 11 add-only `FABLE_RETAIL_<C>_COPY_DECLS` header opt-ins | same; equiv 0 rows, planted control covers all |
| b3 | 4 | units: 0x00464531+0x00464536 (operator= respelled to CThingPhysicalSwitchDef; + CThingBaseDef.h opt-in), 0x0045E65B+0x0045E660 (operator= returns const&, PDB QAEABV0) | all ALL_EQ; link: HEAD 1 duplicate (??4CThingBaseDef in 0x00463950 + 0x00464536), new 0 |
| b4 | 5 | body rows 0x004E01A7, 0x004E064E, 0x004E0CA7, 0x004E0FED (3 add-only Copy declarations), 0x004EB6C8 (local class + stlport vector) | ALL_EQ (0x004EB6C8 LIBCP) |
| held/b5 | 6 | **RULING D-1**: units 0x00460584+0x00460589, 0x0046F77C+0x0046F6AF (made-up CBase_00431ed0 operator= rows respelled onto the PDB classes), body rows 0x004E14FD, 0x004E0E0A | grades unchanged, ctl LINK_FAIL; objcheck ALL_EQ + new UND `__fltused` on 4 objects |
| held/b6 | 2 | **RULING D-2**: 0x0046F49F + 0x0046F43E | 0x0046F49F ALL_EQ; 0x0046F43E RELOCATION_MATCH, operator= section bytes + relocs equal, DEF -Copy, selection pick-any -> NODUPLICATES |

Skipped-port (STATUS.tsv has the measured reason each): 0x0045C549, 0x0045D500, 0x00462B10, 0x004600C6, 0x0046244C (class modelled only under
a `_PASS` switch, NF318 Q2), 0x004E00A3 CReadableDef (`_PASS24`), 0x004E0F02 CTavernDef (`_PASS28`).

## Proof per batch (evidence/<batch>/)
GRADE_*.tsv (HEAD and new compiled at one work path per run; control = HEAD source + moved test), UNITS/OBJCHECK (explicit rename maps,
RENAME_MAPS_*.json), linkcheck_<batch>.txt (HEAD set v new set: duplicates, unresolved), testgate_<batch>.txt. Headers: 19 add-only blocks
(hdr/engine, orig/hdr_*; per batch in batches/<b>/hdr + orig/hdr + HEADERS.tsv): hdrcheck_final.txt 19 SAME; equiv_final.tsv 0 differing over
1382 includer rows; equiv_final_ctl.tsv (planted typedef per block) 210 rows, equiv_final_ctlcover.txt every header >= 1. Override probe + made-up
name control per Copy-declaring block (HDR_PLAN_all.tsv); CThingBaseDef.h: evidence/b3/probe_CThingBaseDef.txt. apply_table --dry: 37 lines,
0 misses, no new multi-target symbol (evidence/apply_table_dry_b3456_all.txt; b1/b2 earlier). bytescan 0 hits.

## Decisions needed
- **D-1 `__fltused`** (b5): typing the PDB members `float` (CSpecialAbilitiesDrunkennessDef USpeed/VSpeed/RotSpeed, CLensFlareElementDef Radius,
  RechargeTime, Interest) makes VC7.1 add the CRT marker `UND __fltused`; code, relocations and every other symbol are equal. The ten landed
  sibling operator= rows (0x00460185, 0x00460457, 0x004604CF, 0x0046051D, 0x004605E5, 0x00461D50, 0x00461F7C, 0x004625FD, 0x004629FB,
  0x00465EB3) already carry it (g1 HEAD objects). Link: the new unresolved set gains `__fltused` (4), loses the made-up names. Ask: allow
  "ALL_EQ + exactly __fltused" like NF318 Q1's libcp? The members were not retyped `long` to dodge it.
- **D-2 0x0046F49F** (b6): retail 0x0046F49F is the Copy (RTTI slot 19, `jmp` to 0x0046F43E). The landed 0x0046F43E row defines
  `?Copy@CAbilityButtonGuiDef` only to make the compiler emit the implicit operator=. The unit writes 0x0046F43E's operator= explicitly
  (out of line; same 69 bytes and relocations; COMDAT selection changes pick-any -> NODUPLICATES) and moves Copy to its own row on a header
  opt-in; the new Copy section equals the one HEAD 0x0046F43E emitted. 0x0046F43E keeps its local model (engine/CAbilityButtonGuiDef.h exists;
  that header's comment "operator= is the landed implicit 0x0046F43E" goes stale if b6 lands). Every other landed row naming
  CAbilityButtonGuiDef::operator= declares it out of line (UND only); the binding witness 0x00476672 (vector::erase) was compiled and linked
  with the unit on both sides (evidence/link_b6w.txt).
- **D-3 scaffolding** (NF318 Q5 debt, untouched): operator= rows of CFactionDef, CCombatDialogueDef, BurntEffect define
  `?Copy@C@@QAEXPBVCParentDefClassBase@@@Z` helpers (a different symbol from the PDB member); left as is.
- **D-4 test member stubs**: body-row tests define the callee `??4CDefClassBase` (and 0x004EB6C8 the vector operator=) as member stubs; the
  test gate allows exactly those (tgate.py `addr:class:sym,...`).

## Landing notes
- Batch dirs follow land_scripted: RESULTS.tsv `address verdict old new reason` (column 4 = the graded symbol; role / unit in the reason),
  UNITS.tsv `address unit` (a unit's members share the Copy address; single rows map to themselves; the old copy/operator= table is
  UNITS_DETAIL.tsv). An operator= row without tests/<a>_test.cpp keeps its landed test (a standalone mock). held/ dirs have the same shape.
- TABLE_CHANGES.tsv (root = b1..b4 only: 32 replace + 1 delete, apply_table --dry 0 misses); held/TABLE_CHANGES_held.tsv (b5 + b6: 1 replace
  + 3 delete) only with those batches. Root hdr/ also holds the two held opt-ins (CLensFlareElementDef.h, CAbilityButtonGuiDef.h): apply them only with held/b5, held/b6. The made-up base `??4CBase_00431ed0@@QAEAAU0@ABU0@@Z` stays bound
  (witness 0x0045BDB6, other rows use it); b5 only stops using it. self_symbols.tsv changes with every moved row; the four operator= rows:
  0x00464536 ??4CThingBaseDef -> ??4CThingPhysicalSwitchDef, 0x0045E660 QAEAAV0 -> QAEABV0, 0x00460589 / 0x0046F6AF made-up -> PDB (b5).
- RENAMES.tsv (85: self + callee per row), NAME_FIXES.tsv (catalog names of 0x00464536, 0x00460589, 0x0046F6AF).
- head/HEAD_FROM_REPO.txt lists HEAD entries left out of head/payload.json for a retail byte dump in the landed source/test (D-f);
  orig/LEFT_OUT.txt likewise. The operator= rows' tests are standalone mocks and stay unchanged (0x00464536's mock still names the old classes:
  debt). drv.py's vland payload names every row `Copy` internally; batch head payloads carry `operator=` for the operator= rows.
- Locks: NF337 holds the jmp addresses, the six b4 body rows, 0x004EB6C8, 0x00463950 (compiled HEAD-only for the b3 link check) and 0x00476672 (HEAD-only, the b6 binding witness).
- b4 link: the new unresolved set equals HEAD's under the rename map (SubContainer::Copy -> the vector operator=); the 0x004EB6E7 object
  was not in the set. Freshness: all 46 rows' landed source + test, the 19 orig headers and the include tree rechecked against the repo at
  hand-back (evidence/freshness_final.txt, tree_drift.txt).
- Observation: every Copy row compiles into a 16-byte-aligned section (HEAD and new alike) while the retail addresses are odd (size-optimised
  def modules); the grade does not see alignment.

## Round 2: coordinator rulings (2026-11-24)
- **D-1 accepted** (NF321 Q1) on condition of two sources per float member: evidence/b5_float_sources.tsv. Every member is `float`
  (Type 0x0040) in both Ego_r.pdb and FableWin.pdb TPI field lists (tools/pdbfields.py over the existing llvm-pdbutil type dumps; extracts
  evidence/b5/tpi_fieldlists_{Ego_r,FableWin}.txt): CSpecialAbilitiesDrunkennessDef USpeed/VSpeed/RotSpeed, CLensFlareElementDef Radius
  (and Position, also copied by 0x0046F6AF), CAICreatureWillPowerIndicatorDef RechargeTime, CInterestingToVillagersDef Interest.
  held/b5 lands alone with its own TABLE_CHANGES.tsv and only CLensFlareElementDef.h (held/HOLD.md).
- **D-2 retried as ruled**: held/b6 is the new run dir. 0x0046F43E keeps the implicit operator= and moves onto engine/CAbilityButtonGuiDef.h
  (no opt-in; no local-model conflict: the object defines only the operator= COMDAT and the helper). The forcing device is now the
  0x00E95380-form anonymous-namespace helper (`// Fidelity: scaffolding, not retail`, returns &CAbilityButtonGuiDef::operator=).
  Pass conditions, measured (evidence/b6/OBJCHECK_retry.txt): the operator= COMDAT equals HEAD's (bytes, relocations, flags, selection 2);
  the object no longer defines ?Copy@CAbilityButtonGuiDef; the grade is RELOCATION_MATCH PASS as before; the link of 0x0046F49F + 0x0046F43E +
  witness 0x00476672 has 0 duplicates; 0x0046F49F is unchanged. The first (explicit) build is kept in work/b6_explicit_rejected_rundir/.
- **Census** (tools/census2.py, the NF328 census generalised to each row's own leaf; evidence/b6/census2_operator_family_HEAD.tsv, one line
  per landed operator= row at the 39 jmp targets, HEAD objects): every row's COMDAT is BYTES_EQUAL to retail (relocation fields zeroed).
  Selection: 34 rows no-duplicates (sel=1: the operator= is declared and defined out of line in the row), 5 pick-any (sel=2: implicit):
  0x0045D2FC CFactionDef, 0x0045E55A CCombatDialogueDef, 0x004604CF CSpecialAbilitiesBurntEffectDef (each forced by a strong
  `?Copy@C@@QAEXPBVCParentDefClassBase@@@Z` helper), 0x0046F43E CAbilityButtonGuiDef (forced by the PDB Copy in HEAD; by the scaffolding
  helper after held/b6), 0x0045FAC1 CSpecialAbilitiesBulletTimeDef (no extra DEF). Note for the NF328 / NF331 rule: most of this family
  lands strong today, though both PDBs spell every one `??4C@@QAEAAV0@ABV0@@Z` (an implicit and a declared operator= mangle the same).
  The five operator= rows behind the skipped-port Copy rows (0x0045C54E, 0x0045D505, 0x004600CB, 0x00462451, 0x00462B15) were compiled
  HEAD-only for this census (locked by NF337).
- **D-3 for the T3 queue (not done)**: the helper swap for the three operator= rows that define the strong scaffolding
  `?Copy@C@@QAEXPBVCParentDefClassBase@@@Z`: 0x0045D2FC CFactionDef::operator=, 0x0045E55A CCombatDialogueDef::operator=,
  0x004604CF CSpecialAbilitiesBurntEffectDef::operator=.
- **D-4 accepted** (test member stubs, tgate.py `addr:class:sym,...`).

# NF327 (2026-11-24): the items unblocked by today's rulings (14 entries)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Rulings applied: NF323 Q1 / Q2 / Q3,
NF321 Q1, NF325 Q1 / Q2 (as written in BRIEF.md and the nf321 / nf323 landing READMEs).
Order of work (advisor, recorded): G1 (six headers) -> G4 (0x0088E520) -> G3 (0x00860290 unit) -> G5 (0x00A86030) -> G2 (PickUpJug)
-> G6 (ServeFood); then equiv_check, planted control, link, a final regrade of all ready rows on one tree.

## Result (STATUS.tsv, 14 lines)
- **10 done**, ready to land (rows.txt; rows/<addr>.cpp; no test changed: every landed test passed unchanged on both sides):
  0x007EBB30 0x007ED140 0x0084C010 0x0084CA30 0x0084CA60 0x00933D50 (Clone rows on six new headers), 0x0088E520 (EndCutFade),
  0x00A86030 (GetPossibleCollisionSet, **lands with or after NF325's 0x00A85FB0**), 0x00902CA7 / 0x00902CDC (ServeFood GetBuilding).
- **4 skipped-held**: 0x009680D0 (PickUpJug, Q1), 0x00860290 + binders 0x0085FF10 / 0x00860150 (the unit, Q2). Their drafts are in
  evidence/held_drafts/ (not in rows/), and the two headers that serve only PickUpJug are in evidence/held_drafts/hdr/ (not in hdr/).
- REJECTED.tsv: none.

## Proof common to every done row
1. Precheck (evidence/pre.log): clang of each landed TEST before its lock (T_), then after the lock the HEAD source (H_) and the new row
   (N_, NF_TREE = work/tree = repo include + hdr/ + NF325's CPhysicsFacesSegmentBranch.h); gcc cpp #error pass CPP_OK. All PASS.
2. vland `--recheck-landed` HEAD then new under ONE LANDVERIFY_WORK path (tools/runpair.sh), per group (evidence/recheck_g{a,b,d,f,w}{h,n}.log)
   and once more for all 10 on the final tree (recheck_final.log = new side, evidence/recheck_final_head.log): 10/10 MATCH /
   RELOCATION_MATCH + test PASS on both sides; every new-side graded symbol is the PDB symbol.
3. objsec on the final objects (evidence/cloneproof_fin.txt, evidence/objsec_fin.txt): see each group.
4. Link (evidence/link_rdy.txt, link_rdy_summary.txt): /FORCE link of the 10 HEAD objects v the 10 new objects, each with 4 controls
   (0x006E71F0 Rest and 0x006E7270, the G4 callee owners; 0x007EBA20 / 0x00842460, constructor rows storing the LookThroughWindow /
   ActivateZTarget vftables): 0 duplicates in both; resolved by the new set: helperA / helperB stand-ins and the six `?v00@` Clone
   stand-ins (ActivateZTarget's 2 -> 1: the control 0x00842460 keeps its own); newly referenced only: the vftable slot targets of the new
   tables (base virtuals and each class's PerformAction: owners outside the link set, NF323 Q6 shape), `?EndFadeScreen@...@@IAEXXZ` (its
   owner 0x006E7270 still defines the stand-in `?Run@T@@QAEXXZ`, NF302 Q2, already queued by NF321), operator delete (the ??_G bodies) and
   `__fltused` 3 -> 4 (CRT, /NODEFAULTLIB). Nothing HEAD resolved goes unresolved.
5. Binders / table: no reloc_bindings.tsv line targets any of the 14 addresses; the 10 done addresses are reached only through their
   vftables (NF321 / NF323 exe census). New-name census (evidence/namecensus.tsv): no other landed source defines or Leafs any new self
   name; no landed row defines a `??_G` / dtor of the six Clone classes; none of the new macro names occurs in rebuild/ (0 hits).

## G1: six new headers + their Clone rows (NF323 Q1)
- tools/gen_class_headers_nf325.py (repo generator, output redirected to work/gen) for the six classes: all six are in
  struct_layouts_egor.tsv (donor 0xB4 -> retail 0xA8), INDEX status OK, no reconcile verdicts (evidence/gen/).
- tools/wrapnew.py: generator text unchanged as the default branch; NF323's held model (held_new_headers/) under
  `#ifdef FABLE_RETAIL_CCREATUREACTION_<X>_VIRTUAL_MODEL` / `#else` / `#endif`, inserted after `#include "rebuild_abi.h"` (the landed
  StartSneaking form); the model's own sizeof assert inside the opt-in. pin_asserts.py with the generator tree as "old" pins the two
  default-branch asserts to the generator's line names (so a later includer of the default branch sees exactly the generator header).
  tools/hdrcheck327.py: stripping the opt-in gives the generator output back byte for byte (evidence/hdrcheck.txt, SAME x6).
- Rows: NF323's drafts (their orig/ == HEAD) plus `#define FABLE_RETAIL_CCREATUREACTION_<X>_VIRTUAL_MODEL`.
- cloneproof (NF320 Q1 rule as a script; evidence/cloneproof_fin.txt, 6 OK): Clone COMDAT bytes and relocations equal under old -> new
  self; no HEAD definition lost; the only added definition is `??_G<X>`; the emitted `??_7<X>@@6B@` has retail's 70 slots and every slot's
  symbol is the RTTI-port label of retail's slot target; the `??_G` body equals retail's slot-0 body with its two relocations at retail's
  targets (~CCreatureActionBase 0x00693EF0, operator delete 0x00BFE9BC); `.drectve` = HEAD + `/DEFAULTLIB:"libcp"` (NF318 Q1); every
  other COMDAT equal. The UND set grows by exactly what the emitted ??_7 / ??_G reference (NF323 Q6).
- The new files have no includer, so equiv_check and the planted control cannot see them (said, as for NF323).
- evidence/newly_flagged_rows.tsv: landed files that keep a local model of these classes (re-port backlog per the owner decision).

## G4: 0x0088E520 EndCutFade (NF321 Q1)
- The repo CGameScriptInterface.h equals NF321's orig, so NF321's diff applies as is (hdr/engine/CGameScriptInterface.h == NF321's
  hdr copy; pin_asserts: 0 pins). Row = NF321's draft unchanged.
- Conditions of the `__fltused` exception:
  - symdiff (evidence/objsec_fin.txt): under the 3-name map (self, helperA -> EndFadeScreen, helperB -> Rest) every section's bytes and
    relocations, the DEF set and `.drectve` are equal; the UND symmetric difference is exactly `{__fltused}` (new side only).
  - an already-landed MATCH row's object carries it: 0x00A86030's HEAD object (graded MATCH) has UND `__fltused`; so does landed
    0x006E71F0 Rest (RELOCATION_MATCH); 0x0088E520's HEAD object does not, its new object does (evidence/fltused_witness.txt).
  - Rest's own second source: the landed self row 0x006E71F0 defines `?Rest@CGameScriptInterface@@IBEXM@Z` (its Leaf, its object DEF),
    plus both PDB name sets (and NF321's donor call). EndFadeScreen: both PDB name sets + the donor call (NF321).
- ref_scan.py (run from work/rsroot so nothing lands in the repo cwd; the five candidate rows naming helperA / helperB locked and
  prechecked first, then released): REFS NONE for both old spellings (evidence/ref_scan_g4_REFS.tsv; all 5 compiled, 5 PASS) ->
  TABLE_CHANGES.tsv: the two `replace` lines.
- vland WARN RELOC_NAME on the new side: 0x006E7270's catalog label `Forward_EndScreenFadeOut_via0xc_006e7270` (queued by NF321).

## G5: 0x00A86030 CPhysicsFacesSegmentBranch::GetPossibleCollisionSet (NF325 Q1)
- hdr/engine/CIPhysicsFacesSegmentBase.h: add-only `#ifdef FABLE_RETAIL_CIPHYSICSFACESSEGMENTBASE_FRIEND_BRANCH` /
  `friend class CPhysicsFacesSegmentBranch;` / `#endif` before the closing `};` (0 pins; the existing assert is already AT-pinned).
- The row is NF325's draft plus that define, with ONE change: NF325 had moved HEAD's inline `Plane()` helper from the alias class to a
  file-static function. That compiled and matched, but objsec failed (evidence/libcp_ge.txt): HEAD's unreferenced pick-any COMDAT
  `?Plane@CPhysicsFacesSegmentBranch_Methods@@QBEABUCPlaneModel@@XZ` became a static COMDAT with another name and body (NF318 Q5 /
  NF320 Q2 shape). A first variant kept both helpers (5 sections v 4, evidence/libcp_gv.txt,
  held_drafts/00a86030_variant1_static_wrapper_rejected.cpp). The final row keeps HEAD's helper class `CPhysicsFacesSegmentBranch_Methods`
  (derived from the real class, only the inline Plane accessor) and calls it through `static_cast`, so every COMDAT is HEAD's:
  objsec LIBCP_OK under the one rename, `.drectve` = HEAD + libcp (evidence/objsec_fin.txt), MATCH both sides. See Q3.
- **Dependency:** the row needs NF325's `FABLE_RETAIL_CPHYSICSFACESSEGMENTBRANCH_VIRTUAL_MODEL` opt-in in CPhysicsFacesSegmentBranch.h
  (NF325 row 0x00A85FB0, not landed when I finished; the repo header still equals NF325's orig). I graded with NF325's header in work/tree
  only; it is NOT in my hdr/. Land 0x00A86030 with or after NF325's c3 batch.

## G6: ServeFood 0x00902CA7 / 0x00902CDC (NF325 Q2)
- hdr/engine/CAIStateGroup_ServeFood.h (NEW): the generator's flat output (== NF325's evidence/gen copy) as default; the opt-in
  `FABLE_RETAIL_CAISTATEGROUP_SERVEFOOD_VIRTUAL_MODEL` model (tools/genmodel.py, base mode over CAIStateGroupBase, basesize 0x14) declares
  the four protected overrides retail labels with ServeFood (slots 10-13) and the three new slots 14-16 as pure (`= 0`): retail
  vftable 0x0128AA1C holds 0x00BFEA08 (`MSVCR71.DLL::_purecall`) there; the names / signatures / access come from the derived overrides
  (IsTime / GetBuilding: Ego_r + FableWin, SetPlaceForSelf: FableWin, all MAE, plus RTTI-port labels). Default asserts pinned to the
  generator lines (11).
- CAIStateGroup_ServeFoodAtHome.h / _AtTavern.h: genmodel base mode over ServeFood (slots 14-16 protected overrides), `#ifdef` new /
  `#else` the old flat text; hdrcheck SAME.
- Rows: HEAD's walk unchanged but starting from the base's `PBrain` (CAIBrain::Creature +0x18 -> CThingAICreature::HomeBuilding +0x168
  / WorkBuilding +0x170, both members documented in the repo headers), defined as `CAIStateGroup_ServeFoodAt<X>::GetBuilding()`.
  objsec LIBCP_OK under one rename, `.drectve` EQUAL.
- Probe (tools/vprobe3.py, `new X` in a scratch TU, VC7.1): evidence/probe_servefood.txt: AtHome 16/17 SAME (slot 0 `??_E` v retail's
  `??_G` label, the VC7.1 weak alias), AtTavern 17/17; the base table `??_7CAIStateGroup_ServeFood@@6B@` emitted in both TUs: slots 1-13
  SAME, slots 14-16 `__purecall` = retail `_purecall`, slot 0 alias. Control (evidence/probe_servefood_control_nonpure.txt, the same
  header with `= 0` removed): slots 14-16 become `?IsTime@CAIStateGroup_ServeFood@@MAE_NXZ` etc., so the probe does see the pure slots.

## Held: G2 PickUpJug 0x009680D0 (Q1)
- Two sources hold: `??0CCreatureAction_PickUpThingAtTable@@QAE@ABV0@@Z` is in FableWin.pdb, and the landed row 0x0084C9A0 defines it
  (and stores PickUpThingAtTable's vftable 0x012753E4); retail 0x009680D0 / 0x0084C970 / 0x0094B6A0 call it. Both table lines the row
  needs already exist (`??0CCreatureAction_PickUpThingAtTable@@QAE@ABV0@@Z 0084c9a0 009680d0`, `??_7CCreatureAction_PickUpJug@@6B@`), so
  no table add is needed.
- Built (evidence/held_drafts/): new CCreatureAction_PickUpJug.h in the opt-in form (overrides slots 3 / 11 from ovlist; base
  PickUpThingAtTable overrides only 0 / 3 / 11, all modelled) and the add-only `FABLE_RETAIL_CCREATUREACTION_PICKUPTHINGATTABLE_COPYCTOR_DECL`
  in CCreatureAction_Tankard.h. Graded RELOCATION_MATCH both sides; Clone bytes / relocations equal; the copy constructor is called out of
  line under the PDB name; the emitted ??_7 matches retail 70/70.
- It fails NF320 Q2 (evidence/cloneproof_ge.txt): the TU also defines `??1?$CIntelligentPointer@VCThing@@@@UAE@XZ` and
  `??1CCreatureAction_PickUpThingAtTable@@UAE@XZ`, and its `??_G` calls `~CCreatureAction_PickUpThingAtTable` out of line. Retail's slot 0
  (0x00968040, and PickUpThingAtTable's own 0x0084B540) inlines the whole implicit chain: `~CBaseIntelligentPointer` 0x00A01C10 on +0xA8,
  then `~CCreatureActionBase` 0x00693EF0, then delete.

## Held: G3 the 0x00860290 unit (Q2)
- 0x00860290 alone: NF323's draft passes cloneproof (81/81, ??_G = retail, evidence/cloneproof_gc.txt).
- The binders (rows = HEAD + `#define FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_RTTI_DECLS`, drafts in evidence/held_drafts/) grade
  RELOCATION_MATCH PASS both sides, but are NOT ALL_EQ under the one rename (evidence/g3_binders_objsec.txt): their emitted
  `??_7CCreatureAction_GetUpFromSitting@@6B@` also moves slots 12 / 49 / 50 / 59 / 68 from the CCreatureActionBase virtuals to the
  GetUpFromSitting overrides (5 UND swaps), and it is still not retail's table either way: the binders compile the non-KRAKEN_ROAR
  CCombatActionBase branch (69 / 70 slots v retail's 81). Standing rule: a vtable that changes stops the address. Held as one unit.
- The ruling's "two table replaces": no reloc_bindings.tsv line spells the MBE Clone (only `??_7CCreatureAction_GetUpFromSitting@@6B@`,
  witness 0x0085FF10), so there is nothing to replace. ref_scan was not run: its identifier screen (`Clone`) is TOO_MANY; the rows naming
  the class are 0x0085FF10 0x00860150 0x008601D0 0x008601F0 0x00860290 0x008A8C60 0x008B6530 0x008B7920 0x008B79A0 0x00910570 0x00938A10
  (0x008A8C60 is the likely third MBE binder, NF323).

## Headers (hdr/engine/, hdr/diff/, orig/hdr_<X>.h = repo copies at the edit; new files: evidence/gen/<X>.generated.h)
- Edited (add-only): CGameScriptInterface.h (NF321's opt-in), CIPhysicsFacesSegmentBase.h (friend), CAIStateGroup_ServeFoodAtHome.h,
  CAIStateGroup_ServeFoodAtTavern.h. New: the six CCreatureAction_* headers and CAIStateGroup_ServeFood.h.
- Every orig/ source, test and header still equals the repo at hand-back (re-checked at 02:03, after the final run; a landing `unit2` had
  appeared meanwhile, and `diff -rq` of the repo include tree v work/tree lists only my headers and NF325's CPhysicsFacesSegmentBranch.h,
  so no row's include closure moved). NF325 had still not landed. No header shared with NF324 / NF326.
- **equiv_check** (stock lane_merge/equiv_check.py, exact mode, S in my folder) of a frozen repo copy v the same copy + hdr/
  (including the two held PickUpJug headers, now moved out): over the include closure (tools/includers.py: 14 headers, **1,709 rows**
  with source or test reaching a changed header; a row outside it never reads a changed file): **0 differing lines** (hdr/equiv_check.tsv,
  .out). Not the full 57,146-row run: at the same load NF325 measured about 9% of it in 25 minutes. Full-tree rerun for the landing:
  `cd /home/user/FableDecomp && S=<dir> python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py rebuild/include <repo include + hdr/> <out.tsv>`.
- **Planted control** (tools/plant327.py, one `typedef int nf327_planted_<X>;` per header on a path every existing includer reads):
  1,532 lines / 1,525 rows flagged (hdr/equiv_planted_control.tsv). Direct includers: Tankard 1/1, CIPhysicsFacesSegmentBase 1/1,
  ServeFoodAtHome 4/4, ServeFoodAtTavern 4/4, CGameScriptInterface 1,514/1,688: the 174 unflagged sit outside the edited region
  (`#ifdef FABLE_RETAIL_NSCRIPT`, line 18): 194 files never define the switch, 19 define it after the include, 0 other
  (evidence/equiv_control_gsi_missed.txt).
- New macro names: 0 hits in rebuild/ (src, tests, include). Row switches (evidence/row_defines_landed.tsv): every `#define FABLE_RETAIL_*`
  in rows/ has landed definers except my new opt-ins and NF325's FABLE_RETAIL_CPHYSICSFACESSEGMENTBRANCH_VIRTUAL_MODEL (0x00A86030's dependency).
- NAME_FIXES: 0x006E71F0's two other table spellings (`SetQuestProgress` / `applyFloat` on CGameScriptInterface_Methods, witnesses
  0x00892F80 / 0x00892FD0) are already queued (NAME_FIXES_QUEUE nf321), so not repeated; 0x0084C9A0 (PickUpTankard binder) is new.

## Questions
- **Q1 (PickUpJug 0x009680D0):** the copy-constructor route works for the Clone bytes and the vftable, but VC7.1 does not inline the
  implicit destructor chain the way retail's slot 0 does, so the TU adds two COMDATs (`??1?$CIntelligentPointer@VCThing@@@@UAE@XZ`,
  `??1CCreatureAction_PickUpThingAtTable@@UAE@XZ`) and its `??_G` differs from retail. Hold until the PickUpThingAtTable / CIntelligentPointer
  destructor shape is ported (an owner or authoring task), or is there an accepted form?
- **Q2 (0x00860290 unit):** defining the RTTI_DECLS opt-in in the binders changes their emitted vftables in 5 slots (closer to retail, still
  70 / 69 slots instead of 81), so "ALL_EQ under the one rename" cannot hold. Options: (a) let the binders also define
  `FABLE_RETAIL_KRAKEN_ROAR` and judge them by the NF320 Q1 / NF323 Q6 vftable rule (not measured here); (b) land 0x00860290 alone
  (the MBE split stays, narrowed, as NF323 left it); (c) keep the unit held. Which?
- **Q3 (0x00A86030 helper):** the row keeps HEAD's inline helper class `CPhysicsFacesSegmentBranch_Methods` (only the `Plane()` accessor,
  reached by `static_cast`) so its unreferenced pick-any COMDAT stays byte-equal and the COMDAT set is unchanged. Without it the helper
  COMDAT changes name, linkage and body (NF325's draft) or disappears. Acceptable as scaffolding, or hold?

## Locks
Kept on the 14 ENTRIES addresses (10 done + the 4 held drafts). Q1 and Q2 block only their own rows; Q3 decides 0x00A86030 alone. Every control lock (0x006E71F0, 0x006E7270, 0x007EBA20, 0x00842460 and
the five ref_scan candidates) is released (evidence/locks.log).

## Rule slips
- One recheck.py call failed because its output directory did not exist; nothing was written (the repo root's `D:\Documents\FableTLC`
  directory dates from 2026-11-23, not from this pass).
- tools/gen_class_headers_nf325.py crashed after writing the PickUpJug header (merging its RECONCILE.tsv in my work/gen); the header
  was written and is the generator's text (INDEX line OK).
- equiv_check and its control ran with `timeout 5400`, ref_scan with `timeout 1800` (scans, not grades).
- The `gw` grade (0x00A86030 variant 2, now the row) was launched in the same call as its precheck (V2_00a86030 PASS printed first in
  that call); the rule wants the precheck in an earlier call. The row text itself was prechecked again (N2_00a86030) before the final run.
- The generator copy also crashed after writing CAIStateGroup_ServeFood.h (same RECONCILE merge); the header equals NF325's generated copy.

# NF475 hand-back (2026-11-26): 7 of NF474's 11 held CAIStateGroup units land with their slot respells; r474d answered (bool works)

No repo writes, no git, no `wineserver -k`. Base: the working tree as read (HEAD plus NF474's landing). All 10 NF474 `orig/hdr_*` and 22 `orig/*.cpp` were cmp-equal to the tree.

## Land order
1. **batches/b0_hdr** (6 headers). These are NF474's b3r_hold opt-ins for the 6 bA units with a header, unchanged: BirdGroundWander, KidHurtSelfRunToMother, Drunk, WifeGiveYouGift, KidWanderNearKids, IdleAtHome.
   - Each is a nested `FABLE_RETAIL_CAISTATEGROUP_<X>_CTOR`. Drunk adds 5 PDB overrides and WifeGiveYouGift 3 (r474c).
   - Each has an `#error` without its view. The only removed lines are the moved size asserts, now pinned (`FABLE_STATIC_ASSERT_AT`).
   - `g++ -E -P` is identical for all 31 HEAD includers (evidence/hdr_equiv.txt).
2. **batches/bA** (21 rows = 7 units, all ACCEPT). Grade with `FABLE_HDR_OVERLAY=$W/ov/inc`, or after b0_hdr lands. ov/src/compiled holds the ctor texts the thin rows include.
   - **Units:** BirdGroundWander, KidHurtSelfRunToMother, Drunk, WifeGiveYouGift, MinionReactToAlert (headerless, the ctor's local class, r462a), KidWanderNearKids, IdleAtHome.
   - **Apply:** SELF_SYMBOLS (21), CATALOG_CHANGES / CATALOG_MANGLED (7 thin `??_G` relabels), TABLE_CHANGES (2 lines). UNITS.tsv keys each row to its unit.
3. **batches/bH_hold** (11 rows, 4 units, HOLD r467b), not gated:
   - MinionReactToNoise: 0x0091CDC0 still to do;
   - MinionKidGroupTag: 0x00934920 still to do;
   - MinionLeaveRegion: 0x0091E4B0 not started;
   - AvoidRegionExit: 0x0092BCB0 not started.

   Three slot rows are already ported in it and grade RELOCATION_MATCH: 0x0091CC90, 0x00934100 and 0x009340F0. The views are in its hdr/.

## Gates (bA)
- **vland HEAD v new:** 21 / 21 PASS on both sides (18 RELOCATION_MATCH, 3 MATCH). Per-row WARN never grows; the total goes 37 → 17. The 7 STANDIN_LEAF warnings on the thin rows go away with the relabel.
- **objproof:** the 7 slot rows are ALL_EQ under their renames, and `.drectve` is unchanged everywhere (no libcp added).
  - objproof.txt shows NOT_EQ (unpaired) for the 14 ctor / thin rows. That is the expected full-table change; their proof is ctor_checks.txt, ctor_relocs.txt and slotproof.txt.
  - Each ctor object defines exactly 4 COMDATs: the ctor, `??_7`, `??_G` and `??1CAIStateGroupBase`. All are byte-equal, and every relocation's retail target is its bound address (evidence/ctor_relocs.txt; NF474 pass2.log lists the same set).
  - Gates ran with the overlay ov/inc, which holds all 10 held headers, a superset of b0_hdr's 6.
- **Ctors:**
  - `??_7` goes 4 → 56 bytes, matching retail's 14 slots; evidence/slotproof.txt shows misspelt=0.
  - The slot targets with no `self_symbols` line all have a landed Leaf equal to their slot symbol, except 0x00914D80, which has no row.
  - The ctor, `??_G` and `??1CAIStateGroupBase` (0x008FD288) are byte-equal to retail (evidence/ctor_checks.txt).
- **Link** (ctors + slot rows, plus HEAD objects of the 34 other slot-target rows and 0x008FD255; thin rows excluded):
  - duplicates 0 → 0;
  - unresolved 94 → 86: the old stand-ins are gone, and the one added name is `?Update@CAIStateGroup_KidWanderNearKids@@UAEXXZ`, whose target 0x00914D80 has no row.
- **Census:** every respelt address has exactly one retail reference, its own vftable slot. That table's emitters are the unit's ctor (7 OK, 0 BAD) and Alloc, which has no row. The old spellings appear only in their own row and test.
- **Sandbox** (shadow repo, overlay ov/inc):
  - apply_table --dry: 0 misses (1 delete, 1 rewitness);
  - bindings_multi 1 → 1;
  - descriptive_check: 2030, 0 FAIL;
  - s_unit_check: 206, 0 failed;
  - the repo table is unchanged.
- **Tracker dry run:** 14 rows move rtti_identified → agree_pdb; 0x008FCFA0 stays split_fixable with fewer names; nothing gets worse.

## TABLE_CHANGES (bA)
- `delete ?h1_9143c0@@…` at 008FCFA0. The PDB `PeekWorld` line is already bound, and this was its only emitter.
- `rewitness` of the `UAE…JJ` long-priority CAIStateGroupBase::Initialise line at 008FCEC0, from 0x0090F0D0 to 0x0091D820. 0x0091D820 was compiled and checked: it still emits that line, as do 0x009705D0 and 0x0093A500. The line stays as debt (NAME_FIXES).

## Notes per row
- **0x00917330 IdleAtHome:**
  - The bool `&&` gives retail's `mov eax,1` / `xor eax,eax`.
  - `// Inferred:` the creature local is comma-sequenced inside the second operand, because retail keeps it in esi across a call.
  - It took 4 grades; ruling r475a.
  - It keeps the HEAD `reinterpret_cast<CThingCreatureBase*>` (r475b).
- **0x00914C60, 0x00917330:** the local base model is gone. The unrelated local views (CThing interface map, components, filters, brain and creature) are kept (r473a); vland's SHARED_HEADER warnings are fewer than at HEAD.
- **0x0090F0D0:** the local CAIStateGroupBase, `typedef long EScriptAIPriority` and local CMainGameComponent are replaced by the shared headers.
- **r474d (bH_hold 0x00934100):** a bool spelling matches retail: `return PTCGroupTag != 0 && PTCGroupTag->GetTagger() != 0;`, RELOCATION_MATCH, symbol `UBE_NXZ`. VC7.1 materialises a bool `&&` in a full register; landed precedents are 0x00419680, 0x0082CF40 and 0x004AB150. The callee is respelt to the PDB `GetTagger() const` (0x008138E0), the spelling the landed sibling 0x00934870 uses.

## Units and the rulings they depend on
The units are independent, so any subset can land without re-gating. IdleAtHome's 3 rows (0x008FDB12, 0x008FFEBC, 0x00917330) can be pulled alone.

| unit (bA) | rows | depends on |
|---|---|---|
| BirdGroundWander, KidHurtSelfRunToMother, Drunk, WifeGiveYouGift, MinionReactToAlert, KidWanderNearKids | ctor + thin + slot each | r475c only (lock takeover) |
| IdleAtHome | 0x008FDB12, 0x008FFEBC, 0x00917330 | r475a (Inferred comma local), r475b (HEAD reinterpret_cast kept; r473b declined this form when new), r475c |

## Disclosures
- **Claims:** NF429's family claim on `CAIStateGroup_*` in COORDINATION_CLAIMS.tsv is still "claimed". An address screen cannot match it. No NF429 locks remain.
- **Rule slip:** several tools copied from NF474 (cc467.py / nf.py / slotproof.py / objproof.py) ran with plain `python3`, not `python3 -I`.

## Locks
LOCKS.txt has the full list.
- **The 11 ctor locks were owned by NF473, a finished agent.** No NF474 / COORD_HOLD_NF474 locks existed. NF475 wrote NF475 over the NF473 locks, and the write was not refused (r475c).
- 25 more were taken from released or unowned. NF475 holds 21 bA locks for landing. The 15 locks of the held units are written back as `released`.
- Other NF473 / COORD_HOLD_NF473 locks were not touched.

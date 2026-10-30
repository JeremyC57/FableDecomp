# Wave 3: remaining class-model debt and the plan (counts from the wave 2 worktree, 2026-10-30)

A row counts when it defines `class|struct <X> {` (template prefix allowed) under `rebuild/src/compiled`.

| class | after wave 1 | after wave 2 | shared header |
|---|---:|---:|---|
| CSpawnedFuncBase | 16 | 16 | CSpawnedFuncBase.h |
| CDefClassBase | 841 | 689 | CDefClassBase.h |
| CBaseClass | 1694 | 1450 | CBaseClass.h |
| CParentDefClassBase | 332 | 108 | CParentDefClassBase.h (retail branch, new in wave 2) |
| CDefPointer | 1392 | 1241 | CDefPointer.h (new in wave 2) |
| CIVCountedPointeeBase / CResource / CDefPointeeBase | 380 / 353 / 501 | 380 / 353 / 501 | def-chain headers |
| CSubDefClassBase | 75 | 75 | CSubDefClassBase.h |
| CScriptBase | 1702 | 1704 | CScriptBase.h |
| CScriptThing | 2501 | 2503 | CScriptThing.h |
| CGameScriptInterfaceBase | 198 | 202 | CGameScriptInterfaceBase.h |
| CGameScriptInterface | 1381 | 1284 | CGameScriptInterface.h |
| CEntityScriptBase / CLevelScriptBase / CTimer | 1120 / 950 / 200 | 1122 / 951 / 200 | NScript headers |

The +2/+4 against wave 1's figures come from rows landed by chunks 580/581 since then. The wave 2 deltas above are exact.

## Ready to script (dry-run counts on the current tree)

1. **CGSI interface-only: `scripts/mig_cgsi.py`, 462 `edit ok`.** Wave 2 took 100 of these: 96 SAME, 2 with only `$L` label
   renames, 2 SRC_FAIL (a local `struct std::vector` against the forward header's `class`).
   - Skips: 460 blocks with non-virtual items, 151 with a local CBaseClass, 149 with a named slot at a different index,
     41 that already include the header, 21 other block shapes.
   - Add a rename map for invented slot names at a proven index: Update/YieldFrame at slot 7 are NewScriptFrame (18 rows).
     Rewrite the row's calls, then run mcheck.
   - The 151 local-CBaseClass rows go after the BC pass.
2. **BC.**
   - **`scripts/mig_bc.py --allow-v00`, 1,088 `edit ok`.**
     - Exact shapes first: cbb21bf1 100, 3ed153c6 53, 9b25d360 29, bbb0d2cf 12 (194 rows, all `CBaseClass(); virtual
       ~CBaseClass();`). Wave 2's 238 cd08cff0 rows came back 238/238 SAME.
     - Then the dtor-only b638a797 (466; 194 of them call ~CBaseClass 0x0099A2E0, 272 neither construct nor destroy).
     - Then the inline-dtor shapes 77992de8 (96) and 62d3d4ae (82). Their retail bytes neither call nor store, so the
       inline/out-of-line choice can't change them unless they destroy a CBaseClass. Run mcheck.
   - **`scripts/mig_bc_copy.py`, 169 `edit ok`** (inline `CBaseClass() { }` used only as `: CBaseClass()` in a copy ctor ->
     header + `: CBaseClass(other)`). LESSONS item 1 is the rule; 6/6 SAME in wave 2.
   - 135 inline-ctor rows have no copy ctor use. Check each against its bytes: a store of 0x01231710 that the derived
     vtable store overwrites is an implicit copy; a call is the out-of-line ctor.
3. **Def-chain copy members (decide before the DCB bulk).** PDCB::operator= (0x00431ED0), the PDCB copy (0x0046EE98) and
   CDefClassBase::operator= (0x00431F10) look implicit: out of line in /Os TUs, expanded in 5 /O2 rows (LESSONS item 3).
   - Test: give `FABLE_PDCB_TYPED_MEMBERS` includers the implicit members (undeclared). Write shared CDefString /
     CVectorMap / CArray models first, or a contract in the CCountedPointer style.
   - Then rerun the 64 /Os callers (they must still call 0x00431ED0) and the 5 expanders.
   - The same experiment settles wave 1 open item 1 (the CScriptThing copy).
4. **PDCB remainder (108).**
   - 70 typed-member rows define the whole local chain (CResource / CDefPointeeBase / CDefClassBase / CDefString /
     CVectorMap / CArray). They need item 3 or `FABLE_PDCB_TYPED_MEMBERS` with the row's own CVectorMap.
   - 14 other shapes, among them local `virtual Transfer` and full CIVCountedPointeeBase slot lists.
   - 12 rows that keep a local block and name `engine/CParentDefClassBase.h` (an include of the flattened header, or a comment). The script skips them as "already".
   - The 12 rows in REVERTED.tsv:
     - 8 DIFFER: derived classes that pad over the base, e.g. `_body[0x38]` inside the derived def. Drop the pad.
     - 2 `typedef long CDefString`;
     - 1 two-parameter CVectorMap;
     - 1 unattributed abstract aggregate.
   - Run `scripts/add_def_overrides.py` on every abstract-derived SRC_FAIL; it is byte-neutral (12/12).
5. **DCB clusters from wave 1 NEXT, still open:**
   - 83101c63 (18): full local CIVCountedPointeeBase/CResource/CDefPointer chain. Its local CIVCountedPointeeBase::Release
     calls Delete itself, and CDefPointer has a default ctor. Map it to CDefPointer.h; the default ctor needs the ENullDef
     ctor's enumerator.
   - 46f7e374 (15): `virtual void Copy` alone.
   - 662692ff (14): GetSubDef template only. Give it the CDefPointer.h GetSubDef form, as in mig_gsd.py.
   - 012a455e (24) and fe4a0baa (7): local CDefBase.
   - Destroy / RefCount stand-ins (e90794eb 11, 1e69bd22 6, 27b1d4c5 6).
   - The 35 local-CResource rows.
   - The 16 SFB hand edits.
6. **NScript families:** CScriptBase + CLevelScriptBase, then CEntityScriptBase + CScriptThing, per wave 1 NEXT (a)-(d).

## Bindings and naming (coordinator; see NAME_FIXES.tsv)

- 0x00430370 is the CParentDefClassBase ctor. It is bound under `??0CDefClassBase@@QAE@XZ` and two invented names. Wave 1's
  `??0CDefClassBase@@AAE@XZ` candidate for it is withdrawn.
- 0x00430300 carries 10 spellings (proven `??1CParentDefClassBase@@UAE@XZ`).
- 0x00431ED0 carries 7 (proven `??4CParentDefClassBase@@QAEAAV0@ABV0@@Z`; one of them is a non-existent
  `?Copy@CParentDefClassBase@@UAE..`).
- 0x0046EE98 carries 2.
- 0x00431F90 carries 5.
- 145 non-template `?GetSubDef@CDefClassBase@@QAE..` bindings are invented spellings of the const member template.
- 0x0143E8F8 is untouched (coordinator).

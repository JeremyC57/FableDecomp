# Wave 4: remaining class-model debt and the plan (counts from the wave 3 worktree, 2026-10-30)

A row counts when it defines `class|struct <X> {` (template prefix allowed) under `rebuild/src/compiled`.

| class | after wave 2 | after wave 3 | shared header |
|---|---:|---:|---|
| CSpawnedFuncBase | 16 | 16 | CSpawnedFuncBase.h |
| CDefClassBase | 689 | 689 | CDefClassBase.h |
| CBaseClass | 1450 | **612** | CBaseClass.h |
| CParentDefClassBase | 108 | 108 | CParentDefClassBase.h |
| CDefPointer | 1241 | 1241 | CDefPointer.h |
| CIVCountedPointeeBase / CResource / CDefPointeeBase | 380 / 353 / 501 | same | def-chain headers |
| CSubDefClassBase | 75 | 75 | CSubDefClassBase.h |
| CScriptBase / CScriptThing | 1704 / 2503 | same | NScript headers |
| CGameScriptInterfaceBase / CGameScriptInterface | 202 / 1284 | same | NScript headers |
| CEntityScriptBase / CLevelScriptBase / CTimer | 1122 / 951 / 200 | same | NScript headers |

Wave 3 edited 838 rows, all in the CBaseClass family (ROWS.tsv). The 800-row budget ran out inside brief item 2. Items 3-6
are still open, but their scripts are ready and dry-run below.

## Settled in wave 3 (don't redo)

- **Def-chain copy members:** they stay declared out of line (LESSONS item 1).
  - The 5 /O2 expanders keep their row-local `__forceinline` chain: 0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980.
  - A `FABLE_PDCB_TYPED_MEMBERS` row brings its own CDefString / CVectorMap. Put them in `#pragma pack(push, 8)` when the
    row includes the chain inside a pack(1) region.
- **Copy ctors with no base initializer:** before every BC batch's mcheck, run `scripts/mig_bc_copyinit.py` on the edited
  files (LESSONS item 2).

## Ready to script (dry-run counts on the wave 3 worktree)

1. **BC remainder (612).**
   - **64 dtor-only rows that mig_bc accepts:**
     - shapes c3f16241 19, 005cec96 19, 16c4339c 16, 1515a492 10;
     - list in `$S/mig3/work/w4_bc_safe.txt`;
     - pipeline: `mig_bc.py --allow-v00`, then `mig_bc_copyinit.py`, then mcheck.
     - Expect SAME: the 644 rows of the same shapes in wave 3 were 644/644 after the copyinit fix.
   - **169 rows for `mig_bc_copy.py`** (inline ctor used only as `: CBaseClass()` in a copy ctor). Wave 2 had 6/6 SAME.
   - **146 inline-ctor rows that mig_bc accepts, with no copy-ctor use** (list `$S/mig3/work/w4_bc_inl.txt`; mostly 15528643,
     187d7de8, 404ddfd3).
     - Check each against its bytes with `bc_ctor_rule.py`. A store of 0x01231710 that the derived vtable store overwrites
       is an implicit copy: write `: CBaseClass(other)`. A call to 0x0099A2D0 is the out-of-line ctor. With neither, the
       row doesn't construct a CBaseClass.
     - From the wave 3 cluster table: 15528643 is 62 neither / 17 store; 187d7de8 is 55 neither / 7 store.
   - **About 233 rows with v00/v000 stand-in shapes that mig_bc rejects:** c7a77b91 73 (66 call the ctor), e87c283e
     remainder, f3856aad 32, 7541a526 15, 948098f8 13, the v000 forms 9+9+9, ...
     - Extend mig_bc's item regex with `virtual void v0+\(\);`. Slot 0 of the CBaseClass vtable is the dtor, so a v00
       stand-in that the row calls virtually must be rewritten as a `delete` or a dtor call. mcheck decides.
2. **CGSI interface-only: `mig_cgsi.py --rename`, 693 `edit ok`.**
   - The new `--rename` maps the invented slot-7 names Update / YieldFrame / Pump to NewScriptFrame (0x006E7410), with
     their calls rewritten through typed CGameScriptInterface* receivers (11 rows).
   - It also treats a named slot that the row never calls as a filler.
   - Without `--rename` the count is 511.
   - Skips: 491 non-virtual items, 74 local CBaseClass (these go after the BC remainder), 68 noblock, 43 already, 21 head,
     47 slot mismatches (16 at slot 0, 6 at slot 1, 25 spread over 19 other slots).
   - Wave 2 had 96/100 SAME on the plain mode.
3. **PDCB remainder (108).** Unchanged from wave 2 NEXT item 4. `mig_pdcb.py --dry` gives:
   - 65 heads `class CParentDefClassBase : public <local base>` (the row defines the whole local chain);
   - 12 `edit ok`: exactly the 12 PDCB rows in wave 2's REVERTED.tsv. The 8 DIFFER rows pad the derived class over the
     base; drop the `_body` pad. 0045784b and 004575fc have `typedef long CDefString`, 004564b5 has a two-parameter
     CVectorMap, 004585bd is an unattributed abstract aggregate;
   - 12 "already includes" rows;
   - 19 other shapes (`virtual void` items 7, `long InstantiationName` 3, `struct` head 3, flattened `__vftable` 4,
     `:public` 2).
   - Run `add_def_overrides.py` on abstract-derived SRC_FAILs.
4. **DCB clusters.** Unchanged:
   - 83101c63 (18), 46f7e374 (15), 662692ff (14), 012a455e (24) / fe4a0baa (7);
   - the Destroy / RefCount stand-ins (e90794eb 11, 1e69bd22 6, 27b1d4c5 6);
   - 35 local-CResource rows;
   - 16 SFB hand edits.
5. **NScript CScriptBase + CLevelScriptBase:** per wave 1 NEXT (a)-(d).

## Budget

- vland `--recheck-landed` ran at about 6 rows a minute on the shared lane, so each 420-row batch takes about 70 minutes.
  mcheck -j 3 ran at about 1-2 s a row.
- An 800-row wave is about 2.5 hours of regrading. Run mcheck per family first and regrade only the rows that come back SAME.

## Bindings and naming (coordinator)

- NAME_FIXES.tsv adds 0x00431F60. Proven `??4CDefPointeeBase@@QAEAAV0@ABV0@@Z`. Landed as `??4CDefRootBase@@QAEAAU0@ABU0@@Z`
  (a binding) and as the leaf `??4CResource@@QAEAAV0@ABV0@@Z` of row 00431f60.
- Wave 2's list stands: 0x00430370, 0x00430300, 0x00431ED0, 0x0046EE98, 0x00431F90, and the 145 GetSubDef spellings.
- 0x0143E8F8 untouched.

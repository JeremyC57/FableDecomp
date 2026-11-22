# NF253 header overlay (2026-11-22)

Base copy in `../orig/engine/CThingCreatureBase.h` (taken from the repo tree at the start of this run; byte-equal to the repo at hand-back).
One add-only `#ifdef` block under a new member-specific macro; no existing line changed, so the default path is token-identical.

- `engine/CThingCreatureBase.h`: `FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON` -- `public: void UnsheatheMeleeWeapon();` (declaration
  only), six lines right after the NF247 `_UNSHEATHERANGEDWEAPON` block's `#endif` (retail class branch, after line 490). The block opens with
  its own `public:` (the region is public anyway; the label keeps it public whatever precedes it). Evidence: Ego_r.pdb / FableWin.pdb /
  Ego_d.pdb ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ (raw grep 1 / 5 / 5; name sets: Ego_r 0x00540810, FableWin 0x01D95690); Ego_r
  donor order UnsheatheWeapon 0x00540710 / UnsheatheMeleeWeapon 0x00540810 = retail 0x006648D0 / 0x006649C0; retail call sites 0x00890CE7,
  0x00664A67, 0x00666842, 0x008B7C37, 0x0096B61C (callers.py).
- pin_asserts.py (repo include v this overlay): `pinned 0` (every FABLE_STATIC_ASSERT in the header is already an `_AT` pin).
- equiv_check.py, whole tree (repo rebuild/include v a hard-link copy `cp -al` with only this file replaced by `cp --remove-destination`):
  57,125 landed rows (sources + tests) checked, 0 differing (evidence/equiv3.log; the repo file was checked unchanged afterwards).
- Includers: 999 landed rows reach the header (tools/incl.py, transitive); none defines the new macro, so the opt-in changes none of them.
- Users: only the hand-back rows 0x00890C80 and 0x006649C0 define the macro.

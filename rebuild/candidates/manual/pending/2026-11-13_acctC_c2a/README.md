# acctC lane, hand-back c2a (2026-11-13): 1 row matched, 5,126 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x00651B04 | NPlayerGui::CDrawAbilityButtons::Draw | 5,126 | RELOCATION_MATCH | PASS |

Re-verified by the lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`, overlay =
only the 13 headers the row includes; `coordverify.log`). No flags. Real length. No oracle file here.

## What closed it (from DIFFER(5126v5126), 26 hunks; about 130 grades)
- The landed DrawButton 0x00645099 body defined out of line in the TU (its position temporaries no longer escape).
- The interact-border loop sits AFTER `if (normal) { ... }`, not inside it (retail's jump at 0x0065260B lands on 0x006528DA): this is
  a behaviour fact too -- the border is drawn in special mode as well.
- Three named references (`offset`, `buttonDef`, `effectPos`) at the operand-order sites.

## Review item (accepted by the lane as an inferred statement; flag for the landing record)
The middle-ring position is written `C2DVector(C2DVector(GFAnchorToBottomRight(ringsPos) + def.ABXYMiddleOffset))`. Retail has a
second vector copy at that site (the 6 bytes the old draft was short); the outer functional cast is the copy VC7.1 does not elide.
It is plain portable C++ and cannot change the result; the original wording of that copy (probably a by-value helper or a named
temporary) is unproven. Respell it if a natural form is found.

## Names
`NAME_FIXES.tsv`: 0x004375E0 CPlayerGui::GetAbilityButtonScale, 0x00625930 CTCQuickAccessMenu::IsActive, 0x0072C430
CTCSpecialAbilities::IsAbilityAvailable, 0x005DA7D0 CTCWeapon::GetWeaponType (the four RELOC_NAME warnings), 0x00A03B30
~CInputEvent, 0x00B8D090 / 0x00B8D0A0 Start / EndOcclusionTest. Descriptive: `HUDRingAlpha` (0x01375744).

## Headers (`hdrdiff/`: unified diffs against rebuild/include at 55262db83; files named NEW_* have no repo counterpart)
Additions under opt-ins only (`hdrdiff/README.md` lists them; it also lists opt-ins of the row still in progress, which are not in
these diffs unless the same header carries them). Some `FABLE_STATIC_ASSERT` lines move: run `lane_merge/pin_asserts.py`, then
`equiv_check.py` / `recheck_rows.py` when applying. Open naming points the agent raised: 0x00BF0B20 is bound to
SetupOutlineStencilStates by the repo opt-in `_OUTLINESTENCILSTATES` while the old draft's evidence says SetupOutlineFillStates
(called for pass 13); chunk 2060's `_MESHRENDER` content was moved to a new `_MESHPRIMITIVE` name because the repo now uses
`_MESHRENDER` for other content.

## Still open in this chunk
`open_00bb3e80/00bb3e80.cpp`: 0x00BB3E80 RenderClothPrimitive, DIFFER(4540v4540), three real sites left (0x00BB49F2, 0x00BB4C15,
0x00BB4C5C); continued by agent C3. NOTES.md has everything tried.

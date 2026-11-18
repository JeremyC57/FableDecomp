# NF81 header overlay (round 7)

Six repo headers are overlaid, plus one new header. Base copies are in `../orig/`; each was byte-identical to the repo header when
this was written (`cmp`). Every change is an `#ifdef` opt-in with a member-specific macro. Without the opt-in, each header gives the
same tokens as the repo copy, with two exceptions:
- `candidates/world_map_theme.h`: the existing `struct C2DCoordI` now sits in the `#else` arm of its opt-in.
- `engine/CGameMap.h`: its two asserts were pinned by pin_asserts.py (below).
No other agent (NF79, NF80) overlays any of these files.

| Header (copy of rebuild/include/...) | Opt-in | Used by | What it adds |
|---|---|---|---|
| engine/CMap.h | `FABLE_RETAIL_CMAP_ENGINETHEMEAT` | 0x0081EE60, 0x0081EFC0, 0x0081F090, 0x0081F170, 0x0063A970, 0x0063A9E0, 0x0063AA50, 0x0063AAB0, 0x0063EB20 | `enum EThemePaintType { THEME_PAINT_ENGINE = 0, THEME_PAINT_ATMOS = 1 }`, placed before the class. The enumerator names are DESCRIPTIVE: neither PDB name set has them. It also adds the four protected per-cell theme accessors (SetEngineThemeAt IAE, SetEngineBlendAt IAE, GetEngineThemeAt IBE, GetEngineBlendAt IBE), with both PDBs' spellings and Ego_d parameter names. Declarations only. |
| engine/CGameMap.h | `FABLE_RETAIL_CGAMEMAPCELL_ATMOSGETTERS` | 0x0081F090, 0x0081F170 | CGameMapCell gets two getters. `GetAtmosTheme(long) const` is inline (FableWin.pdb 0x01B50F50; retail reads AtmosThemes[index] in place). `GetAtmosBlend(long) const` is declared only (Ego_r 0x006F3800 / FableWin 0x024A07A0; out of line at the landed 0x006B74E0). |
| engine/CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_THEMEATBLOCK` | 0x0063A970, 0x0063A9E0, 0x0063AA50, 0x0063AAB0 | The four public block accessors SetThemeAtBlock, SetThemeBlendAtBlock, GetThemeAtBlock and GetThemeBlendAtBlock, with the ABVC2DCoordI spellings both PDBs give. Declarations only. It needs engine/CMap.h with ENGINETHEMEAT included first. |
| candidates/world_map_theme.h | `FABLE_RETAIL_WORLDMAPTHEME_C2DCOORDI_SHARED` | the tests of 0x0063A970 / 0x0063A9E0 / 0x0063AA50 / 0x0063AAB0 | It includes engine/C2DCoordI.h (class, X / Y) instead of the local `struct C2DCoordI`, so that the legacy-header tests link against the rows' ABV symbols. No row uses it. |
| engine/CHitParameters.h | `FABLE_RETAIL_CHITPARAMETERS_CLEARALLDAMAGE` | 0x00868A40 | Declares `void ClearAllDamage();` (public; Ego_r 0x0083EC50 / FableWin 0x02810E60). It needs `_DAMAGEMAP`. |
| engine/CPersistTraits.h | `FABLE_RETAIL_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT` | 0x006DC350, 0x006DB060 (defines the writer) | `template<> class CPersistTraits<CComboMultiplierData>` with `DefaultVal`, `CPersistTraits():DefaultVal() {}` and the declared writer `TransferBinaryOut(CDataOutputStream&, const CComboMultiplierData&)`. That writer is Ego_r 0x0058ACE0 / FableWin 0x01F0ADD0 (retail 0x006DB060). The specialisation follows the CWoundMorphs pattern. Do not define this opt-in in a TU that declares a member specialisation of the primary template for CComboMultiplierData (0x006DCFA0 does). |
| engine/CCombatAction_StrikeResponseJackOfBladesIntoRush_GetValidRushPosition.h | NEW file (only rows that include it see it); `FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH_GETVALIDRUSHPOSITION_DEFINE` | 0x008C1600 (with _DEFINE), 0x008C2550 | The one text of GetValidRushPosition (0x008C1600), moved unchanged from the landed 0x008C1600 row together with its CIsOutsideRadius predicate (descriptive) and the STLport remove_if declaration. Without _DEFINE it is `inline __declspec(noinline)`: a COMDAT that is never expanded, the engine/CMap.h GETSTRONGESTENGINETHEMEAT_BODY pattern. With _DEFINE it is the ordinary out-of-line definition. |

**pin_asserts.py** (base `/home/user/FableDecomp/rebuild/include`) was run after every edit. It pinned 2 asserts in total, both in
engine/CGameMap.h: `FABLE_STATIC_ASSERT(sizeof(CGameMapCell) == 7)` became `FABLE_STATIC_ASSERT_AT(49, ...)` and
`FABLE_STATIC_ASSERT(sizeof(CGameMap) == 0x24)` became `FABLE_STATIC_ASSERT_AT(50, ...)`. Those are their original lines.

The other five overlaid headers pinned 0:
- engine/CMap.h (171), engine/CHitParameters.h (102..111 and 236) and world_map_theme.h (101) were already `_AT`.
- engine/CWorldMap.h and engine/CPersistTraits.h have no assert below the edit.

**Link notes for the coordinator:**
- GetValidRushPosition is emitted twice. 0x008C2550's object carries it as a COMDAT, 761 bytes and byte-identical to the 0x008C1600
  object's function, because 0x008C2550 now also defines `FABLE_RETAIL_C3DVECTOR_PACKED`. 0x008C1600 carries the ordinary definition.
  A whole-program link that takes both objects may need the 0x008C1600 definition marked inline too. That is the same situation as
  engine/CMap.h's GetStrongestEngineThemeAt and NF78's BOOL_BINARYIN_NOINLINE.
- 0x009EE540 and 0x004EBB1C also emit COMDATs owned by other rows: DeleteData 0x009EDC50 and ~_Rb_tree 0x004E81E4. Both were checked
  byte-identical to their owner rows' functions.
- With ENGINETHEMEAT, `EThemePaintType` exists in two headers: here and in the legacy candidates/world_map_theme.h (`EThemePaintType_Default` / `_Replace`).
  - No TU can include both, because each defines CMap.
  - After this pass, no landed row needs world_map_theme.h. Its remaining users are the CMap and CWorldMap tests and the four stale
    `rebuild/src/compiled/00/81/*.pre.cpp` scratch files.

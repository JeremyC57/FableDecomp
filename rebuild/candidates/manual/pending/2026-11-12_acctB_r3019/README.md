# acctB hand-back, row 0x006D1250 CTCWife::CheckAppearanceChanges: matched (was parked at 1629v1608)

| address | result | notes |
|---|---|---|
| 0x006D1250 `?CheckAppearanceChanges@CTCWife@@AAEXAAVCThingPlayerCreature@@@Z` | RELOCATION_MATCH 1608v1608, test PASS (g++ and VC7.1) | `// Flags: /GS`. 42 grades this pass (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win; one `WARN RELOC_CONFLICT` (below).

## What closed it

1. **Fatness tail:** both category indices as named locals before `GetFrame`
   (`long noticed = change > 0.0f ? 10 : 11; long cleared = ...`): retail's two tests ahead of the call.
2. **Tattoo lookup as a named iterator** (`find` before `end()`, retail 0x006D1734..0x006D173D): gave the frame.
3. **Last 11 instructions:** the STLport bodies visible in the TU: the insert chain through the existing opt-ins
   `FABLE_RETAIL_STLP_MAP` + `FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY`, and the const `find<long>` visible but
   called out of line (new opt-in `FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE`).

## Review items

- **noinline device on `find`** (`// Inline-budget:` line; EXTRA_RULES preference 3; the repo already uses the same
  device). Retail calls 0x00616580 at all three sites while it expands the LowerBound sites, whose inline scores
  (63 / 60) sit inside find's (88 / 68 / 60), so no `/d2inlT<N>` separates them; the control-flow spellings tried
  are listed in `NOTES.md`.
- **RELOC_CONFLICT:** `??$find@J@?$_Rb_tree@JJ...` binds 0x00616580 here and 0x00B2EB20 in landed 0x00B2D9F0.
  Retail holds two byte-identical copies of this instance; needs the usual decision at landing.
- **`NAME_FIXES.tsv`:** 0x00616580 is that STLport `find`; reloc_bindings has `?find@CAnimSet@@...`, a local
  stand-in from landed 0x0075D8B0.
- **FITTED:** the `noticed` / `cleared` locals. **PROVEN:** find-before-end at the tattoo site; the visible find and
  insert bodies (their out-of-line copies equal retail 0x00616580 and 0x00512DA0).
- `EWifeAppearanceCategories` values are written as casts: no name set has the enumerators.
- **Headers:** chunk 2020's overlay plus one opt-in branch in `stlport/fable_map.h`; default text unchanged.

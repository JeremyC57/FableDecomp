# Landing 2026-11-08f: chunk 855's three matches

Chunk 855 (expert agent, 97-157 grades per row) matched 3 of 8 rows; all three landed RELOCATION_MATCH + PASS (read-only vland grade with a
trimmed overlay first, then `--land` on the repo headers). Hand-back: `pending/2026-11-08_linux_hand855/`.

| Row | Grade | Note |
|---|---|---|
| 0x00CA2BB0 floor1_encode | **RELOCATION_MATCH + PASS, landed** | libvorbis floor1.c (`/TC /Gr`), the Codex pass-12 verified-pending source unchanged; was catalogued `CEnvironment_SerializeTheme`. vland WARN RELOC_CONFLICT `@ilog@4` -> 0x00CA1790 here, 0x00C9FE70 in landed 0x00C9FEA0: two file-static `ilog`s in different C TUs (expected) |
| 0x00490BE0 CSubtitleRenderer::GetGraphicFromEmbeddedTag | **RELOCATION_MATCH + PASS, landed** | was `ProcessEmbeddedGraphicTags_ResolveButtonName`; binds `CCharString::GetAsUpperCase` 0x0099EE20 (see names) |
| 0x0052F900 NUISystem::CComponent::UpdateColour | **RELOCATION_MATCH + PASS, landed** | was `GameAnimation_UpdatePosition`; slot 40 of the CSprite vtable 0x0122F5D4; `CRGBFloatColour::Clamp` must be `__forceinline` |

- **Headers landed** (opt-in; `coordinator/hdrcheck2.py` printed `default=same` for all four edited headers, and no landed file defines the new
  macros): `fable_string.h` FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE, `engine/CSubtitleRenderer.h` FABLE_RETAIL_CSUBTITLERENDERER_GRAPHICTAG,
  `engine/CPlayerGui.h` FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF, `engine/CRGBFloatColour.h` FABLE_RETAIL_CRGBFLOATCOLOUR_CLAMP; new
  `engine/CPlayerGuiDef.h` (gen_class_headers.py output + opt-in FABLE_RETAIL_CPLAYERGUIDEF_SCRIPTSPRITES). The parked rows' header opt-ins
  (CPixelFormat, CWorldMap, CMap, CGameDefinitionManager, stlport, the volume-texture family, CEngineThemeDef, CMinimapData, ...) stay in the
  pending dir until those rows land.
- **Names:** `ghidra_out/labels_landing_2026-11-08f.tsv` (the three rows + five parked rows proven from Ego_r + FableWin). Queue `land0811f`
  (PRIORITY): 0x0099EE20 is the PDB `GetAsUpperCase` and 0x0099ED10 `GetAsLowerCase`, landed as `ToUpper` / `ToLower` with 10 / 12 landed
  callers; 0x00490BE0 already binds GetAsUpperCase, so 0x0099EE20 has two bound spellings until the respell lands. Catalog kept at the landed
  ToUpper / ToLower until then.
- **Catalog sync:** as in landing_e, only this landing's addresses were kept from `tools/sync_catalog_names.py` (the 4,284-name backlog stays unreviewed).
- `bindings_multi_check.py` prints 1, the pre-existing NullCoord (same at HEAD).

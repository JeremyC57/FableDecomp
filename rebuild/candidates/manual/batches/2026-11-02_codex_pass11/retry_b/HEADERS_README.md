# retry11b header overlay

This overlay sits on top of `rebuild/include` as it is after commit d462c662a, which landed the cons11 consolidation.
Grade with `FABLE_HDR_OVERLAY=$S/w22/retry11b/hdr`.

**Default-path proof:** `python3 $S/w22/hdrcheck.py $S/w22/retry11b/hdr` reports `default=same` for both changed existing headers.

No landed source and no repo header defines the two new macros (checked with grep over `rebuild/src` and `rebuild/include`).

## Changed existing headers (default unchanged)

| Header | Macro | Change | Evidence |
|---|---|---|---|
| engine/C2DVector.h | `FABLE_RETAIL_C2DVECTOR` (new) | Opt-in branch that declares the class under `#pragma pack(push, 1)`. The branch also adds the lib_coord.hpp arithmetic Mass uses: unary `-`, `*(float)`, `+=` and `Dot`. | See below. Coordinator decision needed (see notes). |
| engine/NUISystem_CUIDef.h | `FABLE_RETAIL_CUIDEF` (new) | Whole-class opt-in. Every composite member is typed at the same offsets: `std::vector`/`std::map` from `stlport/fable_containers.h` and `fable_map.h`, `C2DVector`, `C2DBoxF`, `C2DBoxI`, `C3DVector` and the enums. Static asserts check sizeof 0x228 and PositionOffset +0x142. The branch defines `FABLE_RETAIL_C2DVECTOR` if it is not already set. | Row 0x00631C60 transfers every member, and vland passes. |

Evidence for the C2DVector packing:
- Retail reads PositionOffset at +0x142 and +0x146 in 0x00631C60.
- Ego_r `struct_layouts_egor.tsv` places CUIDef::PositionOffset at 350 (0x15E) and COldFileFormatThingBrushInfo::PositionOffset at 17.
- Codex's raw C2DVector TPI records (Ego_r 0x9ebe, FableWin 0x9b09) carry the packed property bit.

### Enums in the CUIDef branch

- **NUISystem enums:** ETableExpansionTypes, ETextAlignement and ETableSprites. Their enumerators come from Codex's raw Ego_r enum records (`rows/0052cf40/identity/UI_DEPENDENCIES_PDB.json`).
- **Global enums:** EEngineGraphicType and EEngineSprite2DFlag are declared with no enumerators, as landed rows already do. No enum record for them is on this lane, so this is debt. Move them to their own header when one exists.

## New headers

| Header | Source | Note |
|---|---|---|
| engine/ErrorDialog.h | Codex row 0099dcb0 | DisplayErrorDialog globals and the ERRORARRAYSTRUCT. Restates the Win32 declarations it needs, because the lane has no windows.h. |
| engine/ConfigDetection.h | Codex row 0099dcb0 | ConfigDetection / CONFIGTABLE / SOUND_DEVICE layouts. These are different classes from the ones `rebuild/include/fable_config_detection.h` declares (GFConfigDetection entry points and globals), so there is no second model. It does define a fourth `struct _GUID`; CSystemManager.h, CBaseVideoRenderer.h and EAX40.h each define one too. That is debt: one guiddef model is needed. |
| engine/TRetVoid.h | Codex row 00e137b0 | Empty class with `Return()`, FableWin TPI. Used only by the parked 00e137b0 shape. |
| engine/Mass.h | Codex row 008eec70 | Mass layout (Ego_r TPI 30d33). Used only by the parked 008eec70 row, which now takes C2DVector from `FABLE_RETAIL_C2DVECTOR` instead of Codex's `FABLE_RETAIL_C2DVECTOR_MASS`. |

## Not adopted

- Codex's per-feature macros (`FABLE_RETAIL_TRAJECTORY`, `_C2DVECTOR_MASS`, `_SUBTITLE`, `_NSHAPE`, `_SHAPE_*`, `_MINIMAP_TRAITS`, `_NAVIGATION_UPDATE` and the others).
- Codex's `engine/CCharString.h` and `stlport46/*`.

The parked rows that were graded in Codex's selected context still use some of these. Each `differ/` grade line says which.

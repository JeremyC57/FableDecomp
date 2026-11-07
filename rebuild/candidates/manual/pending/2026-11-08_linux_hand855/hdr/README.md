# Chunk 855 header overlay

Every edit to an existing repo header is an additive opt-in (`#ifdef FABLE_RETAIL_*`): with the macro undefined the
preprocessed text is the repo's, so landed includers keep their bytes. Rechecked read-only with this overlay: all 156 landed
includers of the seven edited engine headers (CWorldMap, CMap, CPixelFormat, CPlayerGui, CRGBFloatColour, CSubtitleRenderer,
CGameDefinitionManager) grade 15 MATCH + 141 RELOCATION_MATCH, tests PASS (SIBLINGS.tsv). Diffed against the repo on 2026-11-07.

## Edited repo headers (opt-in branches)
| header | macro | used by | change / evidence |
|---|---|---|---|
| fable_string.h | FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE | 0x00490BE0 (matched) | declares `CCharString GetAsUpperCase() const` 0x0099EE20 (Ego_r 0x00AFA710 / FableWin 0x02F61680; see NAME_FIXES.tsv) |
| engine/CSubtitleRenderer.h | FABLE_RETAIL_CSUBTITLERENDERER_GRAPHICTAG | 0x00490BE0 | private const `GetGraphicFromEmbeddedTag(const CWideString&)` declaration (Ego_r + FableWin), forward CWideString |
| engine/CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF | 0x00490BE0 | static __fastcall `PeekGuiDef()` 0x00438570 (Ego_r 0x0044B230 / FableWin 0x01A80EA0) |
| engine/CRGBFloatColour.h | FABLE_RETAIL_CRGBFLOATCOLOUR_CLAMP | 0x0052F900 (matched) | `__forceinline void Clamp()` (FableWin ?Clamp@CRGBFloatColour@@QAEXXZ; retail 0x0052FCF7..0x0052FDC7 inline; plain inline is called out of line) |
| engine/CPixelFormat.h | FABLE_RETAIL_CPIXELFORMAT_INIT | 0x00A68DE0 (parked) | ctor from CPixelFormatInit, Initialise 0x009E3640, GetByteColourDepth 0x009E3800, IsCompressed 0x009E3940 (Ego_r/FableWin) |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_COORDCONVERT | 0x00642AA0 (parked) | ConvertLocalCoordsToWorldMapCoords 0x004FC4A0 / ConvertWorldMapCoordsToLocalCoords 0x004FC3C0 (Ego_r/FableWin) |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_STATICMAPVISIBILITY | 0x00507070 (parked) | chunk 854's opt-in (start/hdr854), kept as is: BuildStaticMapGenerationVisibilityInfo declaration |
| engine/CGameDefinitionManager.h | FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF | 0x00507070 (parked) | chunk 854's opt-in: PeekGlobalDef / PGlobalDef +0xD0 (names descriptive, no PDB record) |
| engine/CMap.h | FABLE_RETAIL_CMAP_WORLDEXTENTS | 0x00642AA0 (parked) | inline GetWorldPosition / GetWorldSizeX / GetWorldSizeY (Ego_r/FableWin; read in place at 0x00642C74..0x00642CA2) |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_FASTCALL_DESTROY_AUX | 0x00507070 (parked) | non-trivial `__destroy_aux` as `__fastcall` (retail 0x00519EB0 / 0x0051AC70 take first/last in ecx/edx; landed 0x0051C110 models the same) |
| stlport/fable_map.h | FABLE_RETAIL_STLP_MAP_OUTOFLINE_DTOR | 0x00D0F640 (parked) | declared-only `~map()` so the TU calls the COMDAT (retail 0x00CDBFB0 for map<CCharString, CScriptGameResourceObjectScriptedThingBase>) |

Root copies: fable_bank_file.h, fable_boot.h, fable_definition_table.h, fable_filesystem.h, fable_game.h, fable_game_text_bank.h,
fable_language.h, fable_profile.h, fable_registry.h, fable_render2d_vertex_queue.h, fable_resource_directories.h,
fable_retail_banks.h, fable_system.h, fable_user_profile.h, fable_video_system.h, fable_window_title.h, fable_write_permissions.h are
byte-identical copies of the repo's root headers, present only so that their `#include "fable_string.h"` resolves to this overlay's
copy (MSVC searches the includer's directory first). Do not land them; land only the fable_string.h edit.

## New headers (no repo header existed)
| header | used by | content / evidence |
|---|---|---|
| engine/CPlayerGuiDef.h | 0x00490BE0 | gen_class_headers.py output (retail STLport layout 0xAB4) + opt-in FABLE_RETAIL_CPLAYERGUIDEF_SCRIPTSPRITES (`std::map<CCharString,long> ScriptSprites` +0x934) and a retail layout check line for the Controller*Graphic members +0x648..+0x690 |
| engine/C3DExtentsI.h | 0x00A68DE0 | C3DExtentsI : C2DExtentsI + Z (Ego_r 12 bytes, ctors inline) |
| engine/CPixelFormatInit.h | 0x00A68DE0 | pack(1) 6 bytes, Type : 4, Linear : 1, ColourDepth, R/G/B/A bits (Ego_r) |
| engine/CVolumeTextureFileInfo.h | 0x00A68DE0 | pack(1) 0x15 bytes (one 0x15-byte read; Ego_r) |
| engine/EVolumePool.h | 0x00A68DE0 | the enum type only (enumerators not in the extracted PDB data; descriptive) |
| engine/CVolumeLockedBox.h | 0x00A68DE0 | 24 bytes (Dimensions, RowPitch, SlicePitch, PMem), Lock's by-value result |
| engine/IDirect3DVolumeTexture9.h | 0x00A68DE0 | d3d9 COM slot list (GetLevelCount 13, GetLevelDesc 17, LockBox 19, UnlockBox 20) + D3DVOLUME_DESC |
| engine/CVolumeTexture.h | 0x00A68DE0 | CBaseClass base (vtable 0x0129DC18), PD3DVolumeTexture +4 / ByteLength +8 / PData +0xC; Initialise 0x00A68370, GetLevelByteLength 0x00A681F0, Lock 0x00A685E0, inline GetNoLevels / GetDimensions / GetPixelFormat / Unlock |
| engine/CEngineThemeDef.h | 0x00642AA0 | opt-in FABLE_RETAIL_CENGINETHEMEDEF: EMinimapThemeType MinimapTheme +0x90 (def schema retail offset 144); the quarantined generated header is not usable |
| engine/CMinimapData.h | 0x00642AA0 | retail 0x3C layout as the landed CMinimapData rows 0x006427F0..0x00642A30 model it; inline SetMinimapThemeValue; opt-in PEEKDEFS PeekDefinitionManager (inline, CMainGameComponent 0x00415FBC) |
| engine/CThingFilter_OverlappingArea.h | 0x00642AA0 | `const C2DBoxI* Area` +0 (Ego_r 4 bytes) |

The parked rows' headers (everything but the 0x00490BE0 / 0x0052F900 lines) are only needed once those rows land.

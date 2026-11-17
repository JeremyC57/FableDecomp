# NF47 header overlay (round 7)

Two headers, each a copy of the CURRENT repo header with one `#ifdef` opt-in added. Nothing else changes: without the macro
each file is token-identical to the repo header (`diff` shows only added lines; the CNewFrontendGameComponent `#else` branch is
the repo's own two lines, unchanged).

| Overlay file | Repo header copied | Opt-in added | Used by |
|---|---|---|---|
| engine/CMiniMapDisplay.h | rebuild/include/engine/CMiniMapDisplay.h | `FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISEONREGIONLOAD`: declarations of `?InitialiseOnRegionLoad@CMiniMapDisplay@@QAEXABVCCharString@@@Z` (0x0082A180) and `?InitialiseMiniMapFileLoading_Region@CMiniMapDisplay@@QAEXABVCRegion@@ABVCCharString@@_N@Z` (0x00829D90), both in Ego_r.pdb and FableWin.pdb. Declarations only, inside the existing class body. | rows/0082a180.cpp |
| engine/CNewFrontendGameComponent.h | rebuild/include/engine/CNewFrontendGameComponent.h | `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT`, nested in the header's existing `#else` (non-FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN) branch: splits the opaque `_pad_0x6c[0x148-0x6c]` into `_pad_0x6c` / `bool XMVCodeLoaded` (+0xB1) / `_pad_0xb2`, declares the private `InitialiseDefs()` (`AAE`, 0x0042F722) and the public `virtual void Init()` (`UAE`, 0x0042F75E; overrides CGameComponent::Init, so the vtable layout does not change). Size stays 0x148. | rows/0042f75e.cpp |

Macro names follow `FABLE_RETAIL_<CLASS>_<MEMBER>` so the coordinator can merge them with NF46 / NF48 overlays (neither of them
overlays these two headers).

All other opt-ins the rows use (FABLE_RETAIL_CMAINGAMECOMPONENT_*, FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC / _ENTRYQUERY,
FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE, FABLE_RETAIL_CWORLD_MESH_BANK, ...) already exist in the repo headers.

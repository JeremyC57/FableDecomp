# NF160 header overlay (hdr/)

Four overlaid headers. Each was copied from the repo working tree (rebuild/include/...) at the start of the pass; orig/ holds the
untouched base copy, and `cmp orig/<h> rebuild/include/<h>` still reports identical for all four at hand-back time. Every addition is
declaration-only, inside a new member-specific `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>` block. No layout change, no existing member
changed, no FABLE_FORCEINLINE. With none of the new macros defined, each header is token-identical to its base apart from the
pin_asserts.py line pins (`FABLE_STATIC_ASSERT(...)` -> `FABLE_STATIC_ASSERT_AT(<line>, ...)`).

The other two agents of this wave (NF158, NF159) overlay none of these headers (checked their hdr/ trees: CPSCBase.h,
CParticleEmitterManager.h, CPSCUpdateNormal.h, CTCInventoryWeapons.h, CTCTavernGame*.h).

| Header (base = repo copy) | Opt-ins added | Address / evidence | Pins |
|---|---|---|---|
| engine/CEngineWorldMap.h | FABLE_RETAIL_CENGINEWORLDMAP_ONGAMEMAPUNLOADED | `OnGameMapUnloaded(long)` 0x00B40230, Ego_r 0x009E05B0 + FableWin 0x02D61810 | 0 new (its existing asserts are unchanged) |
| | FABLE_RETAIL_CENGINEWORLDMAP_SETMAPVISIBILITY | `SetMapVisibility(long, bool)` 0x00B42390, Ego_r 0x009E2500 + FableWin 0x02D625D0 | |
| | FABLE_RETAIL_CENGINEWORLDMAP_SETMAPVISIBILITYALLMAPS | `SetMapVisibilityAllMaps(bool)` 0x00B423B0, Ego_r 0x009E2520 + FableWin 0x02D62820 | |
| engine/CEngineMap.h | FABLE_RETAIL_CENGINEMAP_ONGAMEMAPUNLOADED | `OnGameMapUnloaded()` (no retail out-of-line copy; expanded in 0x00B40230), Ego_r 0x009DD2B0 + FableWin 0x02D5C630 | 0 |
| | FABLE_RETAIL_CENGINEMAP_SETMAPVISIBILITY | `SetMapVisibility(std::list<CEngineMap*>&, bool)` 0x00B41DB0, Ego_r 0x009E2440 + FableWin 0x02D5B370; needs stlport/fable_containers.h included first | |
| engine/_quarantine/CEngineLandscapeMap.h | FABLE_RETAIL_CENGINELANDSCAPEMAP_ONGAMEMAPUNLOADED | `OnGameMapUnloaded()` 0x00BDBFF0, Ego_r 0x009B4730 + FableWin 0x02CC75D0 | 26 (its existing sizeof / offsetof asserts, line-pinned) |
| | FABLE_RETAIL_CENGINELANDSCAPEMAP_ONMAPVISIBILITYCHANGED | `OnMapVisibilityChanged(bool)` 0x00BDC3C0, Ego_r 0x009B5150 + FableWin 0x02CC7720 | |
| engine/CLocalDetailCacheMap.h | FABLE_RETAIL_CLOCALDETAILCACHEMAP_ONMAPVISIBILITYCHANGED | `OnMapVisibilityChanged(bool)` 0x00BDE160, Ego_r 0x00A8FAB0 + FableWin 0x02E3B3A0 | 0 |

## Includers

Landed includers, from `grep -rlE '<header name>' rebuild/src/compiled`:

| Header | Landed rows | Tests |
|---|---|---|
| CEngineWorldMap.h | 46 | 4 |
| CEngineMap.h | 21 | 3 |
| CEngineLandscapeMap.h | 10 | 0 |
| CLocalDetailCacheMap.h | 17 | 1 |

`grep -rlE` for the eight new macro names over rebuild/ and tools/ finds no hit. So no landed row or test defines them, and these
opt-ins change none of the landed includers: those rows compile the default path, which is token-identical.

The new macros are defined only by the four hand-back rows below:
- rows/00b40230.cpp: CENGINEWORLDMAP_ONGAMEMAPUNLOADED, CENGINEMAP_ONGAMEMAPUNLOADED, CENGINELANDSCAPEMAP_ONGAMEMAPUNLOADED;
- rows/00b42390.cpp: CENGINEWORLDMAP_SETMAPVISIBILITY, CENGINEMAP_SETMAPVISIBILITY;
- rows/00b423b0.cpp: CENGINEWORLDMAP_SETMAPVISIBILITYALLMAPS, CENGINEMAP_SETMAPVISIBILITY;
- rows/00b41db0.cpp: CENGINEMAP_SETMAPVISIBILITY, CENGINELANDSCAPEMAP_ONMAPVISIBILITYCHANGED, CLOCALDETAILCACHEMAP_ONMAPVISIBILITYCHANGED.

Batch b3 and the final recheck_final.log (all 41 rows, through this overlay) both grade these four RELOCATION_MATCH, and every test PASSes.
No hand-back of NF154 to NF159 overlays any of these four headers, so the orig/ bases are current.

## Not overlaid (opt-ins that already existed and were used as they are)

The rows also use these existing opt-ins, unchanged:
- engine/CInputEvent.h: FABLE_RETAIL_CINPUTEVENT_DEVICETYPE, FABLE_RETAIL_CINPUTEVENT_ACCESSORS;
- engine/CTexture.h: FABLE_RETAIL_CTEXTURE;
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL;
- the StopProgress set (FABLE_RETAIL_ prefix): CCHARSTRING, CWIDESTRING, CTEXTURE, CPIXELFORMAT, C2DBOXF, SCOPED_PTR, CSYSTEMMANAGER, CDISPLAYMANAGER,
  CDISPLAYMANAGER_RENDERTARGET, CRENDERMANAGERCORE, CRENDERMANAGER2D_LOADINGSCREEN, CPLAYERGUI, CPLAYERGUI_GRAPHICBANK,
  SINGLETON_ISSETUP.

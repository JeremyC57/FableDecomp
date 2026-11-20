# NF205 header overlay (2026-11-20)

Every header here is a copy of the CURRENT repo header (base copy in ../orig/, byte-equal to the repo when copied) plus `#ifdef` opt-ins
only. With none of the macros defined, every header is token-identical to the repo copy (the additions sit wholly inside new `#ifdef` blocks).
No FABLE_STATIC_ASSERT was added, moved or removed in any overlay, so pin_asserts.py has nothing to pin (run and recorded below).

## engine/CQ_EndGameFocalSitesScript.h
- Adds `FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_SCARLETROBE`: the nested entity `NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe`
  (Ego_r.pdb CEndGameScarletRobe, 28 bytes: CEntityScriptBase, ParentClass +0x14, PMasterData +0x18; the shape of the existing
  FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES block). Placed after the JACKOFBLADES block, before `#endif // FABLE_RETAIL_NSCRIPT`.
- Used by: rows/00d2d060.cpp.
- Landed includers (grep of rebuild/src/compiled): 0x00D2AA40, 0x00D2ED20, 0x00D2D930. None defines the new macro, so none changes.
- NF202 / NF204 overlays do not carry this header (checked).

## engine/CDataInputStream.h
- Adds `FABLE_RETAIL_CDATAINPUTSTREAM_READPIXELFORMAT` in the retail (FABLE_RETAIL_CDATAINPUTSTREAM) class, public, after SetPosition:
  the declaration `class CPixelFormat ReadPixelFormat();` (Ego_r.pdb / FableWin.pdb ?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ,
  retail 0x00994670). Declaration only, no layout change; the elaborated `class CPixelFormat` needs no prior include.
- Used by: rows/009fd160.cpp, rows/00994670.cpp.
- Landed includers: 119 rows (grep `CDataInputStream.h` in rebuild/src/compiled); none defines the new macro, so none changes.
  pin_asserts: pinned 0. Not in the NF200 / NF202 / NF203 / NF204 overlays (checked).

## engine/CGraphicInfo.h
- Adds `FABLE_RETAIL_CGRAPHICINFO_DEFAULT_CTOR` (active only together with FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) in the
  FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS branch, right after `class CGraphicInfo { public:` and the PeekInfoFrame line: the inline default
  constructor `CGraphicInfo() : Bank(0), Index(-1), Frames(0) {}` (Ego_r.pdb / FableWin.pdb ??0CGraphicInfo@@QAE@XZ). No layout change.
- Used by: rows/00a01650.cpp. Landed includers: 72 rows; none defines the new macro, so none changes. pin_asserts: pinned 0.
  Not in the NF200 / NF202 / NF203 / NF204 overlays (checked).

## engine/CRegionMinimap.h
- Adds `FABLE_RETAIL_CREGIONMINIMAP_LOADTGAFILE` inside the FABLE_RETAIL_CREGIONMINIMAP_BUILDTGA block, right after the BuildRegionMinimapTGA
  declaration: `void LoadRegionMinimapTGAFile(const CWideString& filename);` (Ego_r.pdb / FableWin.pdb, retail 0x0066CA80). Declaration only.
- Used by: rows/0066ca80.cpp. Landed includers: 16 rows; none defines the new macro. pin_asserts: pinned 0. Not in other agents' overlays.

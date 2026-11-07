# Chunk 853 header overlay

Every edit is an **opt-in** branch (`#ifdef FABLE_RETAIL_<X>`); the default text of each repo header is unchanged (`diff` against
rebuild/include shows only added lines, re-checked at hand-back). None of the new macros is defined by any landed source or header (grep of
rebuild/src + rebuild/include: 0 hits each), so no landed includer's preprocessed text changes. Ten landed siblings / includers regraded
read-only with this overlay: all MATCH / RELOCATION_MATCH (SIBLINGS.tsv). No INCLUDERS_*.txt: no default path changed.

| Header | Macro | Row | What / evidence |
|---|---|---|---|
| engine/C2DBoxF.h | `FABLE_RETAIL_C2DBOXF_COPY` (nested in FABLE_RETAIL_C2DBOXF) | 0x0050A650 | The authored copy ctor `C2DBoxF(const C2DBoxF&)` member-wise. FableWin.pdb `??0C2DBoxF@@QAE@ABV0@@Z` (0x01A664E0; inline in release). With it VC7.1 builds the CThingFilter_InArea copy from the four fild results (retail 0x0050AC81..0x0050ACB1), frame 0x80 -> 0x7c, 2252 -> 2233 bytes. |
| engine/C2DCoordI.h | `FABLE_RETAIL_C2DCOORDI_TOFLOAT` | 0x0050A650 | Chunk 850's pending opt-in (FableWin `?ToC2DCoordF@C2DCoordI@@QBE?AVC2DVector@@XZ`, inline), re-applied with one reorder: the class is complete before `engine/C2DVector.h` is included (its FROM3D branch pulls C3DVector.h, whose inline ToC2DCoordI needs C2DCoordI), and ToC2DCoordF is defined inline after the include. Coordinate with chunk 850's copy if it lands first. |
| engine/CDefPointer.h | `FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF` | 0x0043A570 | Release() reads the count through a reference on Object (`mov ebx,[eax+4]; add eax,4; dec ebx; ... mov [eax],ebx`, 0x0043A587; the static's atexit dtor 0x01228B70 has the same shape). Same semantics as the default; the default in-place form gives `dec [ecx+4]`. |
| engine/CNavigationPosition.h | `FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS` | 0x0050A650 | FableWin `??0CNavigationPosition@@QAE@XZ` and `??0CNavigationPosition@@QAE@ABVC2DVector@@J@Z` (inline). No layout change. |
| engine/CPlayerGui.h | `FABLE_RETAIL_CPLAYERGUI_MEMBERS` | 0x0043A570 | Members PFont .. DisplayBottomHalfGUI with their Ego_r.pdb types at the retail offsets (the per-offset destruction code in ~CPlayerGui is the second source; detail in the header comment), plus the static PGuiDef as `CDefPointer<const CPlayerGuiDef>`. |
| engine/NHeroInformationScreens_CManager.h | `FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT` | 0x0043A570 | Inline ~CManager frees three STLport vectors at +0x1C, +0x10, +0x04 (0x0043A5AD..0x0043A5DD); SelectScreenOfType stores at +0x28. Member names and element types are DESCRIPTIVE (neither PDB gives the layout). |
| engine/CSubtitleRenderer.h | `FABLE_RETAIL_CSUBTITLERENDERER_DTOR` | 0x0043A570 | The vptr as a virtual dtor: Ego_r `??1CSubtitleRenderer@@UAE@XZ` / `??_G..`, out of line at 0x00443E20 (`lea ecx,[esi+18h]; call 0x00443E20`, 0x0043B023). Same layout (vptr at +0, sizeof 0x120). The default's flattened vptr member is kept under `#ifndef`. |
| engine/CSymbolMap.h | `FABLE_RETAIL_CSYMBOLMAP_FINDBYVALUE` | 0x0066B370 | Declaration of Ego_r `?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@J@Z`, retail 0x009E5090 (before the (group, value) overload 0x009E50C0, the same order as in Ego_r). |
| engine/CTCSoundPlayer.h | `FABLE_RETAIL_CTCSOUNDPLAYER_PLAYALLSOUNDEVENTS` | 0x0066B370 | Declarations of `PlayAllSoundEvents()` (Ego_r 0x005749A0) and the private `InternalPlaySoundEvent` (landed 0x0066B120, exact PDB spelling) and a forward `class CASoundBank;`. |
| engine/CThingSearchTools.h | `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA` | 0x0050A650 | Template declarations GetAllTCsInRoughArea / GetAllTCDsInRoughArea (Ego_r template order T, Filter, Container; retail out of line 0x00518340, 0x005185F0, 0x00518900). |
| engine/CThingVillage.h | `FABLE_RETAIL_CTHINGVILLAGE_STATGETTYPE` | 0x0050A650 | FableWin `?StatGetType@CThingVillage@@SIJXZ` returns 4 (retail GetAllLeafThings<CThingVillage> 0x005166B0: `push 4; call PeekTypeList`). |
| engine/CThingAICreature.h | `FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE` | 0x0050A650 | FableWin `?StatGetType@CThingAICreature@@SIJXZ` returns 1 (0x00516830: `push 1`). |
| engine/CQ_TentacleKrakenBossFightScript.h | NEW header (requires `FABLE_RETAIL_NSCRIPT`) | 0x00DF5000 | NScript::CQ_TentacleKrakenBossFightScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, Ego_r layout at retail offsets (PDB - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, KrakenAutoSaveCheckPoint +0x4a, TheKraken +0x4c, size 0x58. Checked against the landed DoMission 0x00DF4E00 / ??_G 0x00DF5C60 and PhaseOne. Methods with Ego_r names and retail addresses. No landed includer. Its `#error` guard fires only without FABLE_RETAIL_NSCRIPT (never hit by the row: run errchk before grading anything that includes it). |

Rows: 0x0050A650 also uses the existing opt-ins C2DBOXF_CORNERS, CARRAY, CTHINGSEARCHTOOLS LEAFBODY / ALLTHINGSBODY (already in the repo
headers, untouched).

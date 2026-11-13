# Chunk 2031 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2031/hdr)

Every change to an existing header is an opt-in (or a new header); the default text of every copied header is unchanged, so landed
includers keep their bytes. Copies taken 2026-11-12 from rebuild/include, or from the named hand-backs (c2008 = /opt/fable_lane/w22/agent2008/hdr,
itself partly from c2005; c2018 = rebuild/candidates/manual/pending/2026-11-11_acctB_c2018/hdr). Re-base on HEAD before landing (each copy
was checked to be HEAD + additions only on 2026-11-12). Rows: 0x00B54E30 CEngineWeatherRenderer::RenderSnow (parked, pair-landing) and
0x00B65A20 CEngineSkyRenderer::RenderOuterSky (parked); both differ/ sources compile only with this overlay.

## New opt-ins / new headers (chunk 2031)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CGraphicInfoFrame.h (STATICBLOCKS branch) | `FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME` | declares FableWin.pdb ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z | FableWin name; no Ego_r copy; body below |
| engine/CGraphicInfo.h (STATICBLOCKS branch) | `FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME` | bodies: FABLE_FORCEINLINE CManagedTexture::TouchTexture, FABLE_FORCEINLINE CGraphicInfoFrame::GetFrame (the touch block written in place) | compiled out of line, this GetFrame grades RELOCATION_MATCH against retail's uncalled copy 0x0041C260 (probe/frag/gf1.cpp); TouchTexture's only retail copy 0x0041BB70 is uncalled; c2 inline trace: GetFrame / TouchTexture plain-inline scores over 74 (TouchTexture c1 56, score 133) |
| engine/CResource.h (STATICBLOCKS branch) | `FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE` | FableWin.pdb ?TouchResource@CResource@@QAEXXZ declaration, private ?ListRemove@CResource@@AAEXXZ 0x009FBFB0 / ?ListInsert@CResource@@AAEXPAV1@0@Z 0x009FBF90 | Ego_r / FableWin private names; RenderSnow 0x00B54F11 / 0x00B54F23 |
| engine/CResourceList.h | `FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE` | `friend class CResource;` and the FABLE_FORCEINLINE TouchResource body (Head +0x04, CurrentFrame +0x2C) | the landed CGraphicInfoFrame::GetTexture 0x00B66C10 spelling; retail's only copy 0x0041B9C0 is uncalled (landed under a wrong name, NAME_FIXES.tsv); trace c1 51, score 199 |
| engine/CGraphicFrame.h (c2018 copy) | `FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE` (inside c2018's `_GETTEXTURE`) | FABLE_FORCEINLINE ~CGraphicFrame with the release in its own body (UnInitialise stays plain inline) | ~CFrameData 0x009FE6C0 calls UnInitialise 0x00A00B90 (0x009FE6DB) yet expands the member ~CGraphicFrame in place (0x009FE721..0x009FE769): one cost (c1 88), two decisions; no called retail ??1CGraphicFrame (??_G 0x00B2ED60 uncalled) |
| engine/CEngineEnvironment.h | `FABLE_RETAIL_CENGINEENVIRONMENT_WIND` (in the retail branch) | Ego_r members (CurrentWindVector +0x61C .. ShowWindVector +0x651, no containers so Ego_r = retail) and FableWin.pdb FABLE_FORCEINLINE GetClampedScaledCurrentWindVector(scale, min, max) | RenderSnow 0x00B55130..0x00B5518C; landed WorldFrameUpdate 0x00B52B30 reads NextWindVector +0x628 / NextWindSpeed +0x644 with the same clamp; trace c1 73, score 83 at f 0.5 |
| engine/CRenderStateManager.h | `FABLE_RETAIL_CRENDERSTATEMANAGER_POINTSPRITE_INLINE` | FABLE_FORCEINLINE SetPointSpriteEnable / SetPointScaleEnable (SetRenderState on PointSpriteEnable +0x280 / PointScaleEnable +0x294) | FableWin-only names; RenderSnow 0x00B55063..0x00B5512A (core +0x2A98 / +0x2AAC) |
| engine/C2DBoxI.h | `FABLE_RETAIL_C2DBOXI_GETHEIGHT` | FableWin.pdb ?GetHeight@C2DBoxI@@QBEJXZ (BRY - TLY) | RenderSnow 0x00B55222..0x00B55236 on CDisplayManager::GetViewport (+0x1F4) |
| engine/CEngineWeatherRenderer.h (c2008 copy) | `FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERSNOW` (inside c2008's RENDERRAIN) | private RenderSnowBox 0x00B54210 and RenderSnow 0x00B54E30 | Ego_r / FableWin ?RenderSnowBox@..@@AAEXABVC3DVector@@@Z, ?RenderSnow@..@@AAEXABVCEngineComponentRenderParams@@@Z |
| engine/CEngineSkyRenderer.h (c2008 copy) | `FABLE_RETAIL_CENGINESKYRENDERER_RENDEROUTERSKY` | private RenderOuterSky(float) 0x00B65A20 | Ego_r 0x00A169E0 / FableWin ?RenderOuterSky@CEngineSkyRenderer@@AAEXM@Z |
| engine/CRenderManagerCore.h (c2008 copy) | `FABLE_RETAIL_CRENDERMANAGERCORE_DRAWINDEXED_TEXTURES` | ?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKKK@Z 0x00A0ACA0 (declaration) | Ego_r donor 0x00B74260 between the textured AsTris 0x00B74190 and the KKKK forms (KKKK AsIndexedTris 0x00B74220 = retail 0x00A0AC60); body attaches the texture array then draws indexed (`ret 18h`) |
| engine/CEngineWorldMap.h | `FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD` (in the retail branch) | GameWorld (CIDrawWorld*) at +0x198 and FableWin.pdb ?PeekGameWorld@CEngineWorldMap@@QBEPBVCIDrawWorld@@XZ; the rest of the 0x1C0 class stays opaque | Ego_r name GameWorld; RenderOuterSky 0x00B65A48..0x00B65A55 calls CIDrawWorld slot 1 on [map+0x198] |
| engine/CIDrawWorld.h (new) | default (guard `FABLE_RETAIL_CIDRAWWORLD_DEFINED`) | interface with the retail slot order: 0 DrawGetProceduralTexture, 1 DrawGetEnvironment, 2 DrawGetWorldMap, 3 dtor | vtable 0x012390DC = {0x004AE890, 0x0049E350 (`mov eax,[ecx+14h]` = CWorld+0x18 PEnvironment), 0x0049E1A0 (`[ecx+10h]` = PWorldMap), 0x004AE8A0}; PDB names of the CWorld overrides. Header debt: engine/CWorld.h's CIDrawWorld lists slots 1 / 2 the other way round |
| engine/CIDrawEnvironment.h (new) | default | struct CIDrawEnvironmentParams (members to ThemeChannels +0x54) with nested struct CThemeChannel (Colours[19], Scalers[30] +0x130, TextureSets[3] +0x1A8; 0x1F0 bytes); class CIDrawEnvironment (slot 0 dtor, 1 PeekInterpolatedParams, 2 PeekWindDirection, 3 PeekWindSpeed, 4 PeekSunVector, 5 PeekMoonVector) | struct_layouts_egor.tsv (no containers before ThemeChannels; 24-byte retail texture sets); PDB key U for both structs; CIDrawEnvironment vtable 0x0125D4E4 + CEnvironment vtable 0x0125D53C (slot 1 = landed 0x006B7350 `lea eax,[ecx+1894h]`) |

## Copied unchanged from other hand-backs (needed by the includes; their own opt-ins kept)
engine/CEngineWeatherSettings.h (c2008/c2005: `_TAIL`), engine/CWeatherMask.h (c2008, new), engine/CEngineResourceManager.h (c2008/c2005),
engine/CDisplayManager.h (c2008: `_SETRENDERTARGET`), engine/CSurface.h (c2008: `_ASSIGN`), engine/C4DVector.h (c2008/c2005: `_ADDASSIGN`,
`_ASSIGN3D`), engine/CGraphicFrame.h (c2018: `_GETTEXTURE`, plus the 2031 opt-in above).

## Chunk 2048 additions (agent2048; base = r3014's hdr/ = c2031's overlay, copied unchanged otherwise)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CEngineSkyRenderer.h | `FABLE_RETAIL_CENGINESKYRENDERER_RENDERCLOUDS` | private `void RenderClouds(float interpolate)` 0x00B64FA0 | FableWin.pdb ?RenderClouds@CEngineSkyRenderer@@AAEXM@Z + Ego_r.pdb 0x00A15F60 (just before RenderOuterSky 0x00A169E0, as in retail); `ret 4` |
| engine/CEngineResourceManager.h (retail branch) | `FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE` | inline `CTexture* GetZeroRGBATexture()` returning the Ego_r member ZeroRGBATexture (+0x2AD4) | FableWin.pdb ?GetZeroRGBATexture@CEngineResourceManager@@QAEPAVCTexture@@XZ (its COMDAT is listed right after RenderClouds = first used there); Ego_r layout member ZeroRGBATexture; landed ~CEngineResourceManager 0x00B3B8E0 puts it at +0x2AD4; RenderClouds 0x00B659C9 `add edx,2AD4h` |

Both are additions inside new #ifdef blocks; default texts unchanged. 0x00B65A20 RenderOuterSky (r3014's row) regraded under this overlay:
RELOCATION_MATCH (2026-11-12).

## Chunk 2048 session 2 additions (all opt-ins inside new #ifdef blocks; default texts unchanged; RenderOuterSky 0x00B65A20 regraded
## RELOCATION_MATCH under the final overlay, grade 28; RenderClouds' differ source unchanged at 721/734, grade 29)
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CRenderStateCaptureBlock.h (repo copy) | `FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE` | CRenderStateCaptureBlock with inline bodies (ctor Open = false, dtor if (Open) Restore(), Capture / Restore setting Open); CRenderStateScopedCaptureBlock derives from it, Capture() in ctor, Restore() in dtor | FableWin debug ??0 10 instr / ??1 13 instr (= those bodies); Ego_r.pdb has no out-of-line CRenderStateCaptureBlock function; Ego_r layout: scoped block size 1, no members. NOT used by any row (moves RenderClouds' pattern to AAAAB; RenderOuterSky stays RELOCATION_MATCH with it, grade 15). FITTED candidate only. |
| engine/CRGBFloatColour.h (repo copy) | `FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL` | GetR / GetG / GetB / GetA | FableWin.pdb names (9 debug instr each). Not used by any row. |
| engine/CGraphicInfo.h / CGraphicInfoFrame.h | `FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME` | GetFrame's touch block moved into FABLE_FORCEINLINE TouchFrame(mipmap_level) + private inline GetBank() | FableWin lists TouchFrame (69 instr) and GetBank (10) in GetFrame's COMDAT run. Not used by any row (code-identical, no pattern change). |
| engine/CGraphicInfo.h / CGraphicInfoFrame.h | `FABLE_RETAIL_CFRAMEDATA_ISLOADING` | CFrameData::IsLoading() = FileData.Get() != 0, used in GetFrame's flag-16 test | FableWin ?IsLoading@CFrameData@CGraphicInfoFrame@@QBE_NXZ (14 instr). Body FITTED (unproven). Not used by any row. |
| engine/CGraphicInfoFrame.h + CGraphicInfo.h | `FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING` | ?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z 0x009FE620 declaration; CFrameData::IsLoaded() (PDB name, FREE-branch body); descriptive inline IsFrameDataLoaded() = `if (Data.IsNull()) return false; return Data->IsLoaded();` | RenderSunFlare 0x00B6496E..0x00B6497A (`je ; mov edi,[eax+1Ch] ; test ; setne al ; test al,al`). IsFrameDataLoaded is a DESCRIPTIVE name (no PDB name found). |
| engine/CGraphicInfo.h | `FABLE_RETAIL_CGRAPHICINFO_GETFRAME3` | ?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z 0x009FF8D0 declaration | Ego_r / FableWin |
| engine/CEngineSkyRenderer.h | `FABLE_RETAIL_CENGINESKYRENDERER_RENDERSUNFLARE` (+ forward decls CTexture, CRGBColour) | private RenderSunFlare() 0x00B64550, RenderScreenSpaceSprite(const CTexture*, float x4, CRGBColour) 0x00B63530 | FableWin.pdb |
| engine/CSkyDef.h (new) | default (needs FABLE_RETAIL_DEF_CLASS_BASE) | retail CSkyDef : CParentDefClassBase, SunRadius +0x3C .. FlareElements (STLport vector) +0x74 | Ego_r layout - 4 (packed 0x25 CDefClassBase); RenderSunFlare reads +0x4C, +0x50, +0x74 / +0x78 |
| engine/CEngineCamera.h (repo copy) | `FABLE_RETAIL_CENGINECAMERA_VIEWTOSCREEN` | FABLE_FORCEINLINE TransformWorldToView (ViewTransform.TransformPoint) and TransformViewToScreen (centre + (1/z) * half size * coordinate; Y subtracted) | FableWin names listed right after RenderSunFlare; retail 0x00B647EC..0x00B648A4. Plain inline left TransformWorldToView called (grade 22). |
| engine/CEngineLightingManager.h (repo copy) | `FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKSUNVECTOR` | inline PeekSunVector() returning SunVector (+0x18) | FableWin ?PeekSunVector@CEngineLightingManager@@QBEABVC3DVector@@XZ |
| engine/CEngineOcclusionManager.h (repo copy) | `FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_FLARE` | GetPrevFrameFlareTestResultFromRetrivalID(unsigned long) declaration 0x00B8D3C0 | FableWin name + landed binding |

## Chunk 2048 session 3 additions (opt-ins only; default texts unchanged). Evidence = FableWin.pdb debug size reproduced by plain VC7.1 /Od of
## the opt-in text (/opt/fable_lane/tools/fwsize.py): PROVEN-by-size where marked SAME. Regrades under the final overlay: RenderOuterSky
## 0x00B65A20 RELOCATION_MATCH without (grade 93) and with all opt-ins (cand/ros_fits.cpp, grade 97); RenderSunFlare RELOCATION_MATCH both ways
## (grades 94, 98, src = with); RenderClouds 731/734 (grade 115).
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/C2DVector.h (repo copy) | `FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB` / `_MUL` / `_ADD` | `C2DVector result(...); return result;` operator- / operator*(float) / operator+ | FableWin 26/68, 24/63, 26/68 = SAME. _ADD changes RenderClouds' code (coordinator, 2708 bytes): not used. |
| engine/CGraphicFrame.h | `FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA` (inside `_DTOR_FORCEINLINE`) | ~CGraphicFrame() { ReleaseData(); } + private FABLE_FORCEINLINE ReleaseData | FableWin ??1 9/19 SAME, ReleaseData (0x01D2D310, labelled `Clear` in the signatures) 15/38 SAME; plain inline ReleaseData is called out of line in RenderSunFlare / RenderOuterSky (grades 95/96) |
| engine/CGraphicInfo.h | `FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT` | CGraphicInfoFrame::GetBank() = GraphicInfo->Bank (CGraphicInfo befriends CGraphicInfoFrame) | FableWin 10/20 SAME (through CGraphicInfo::GetBank 10/22) |
| engine/CResourceBank.h (repo copy) + CResourceList.h | `FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT` | GetCurrentFrame() = ResourceList.CurrentFrame (friend) | FableWin 9/17 SAME |
| engine/CGraphicInfoFrame.h | `FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL` (with `_ISLOADING`) | IsLoading() = !FileData.IsNull() | FableWin 14/32 v ours 14/30 (VC10 `add eax,1` v VC7.1 `inc eax`): instruction count SAME. Changes RenderClouds' or-pattern (grade 85). |
| engine/CIVCountedPointer.h (repo copy) | `FABLE_RETAIL_CIVCOUNTEDPOINTER_GET` | T* Get() const | FableWin ?Get@?$CIVCountedPointer@VCFrameData..@@ 9/17 SAME |
| engine/CGraphicInfo.h | `FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET` | TouchTexture: `if (Data.Get()) Data->TouchResource();` | FableWin 17/44 SAME (`!Data.IsNull()` 18/47) |
| engine/CShaderRenderManager.h (repo copy) | `FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX` | SetPixelShaderConstant: PSConstTable[index] = value | FableWin 28/79 SAME (`*(begin()+index)` 29/83) |
Existing opt-ins the best RenderClouds source also uses: FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE (session 2), FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
(TouchFrame 64/209 v FableWin 69/228: still DIFF), FABLE_RETAIL_CFRAMEDATA_ISLOADING, FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL (session 2).
Still DIFF against FableWin (not fitted): TouchFrame +5/+19, CIVCountedPointer<CFrameData>::Release (17/50), Reset (18/47 = the AddRef form, but
retail's cold path has no AddRef: adopt form kept), CIVCountedPointeeBase::Release (24/58; RELEASE_DELETES form gives 23/56), C2DVector operator+.
Rows: FableWin is /EHsc -- plain /Od /EHsc (bin/fwsize_x.py, FW_EXTRA=/EHsc) brings GetFrame to 147/464 (FableWin 146/461; plain /Od 136/413)
and the rows to RenderClouds 348 v 366, RenderOuterSky 208 v 219, RenderSunFlare 442 v 473: most of the "row text gap" is EH bookkeeping.

## Chunk 2048 session 4 additions (opt-ins only; default texts unchanged). Regrades: RenderClouds 0x00B64FA0 RELOCATION_MATCH (src, grade 155);
## RenderSunFlare RELOCATION_MATCH with the three used opt-ins (grade 150) and without (151); RenderOuterSky RELOCATION_MATCH with (152, 153) and without (154).
| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CIVCountedPointeeBase.h (new copy of the repo header) | `FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT` | `long Release() const { unsigned long count = --IVRefCount; if (count == 0) { Delete(); return 0; } return count; }` | FableWin ?Release@CIVCountedPointeeBase@@QBEJXZ 24i/58b; this text 24i/61b (count SAME, FableWin is VC10). Used by RenderClouds. |
| engine/CIVCountedPointer.h | `FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL` (needs RELEASE_COUNT) | `void Release() { if (!IsNull()) { Object->Release(); Object = 0; } }` | FableWin ?Release@?$CIVCountedPointer@VCFrameData..@@QAEXXZ 17i/50b; this text 17i/45b (count SAME). Same semantics. Used by RenderClouds. |
| engine/CRenderStateManager.h | `FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED` | SetRenderState reads `const unsigned long desired_state = state.DesiredState;` for the unchanged test | FITTED: code-identical; one named symbol per expansion decides the bookmark or-order tie (RenderClouds). FableWin's 213i debug body has asserts. |
| engine/CIVCountedPointer.h | `FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE` (with RESET_ADOPT) | Reset stores `CastToBase(object)`; private CastToBase | FableWin ?CastToBase@?$CIVCountedPointer@VCFrameData..@@AAE.. 10i/24b SAME. NOT used (no effect on the row). |
| engine/CGraphicInfo.h | `FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME_EARLYRETURN` | GetFrame returns the loaded level first | FableWin GetFrame 146i /EHsc (this text 146i/462b). NOT used: changes RenderClouds' block layout. |

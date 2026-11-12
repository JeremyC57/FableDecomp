# Chunk 3015 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3015/hdr)

Agent 3015 (continuation of chunk 2032's parked row 0x00BA6FF0): this directory is an UNCHANGED copy of /opt/fable_lane/w22/agent2032/hdr
(= prior/2032/hdr). No header text was edited by agent 3015. The matched row 0x00BA6FF0 now USES the existing opt-in
FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD (declaration of the static fastcall GetAppropriateLOD; the row defines the
FABLE_FORCEINLINE body, which compiles byte-identically to retail's unreferenced out-of-line COMDAT 0x00B9C210). The row text below that says
"probes only, the parked source does not use it" is superseded. Everything below is chunk 2032's README, kept verbatim.

# Chunk 2032 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2032/hdr)

Every file is a copy of the repo header (rebuild/include, 2026-11-12) or of the chunk-2028 overlay copy (/opt/fable_lane/w22/agent2028/hdr,
itself built on rebuild/include + pending c2008 / c2011 / c2024 copies) plus OPT-IN blocks only; the default text of each file is unchanged
except where noted. Re-base on HEAD (and on the 2028 hand-back) before landing.

## Row 0x009C47A0 CFileInstaller::AddFile (RELOCATION_MATCH) uses

| header | opt-in | what | evidence (two sources) |
|---|---|---|---|
| fable_string.h (WHOLE-FILE overlay) | `FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE` (nested in `_TEXT_EQ`) | skips the explicit-specialisation declaration of the out-of-line STLport `operator==(basic_string, const C*)` (0x00404280) so the inline STLport body is used | retail 0x009C4C9A..0x009C4CB5: `(finish - start) & ~1` size test and `compare(start, L"", 0)` 0x00403C60 in place (no 0x00404280 call); the landed 0x0049EF50 / 0x00997F80 local models show the same expansion |
| stlport/fable_string.h | `FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH` | STLport 4.6.2 char_traits.h `__char_traits_base::length` body (folds to 0 for `L""`) | STLport source; retail pushes n = 0 to compare (0x009C4CAD) |
| stlport/fable_string.h (path only) | none | `#include "fable_containers.h"` became `"stlport/fable_containers.h"` so the overlay copy resolves the repo file (no other change; drop at landing, the repo file sits next to fable_containers.h) | -- |
| engine/CFileInstaller.h | `FABLE_RETAIL_CFILEINSTALLER_CFILEINFO_CTOR` | inline body of `CFileInfo(const CWideString&, const CWideString&, long, bool, bool)` (SrcPath, DstPath, InputSize, Started = false, Decompress, CreateMarkerFile) | FableWin.pdb ??0CFileInfo@CFileInstaller@@QAE@ABVCWideString@@0J_N1@Z; retail expands it at 0x009C4A86..0x009C4AB5 and calls its COMDAT 0x009C3130 (landed, same stores) at 0x009C4C90 / 0x009C4DA1 |
| engine/CAFile.h | `FABLE_RETAIL_CAFILE_INSTALLERPATHS` (nested in `FABLE_RETAIL_CAFILE`) | static fastcall ConvertPathToFullPath(const CCharString&) 0x009990B0 / (const CWideString&, CWideString&) 0x00999140, GetFilenameFromPathname 0x00997930, StartFileScan(.., CWideString&) 0x00999C50, ScanNextFile(.., CWideString&) 0x00999CB0 | Ego_r.pdb / FableWin.pdb spellings (SI...); each is the self-symbol of a landed row at that address |

Existing repo opt-ins the row also defines: FABLE_RETAIL_CCHARSTRING(_GETASUPPERCASE, _SUBSTRING, _LESS_INLINE), FABLE_RETAIL_CWIDESTRING(_TEXT_EQ,
_FORCEINLINE_EQ, _FORCEINLINE_DATA_TEXT_EQ), FABLE_RETAIL_STLP_LIST / _SET / _MAP / _TREE_CONSTFIND, FABLE_RETAIL_CDISKFILEWIN32(_DEFAULTCTOR),
FABLE_RETAIL_CCRITICALSECTIONLOCK, FABLE_RETAIL_CCOMPRESSORZLIB, FABLE_RETAIL_CACOMPRESSOR, FABLE_RETAIL_DEF_CLASS_BASE.
Preprocessed with `g++ -E`: only the overlay fable_string.h / stlport/fable_string.h are included (no root-level repo header pulls the repo copy).

## Parked row 0x00BA6FF0 CEnginePrimitiveManagerStaticMeshesBase::DrawSilhouette uses (copies of the chunk-2028 overlay files plus)

| header | opt-in | what | evidence |
|---|---|---|---|
| engine/CEnginePrimitiveManagerOcclusionBase.h (repo copy) | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_SILHOUETTE` | protected GetSilhouetteColour(CEngineInternalPrimitiveOcclusionBase&, CRGBColour&) 0x00B90090, DrawAlphaBoundingBox(const CMatrix3x4&, unsigned long) 0x00B90CD0 | Ego_r.pdb / FableWin.pdb IAE spellings; Ego_r donor order GetSilhouetteColour 0x00A4FCE0 before IsClippedByNearPlane 0x00A4FD00 = retail 0x00B90090 (`xor al,al ; ret 8`) before 0x00B900B0; DrawAlphaBoundingBox landed 0x00B90CD0 |
| engine/CEnginePrimitiveManagerStaticMeshesBase.h (repo copy) | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SILHOUETTE` | private DrawImmediately(..., ERenderMode, ERenderPassType) 0x00BA6DC0 and DrawSilhouette 0x00BA6FF0 | Ego_r.pdb / FableWin.pdb AAE spellings; landed 0x00BA6DC0 |
| engine/CEnginePrimitiveManagerMeshBase.h (repo copy) | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD` | static fastcall GetAppropriateLOD(const C3DMeshInfo*, float, float, float) (declaration; probes only, the parked source does not use it) | FableWin.pdb ?GetAppropriateLOD@CEnginePrimitiveManagerMeshBase@@SIKPBVC3DMeshInfo@@MMM@Z |
| engine/CEngineInternalPrimitiveStaticMesh.h (repo copy) | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_GETRENDERINFO` | inline GetRenderInfo() { return &RenderInfo; } | FableWin.pdb ?GetRenderInfo@CEngineInternalPrimitiveStaticMesh@@QAEPAVCEngineRenderInfoStaticMesh@@XZ (no Ego_r / retail copy); RenderInfo +0xA0 written in place 0x00BA71BD / 0x00BA71FE / 0x00BA7214 |
| engine/CEngineInternalPrimitiveMeshBase.h (repo copy) | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_RADIUS_INLINE` | inline float GetBoundingSphereRadiusWorldSpace() const = MeshInfo->GetBoundingSphere().GetRadius() * Scale | FableWin.pdb ?GetBoundingSphereRadiusWorldSpace@CEngineInternalPrimitiveMeshBase@@QBEMXZ; retail 0x00BA7119 `fld [ecx+34h] ; fmul [ebp+58h]` (descriptive body: radius field +0x34 measured) |
| engine/CRGBColour.h (repo copy) | `FABLE_RETAIL_CRGBCOLOUR_GETASINT` | unsigned long GetAsInt() const { return IntValue; } | FableWin.pdb ?GetAsInt@CRGBColour@@QBEKXZ; 0x00BA7732 dword push into DrawAlphaBoundingBox |
| engine/CEnginePrimitiveRenderUtil.h (2028 copy) | `FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS`, nested `_DISTANCE_SQUARED` | FindLocalLights(const C3DVector&, float, CEnginePrimitiveThemeChannelDesc by value), GetVisibilityRatiosValid, GetShowSilhouettes; CalcDistanceFromCameraSquared (probes only) | FableWin.pdb SI spellings (no Ego_r copy); 0x00BA71C7..0x00BA71DB by-value channel copy |
| engine/CEnginePrimitiveRendererSharedData.h (2028 copy) | `FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES` (nested in `_RENDERSETTINGS`) | BoundingRadiusFix .. LODForced at Ego_r offsets (ShowSilhouettes +0x250, VisibilityRatiosValid +0x25A) | struct_layouts_egor.tsv; 0x00BA7026 / 0x00BA7220 |
| engine/CEngineLightingManager.h (2028 copy) | `FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE` | GetLightsInSphereFast(const C3DBoundingSphere&, long, const CEnginePrimitiveThemeChannelDesc&) | Ego_r.pdb donor 0x009BC860; reloc_bindings binds 0x00B46B90 to that spelling |
| C3DMeshInfo.h, CEngineCamera.h, CEngineCameraDesc.h, CEngineInternalPrimitiveBase.h, CEngineInternalPrimitiveOcclusionBase.h, CEnginePrimitiveManager.h, CIVCountedPointer.h, CMatrix3x4.h, CRenderStateCaptureBlock.h | chunk-2028 overlay copies, unchanged here | | see /opt/fable_lane/w22/agent2028/hdr/README.md |

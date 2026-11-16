# NF41 header overlay

Each file here is a copy of the CURRENT repo header at the same path under `rebuild/include/`, with `#ifdef` opt-ins added only.
Nothing changes unless a row defines the new macro. Every file is an `#ifndef`-guarded header. A `fable_string.h` overlay
(root, `#pragma once`) was tried and withdrawn: with quote includes resolving from the includer's directory, it double-defined classes
in any TU that also reached another root `fable_*.h` header (vland SRC_FAIL on 0x007D3560). It is parked in `work/withdrawn/`, not here.

| Overlay (copy of rebuild/include/...) | New opt-in(s) | Used by row(s) |
|---|---|---|
| engine/C3DAnimFileSequenceChunk.h | FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX (AddMatrix 0x00AAF800, CArray<CMatrix3x4> Matrices, implicit destructor) | 0x00A4FD70 |
| engine/C3DClothPrimitive.h | FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPRIMITIVEINDEX | 0x00BA6FA0, 0x00BA77B0 |
| engine/C3DMeshStats.h | FABLE_RETAIL_C3DMESHSTATS_HASHELPERPOINT | 0x00A36100 |
| engine/C3DPositionInterpolator.h | FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_IMPLICIT_DTOR | 0x004BCFE0 |
| engine/C3DPrimitive2.h | FABLE_RETAIL_C3DPRIMITIVE2_GETCLOTHPRIMITIVE (inside the CLOTHPRIMITIVES branch) | 0x00BA6FA0, 0x00BA77B0 |
| engine/CAFile.h | FABLE_RETAIL_CAFILE_EXPANDPATH | 0x00999850 |
| engine/CAIStateGroup_BirdGroundWander.h | FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER (class branch: public virtual Cleanup / IsRunnable / Update, protected FrameLastFinishedWalking) | 0x009143E0 |
| engine/CChunkedFileChunk.h | FABLE_RETAIL_CCHUNKEDFILECHUNK_GETCHUNKTYPE | 0x00AA7F40, 0x00AA7FF0 |
| engine/CChunkedFileSuperChunk.h | FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_SUBCHUNKS_TYPED (0x00AA7F40, 0x00AA7FF0), FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_FINDNEXT_CONST (0x00AA7FF0) | see left |
| engine/CDisplayManager.h | FABLE_RETAIL_CDISPLAYMANAGER_ISVIEWPORTOFFSCREEN (0x009DB700, 0x009DB810, 0x009DBAE0, 0x009DBD10, 0x009DC030, 0x009DC2C0), FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORTFLOAT (0x00A0AAA0) | see left |
| engine/CEngineCamera.h | FABLE_RETAIL_CENGINECAMERA_SCREENTOHOMOGENOUS (FableWin TransformScreenTo2DHomogenousSpace 0x00B2FB70) | 0x00B63530 |
| engine/CEngineTreeFiler.h | FABLE_RETAIL_CENGINETREEFILER_ADDNODEAUTOEXPANDTREE, FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT | 0x00BF3030 |
| engine/CNavQuadTree.h | FABLE_RETAIL_CNAVQUADTREE_SETBLOCKEDNODE | 0x00A76340 |
| engine/CQuickDrawTriInfo.h | FABLE_RETAIL_CQUICKDRAWTRIINFO_COPY (default ctor, copy ctor, operator=) | 0x009DB700, 0x009DB810, 0x009DBAE0, 0x009DBD10, 0x009DC030, 0x009DC2C0 |
| engine/CRenderManager2D.h | FABLE_RETAIL_CRENDERMANAGER2D_ADDTEXT (0x009DB700, 0x009DBFF0, 0x009DD8F0), _ADD2DLINE (0x009DC2C0), _ADDLINE: AddTri, AddLine, AddAlphaTri, public AddQuadToDrawBuffer (0x009DB810, 0x009DBAE0, 0x009DBD10, 0x009DC500) | see left |
| engine/CSurfaceCompositor.h | FABLE_RETAIL_CSURFACECOMPOSITOR_SETBLENDMODE (protected) | 0x00F39A50 |
| engine/CTCCreatureGenerator.h | FABLE_RETAIL_CTCCREATUREGENERATOR_GETNUMGENERATEDCREATURES | 0x0074A100 |
| engine/CTCCreatureNavigation.h | FABLE_RETAIL_CTCCREATURENAVIGATION_SETNAVIGATORTOTYPE | 0x00912700 |
| engine/CTCOpinionSource.h | FABLE_RETAIL_CTCOPINIONSOURCE_AWARENESS_PEEKS (IsReactNonstop, PeekUseNavLineOfSight: Ego_r / FableWin inline accessors) | 0x00960FC0 |
| engine/CTexture.h | FABLE_RETAIL_CTEXTURE_EXTENTS_INLINE (GetPixelFormat plus inline unsigned GetWidth / GetHeight) | 0x009DC870 |
| engine/CThingSearchTools.h | FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL (ORIGINAL_ROUGHAREA body: the typed pointer is a named local pushed by reference) | 0x00746560 |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_ISTHEMEDESTROYABLE | 0x0063DD70 |
| engine/NCreatureAI.h | FABLE_RETAIL_NCREATUREAI_AWARE_INDIRECTLY (NCreatureAI::IsAwareOfThingIndirectly 0x00864700, both PDBs) | 0x00960FC0 |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_ISDRAWINGAFTERWATER (protected; 0x004BD620, 0x004BD690), FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETATTACHEDPRIMITIVEGRAPHICS (0x004BD690) | see left |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE (re-based on the current repo copy, which gained NF42's LIST_DEFAULT_CTOR_NOINLINE) | 0x00618420 |

The new-macro list was produced by diffing each overlay against the repo header and keeping the `FABLE_RETAIL_*` names that
appear in an overlay `#if`/`#ifdef` line and nowhere in the repo copy. The final recheck (`recheck_final.log`, 61 rows, 7 MATCH + 54
RELOCATION_MATCH, 61 tests PASS) ran with this overlay (`FABLE_HDR_OVERLAY`).

Purity: `diff <repo> <overlay> | grep '^<'` is empty for every overlay (checked after the re-base).

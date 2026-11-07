# Chunk 851 header overlay

Every edit to an existing header is an opt-in branch: its macro is defined only by this chunk's rows. Each branch carries a
`// Retail opt-in (chunk 851)` or `// Nested opt-in (chunk 851)` evidence comment. The default text is unchanged, with no
lines reflowed. Every repo header was copied fresh. On 2026-11-07 at 16:40 UTC, every file was diffed against
rebuild/include. CIVCountedPointer.h and CIVCountedPointeeBase.h had changed in the repo (chunk 850 landed its RESET_ADOPT /
CTOR opt-ins at 15:40), so I copied them again and re-applied my branches. CIVCountedPointer's RELEASE_MEMBER is nested inside the existing
`#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER` (the repo lines are textually intact). After that, 00A9BBE0 and the includers
00AC5770 / 00ABF800 / 00B2D9F0 were rechecked again. My copy of C3DMeshFileEngineMaterialChunk.h is
gone: the repo now has chunk 850's FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK retail branch, and 0x00A9BBE0 matches with
it unchanged.

Includers rechecked read-only with this final overlay using `vland --recheck-landed`; all are RELOCATION_MATCH + PASS (see SIBLINGS.tsv; the
logs went with tmp/ at cleanup): 00BA6DC0, 00606300, 0047A9F0, 00775B40, 00B2D9F0, 00ABF800, 00ABD150, 00ABBD70,
00AC4FE0, 00AC5770 and 008E7CF0. No default path changed, so no INCLUDERS_*.txt file is needed.

Chunk 850's overlay also edits CThing.h, CWorldMap.h, CIVCountedPointer.h and CIVCountedPointeeBase.h. Its opt-ins and
mine use different macros. CThing.h and CWorldMap.h need a merge at landing. The two CIV headers in the repo already carry
chunk 850's branches.

## Edits to existing headers (all opt-in)

| header | macro | what | evidence | row |
|---|---|---|---|---|
| engine/C3DMeshFile{SubMesh,MaterialList,Helpers,Transformation,Primitive,Cloth,SmoothingGroups,UniqueVertices,Group,Material,MaterialExt,HelperPoint,DummyObject}Chunk.h | FABLE_RETAIL_<UPPER CLASS> wraps the generated file (`#ifdef` retail class / `#else` original text / `#endif`) | Polymorphic retail class on the CChunkedFileSuperChunk / CChunkedFileChunk retail branches. Each has its out-of-line ctor (and a user dtor for the five that have one); retail sizes are listed in each comment. | RTTI bases (ghidra_out/rtti_bases.tsv), Ego_r members, the retail ctor addresses 0x00AB9A70 .. 0x00AC66A0, and the Codex pass-12 class models | 00A9BBE0 |
| engine/C3DMeshFileVerticesChunk.h | FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK_CTOR (nested inside the existing retail branch) | ctor 0x00ABC780, user dtor 0x00ABC7B0, the five overrides | vtable 0x0129ECF8; Ego_r | 00A9BBE0 |
| engine/C3DMeshFileLoader.h | FABLE_RETAIL_C3DMESHFILELOADER_INLINE_REGISTER | Includes C3DMeshFileRootChunk.h. Inline Register*SubChunkType bodies (`AddChunkType(chunk, X->GetAsSuperChunk())`). Declares RegisterClothSubChunkType (0x00A9B930, out of line). | Retail expands each register call into AddChunkType 0x00A9AD00 inside the ctor | 00A9BBE0 |
| engine/CMirroredTextureHighlightsChunk.h | FABLE_RETAIL_CMIRROREDTEXTUREHIGHLIGHTSCHUNK | Retail class with the inline ctor ("MTEH", 0x00AA7590) and inline virtual dtor | Expanded in 0x00A9BBE0 | 00A9BBE0 |
| engine/CIVCountedPointer.h | FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER (with FABLE_RETAIL_CIVCOUNTEDPOINTER) | `Release(){ if (Object) { Object->Release(); Object = 0; } }` | 0x00A9C4BF: count loaded before `mov ecx,eax` (the cached-copy form schedules it the other way) | 00A9BBE0 |
| engine/CIVCountedPointeeBase.h | FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES (nested in FABLE_RETAIL_DEF_CLASS_BASE) | `long Release() const` that calls Delete() at zero (the Codex model) | 0x00A9C4C7 | 00A9BBE0 |
| engine/CEnginePrimitiveRendererSharedData.h | FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS | Typed DynamicIBUsage +0xD0, DynamicIBPool +0xD4, DynamicIBLockFlags +0xD8, PixelShader* +0xDC..+0xF4, HighlightMirroredTexturesEnabled +0x262 | Ego_r names and offsets (no container before +0x268); retail reads at 0x00BA1B00 | 00BA1B00 |
| engine/CEnginePrimitiveManagerStaticMeshesBase.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED | Private DisplayMirroredTextures declaration plus forward declarations | Ego_r + FableWin symbol | 00BA1B00 |
| engine/CEnginePrimitiveRenderUtil.h | FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR | `static void __fastcall SetTextureFactor(const CRGBFloatColour&)` (no retail copy; the row defines the body) | Ego_r + FableWin symbol; expanded at 0x00BA1BDC / 0x00BA1F56 | 00BA1B00 |
| engine/CTCInventoryMap.h | FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION | Declarations: ConstructRegionDescriptionShops, ConstructRegionExitMarkers, GetRelativePosOnMiniMap (0x005FBAA0), IsThingAsPosBeingDrawn (0x005FABA0) | Ego_r + FableWin; Ego_r member order equals retail address order | 006001E0, 005FE840 |
| engine/CTCShop.h | FABLE_RETAIL_CTCSHOP_SHOPTYPE | IsBarberShop / IsTattooShop / IsTitleShop declarations | Ego_r + FableWin (CTCShop) | 006001E0 |
| engine/CTCDRegionExit.h | FABLE_RETAIL_CTCDREGIONEXIT_MINIMAP | Inline IsActive / IsHiddenOnMiniMap / IsReversedOnMiniMap | Ego_r + FableWin; read in place at 0x005FEA4B / 0x005FEA54 / 0x005FEBFF | 005FE840 (parked) |
| engine/CTCActionUseScriptedHook.h | FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_MINIMAP | Inline IsHiddenOnMiniMap / IsReversedOnMiniMap | Ego_r + FableWin; +0x4D / +0x4C in place | 005FE840 (parked) |
| engine/CTCQuestCard.h | FABLE_RETAIL_CTCQUESTCARD_ROUTE (nested) | GetRegionName (0x007025A0), IsCore (0x00702280), IsRouteToAppearOnMinimap (0x007022F0) declarations | Ego_r + FableWin | 005FE840 (parked) |
| engine/CThing.h | FABLE_RETAIL_CTHING_SCRIPTNAME | Inline `const CCharString& GetScriptName() const` | Ego_r + FableWin; `lea ecx,[esi+74h]` in place at 0x005FEB00 | 005FE840 (parked) |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_ROUTE | GetNextRegionOnRouteTo(long,long) const declaration (0x004FB4D0) | Ego_r + FableWin | 005FE840 (parked) |

## New headers

| header | what | evidence | row |
|---|---|---|---|
| engine/C3DMeshFileRootChunk.h | Root chunk ("3DRT") with an inline ctor (CChunkedFileSuperChunk 0x00AA8360, vtable 0x0129E090), 0x2c | Codex model; 0x00A9BBE0 expands it | 00A9BBE0 |
| engine/C3DMeshFileBoneChunk.h, C3DMeshFileMapChunk.h, C3DMeshFileConvexVolumeChunk.h | Retail chunk classes (ctors 0x00AB9DA0 / 0x00ABFE50 / 0x00AAAA10; sizes 0x70 / 0x3C / 0x3C) | RTTI + Ego_r; Codex | 00A9BBE0 |
| engine/CPlane.h, engine/C3DVertexBlend2.h | Ego_r PODs used by the Bone / ConvexVolume / Vertices members (0x10, 8) | Ego_r layouts | 00A9BBE0 |
| engine/CThingFilter_InMap.h | Ego_r CThingFilter_InMap (WorldMap, MapNo; 8 bytes); inline ctor, out-of-line operator() 0x006A7930 | Ego_r + FableWin; landed binding | 006001E0 |

GFMain's 16 new headers (Codex models) are not in this overlay. They are in differ/00402510_hdr/engine/ as the starting
point for that header debt (NOTES.md).

The overlay was confirmed to be the one in use: the 00A9BBE0 / 00BA1B00 sources only compile with the opt-in macros that
exist only in these copies (m1 failed to compile when the vertices chunk was abstract and matched once the overlay edit
went in).

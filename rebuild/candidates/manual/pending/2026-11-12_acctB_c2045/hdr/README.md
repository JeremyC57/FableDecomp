# Chunk 2045 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2045/hdr) -- started from chunk 2038's overlay (unchanged text below), plus the chunk 2045 additions in the last section

Used by row 0x00BB8750 CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup (RELOCATION_MATCH). Three files are copies of
the repo header (rebuild/include, 2026-11-12) with ADDED opt-in blocks only (default text unchanged: `diff` shows 0 removed lines; the
generated flattened models now sit in the `#else` arm of the new opt-in); four files are new. No shared inline body was given a second text.
Earlier copies I tried (r3015's CEngineCamera / CEngineCameraDesc / CEngineInternalPrimitiveBase / CIVCountedPointer / CEnginePrimitiveManager
and r3015's CMatrix3x4) are NOT needed: with the repo versions the object is byte-identical, so they were dropped.

| header | kind | opt-in | what | evidence (two sources) |
|---|---|---|---|---|
| engine/CMatrix3x4.h | repo copy + block | `FABLE_RETAIL_CMATRIX3X4_SETIDENTITY` | inline `SetIdentity()`: `E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f; E11 = E22 = E33 = 1.0f;` -- the SAME text as the pending c2005 / c2014 / r3001 overlays (one body) | FableWin.pdb ?SetIdentity@CMatrix3x4@@QAEXXZ (the c2005 comment also says Ego_r.pdb: not in ghidra_out/egor_pdb_names.tsv, FableWin only); retail 0x00BB8998..0x00BB89E5 stores the zeros E43..E12 then the ones E33, E22, E11 (chained assignments store right to left), as 0x00AB28CF |
| engine/CEngineInternalPrimitiveParticleGroup.h | repo copy + typed branch | `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP` | `class CEngineInternalPrimitiveParticleGroup : public CEngineInternalPrimitiveBase` with the Ego_r.pdb members typed (Primitives = STLport `std::list<CEnginePrimitiveHandle>` +0x48, PrimitiveIDToIterator CArray<CPrimitiveIterator> +0x4C, FreePrimitiveIDs list<long> +0x58, BSphere +0x5C, DistanceFromCam +0x6C, WorldMat +0x70, the flags +0xA0..+0xAA; sizeof 0xAC) and the nested CPrimitiveIterator (Valid +0, Iterator +4) | Ego_r.pdb layout (struct_layouts_egor.tsv) at the generated retail offsets; Fable.exe RTTI `.?AVCEngineInternalPrimitiveParticleGroup@@` : CEngineInternalPrimitiveBase@0 (ghidra_out/rtti_bases.tsv); retail 0x00BB8A10 / 0x00BB8ED7 read the list node pointer at +0x48 |
| engine/CEnginePrimitiveManagerParticleDecalGroup.h | repo copy + typed branch | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP` | `class ... : public CEnginePrimitiveManager` with typed members (PointLightShaderIndex[6] +0xC, CVertexShader LandscapeDecalVertexShader[3] +0x24 .. CIndexBuffer MeshDecalIndexBuffer +0x110; sizeof 0x124), PDB member functions (ctor, virtual dtor, private BuildBuffers, RenderLandscapeDecalGroup, RenderMeshDecalGroup, RenderMeshDecalBlock, ApplyMeshVertexShader, RenderMeshDecalStencilClear) and the inline private `ApplyLandscapeVertexShader(unsigned long)` | Ego_r.pdb + FableWin.pdb spellings (AAE / UAE); RTTI : CEnginePrimitiveManager@0; ApplyLandscapeVertexShader is FableWin-only (donor 0x02EAD1F0) and its body compiles RELOCATION_MATCH against retail's unreferenced out-of-line copy 0x00BB97F0 (probe/oolapply.cpp, tools/qx.py, QLEN 0x1D); expanded at 0x00BB875B..0x00BB8773 |
| engine/CEngineInternalPrimitiveParticleDecalGroup.h | NEW | (turns on the typed base) | `class CEngineInternalPrimitiveParticleDecalGroup : public CEngineInternalPrimitiveParticleGroup`: DecalBankIndex +0xAC, Flags +0xB0, BlendMode +0xB4, BlendOp +0xB8, StencilCubeHeight +0xBC, LandscapeDecal +0xC0 (sizeof 0xC4) | Ego_r.pdb layout (own members from the Ego_r base size 0xC0); RTTI `.?AV...` : CEngineInternalPrimitiveParticleGroup@0; the landed Update 0x00BB6D70 stores +0xAC..+0xC0; RenderSubPrimitive tests +0xC0 before calling this row (0x00BB951E) |
| engine/CEngineInternalPrimitiveParticleDecal.h | NEW | -- | `class CEngineInternalPrimitiveParticleDecal : public CEngineInternalPrimitiveBase`: WorldPos +0x48, Size (C2DVector) +0x54, Colour (CRGBColour) +0x5C, DecalFrame +0x60, RHSet (CRightHandedSet) +0x64 (sizeof 0x7C) | Ego_r.pdb layout (no container member: retail = Ego_r); RTTI : CEngineInternalPrimitiveBase@0; this row reads +0x48..+0x6C (0x00BB8A66..0x00BB8C85) |
| engine/CTVertexNormTex1Base.h | NEW | -- | X, Y, Z, NormalX, NormalY, NormalZ, U1, V1 (0x20 bytes) | Ego_r.pdb layout CTVertexNormTex1Base; retail stores +0x00..+0x1C per vertex (0x00BB8C4E..0x00BB8CA4) |
| engine/CTVertexNormTex1.h | NEW | -- | `class CTVertexNormTex1 : public CGenericVertex<CTVertexNormTex1Base> { };` (no own member, no user constructor) | FableWin.pdb ?SetPosition / SetNormal / SetU1V1 @?$CGenericVertex@VCTVertexNormTex1Base@@ and CArray<CTVertexNormTex1> members; pdb_class_keys V; no ??0CTVertexNormTex1 in either name set; the landed vector<CTVertexNormTex1>(n) 0x00BB9FD0 copies an unwritten frame temporary (the derived class is default-initialised, not zeroed) |

Defines the row also uses from existing repo opt-ins: FABLE_RETAIL_DEF_CLASS_BASE, _CENGINEPERSISTENTPRIMITIVEBASE,
_CENGINEINTERNALPRIMITIVEBASE_ATTACHED, _CIVCOUNTEDPOINTER, _CRENDERMANAGERCORE (+ _DRAWASINDEXEDTRIS_USER), _CRENDERSTATEMANAGER, _CTEXTURE,
_CINDEXBUFFER, _CVERTEXBUFFERWIN32, _CENGINECOMPONENT, _CMATRIX3X4_SETTRANSLATEONLY, _CENGINECAMERA_POSITION, _C3DVECTOR, _CRIGHTHANDEDSET,
_CARRAY (CArray(n) forwards to vector(n), so CArray<CTVertexNormTex1>(n) calls the landed 0x00BB9FD0), _STLP_LIST.

Not needed by this row: no CEngineMap / CEngineWorldMap inline at all (a landscape decal here takes z from the decal's WorldPos.Z; the row never
reads landscape heights), so it is no evidence for the c2037 CEngineMap.h opt-in question.

Re-base on HEAD at landing (rebuild/include + the pending c2005 SetIdentity block; drop the duplicate if c2005's CMatrix3x4 overlay lands first).


## Chunk 2045 additions (row 0x00BB8EF0 CEnginePrimitiveManagerParticleDecalGroup::RenderSubPrimitive, RELOCATION_MATCH)
Every change is an ADDED opt-in block or a new header; `diff` against rebuild/include shows 0 removed lines for every repo copy. Chunk 2038's row
0x00BB8750 was regraded under this overlay: still RELOCATION_MATCH (it includes none of the new files; the decal-group header only gained a
declaration).

| header | kind | opt-in | what | evidence (two sources) |
|---|---|---|---|---|
| engine/CEnginePrimitiveManagerParticleDecalGroup.h | c2038 copy + 1 declaration (inside the existing typed branch) | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP` | `virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType);` (public) | Ego_r.pdb + FableWin.pdb ?RenderSubPrimitive@CEnginePrimitiveManagerParticleDecalGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z; RTTI vtable 0x012A5998 slot 5 = 0x00BB8EF0 |
| engine/CEngineSubPrimitiveBase.h | repo copy + block | `FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE` | inline `GetParentBase()` / `PeekParentBase() const` returning Parent (+0x0C) | FableWin.pdb ?GetParentBase@CEngineSubPrimitiveBase@@QAEPAVCEngineInternalPrimitiveBase@@XZ / ?PeekParentBase@..@@QBEPBV..@@XZ (no Ego_r copy, none in retail: inline); retail 0x00BB8F0A `mov ebp,[eax+0Ch]` |
| engine/CGraphicInfo.h | c2037 / c2031 / r3014 copy (identical in the three) + 2 blocks | `FABLE_RETAIL_CGRAPHICINFO_GETFRAME` | declaration `bool GetFrame(CGraphicFrame&, unsigned long frame_index, unsigned long mipmap_level, unsigned long flags) const;` | Ego_r.pdb + FableWin.pdb ?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z; retail 0x009FF8D0 `ret 10h`, called at 0x00BB8F8B with three zero pushes |
| engine/CGraphicInfo.h | (same file) | `FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC` | inline `CGraphicDataBank::PeekGraphic(index)`: `index < GraphicList.size() ? &GraphicList[index] : 0` (defined after CGraphicInfo, the element type must be complete) | FableWin.pdb ?PeekGraphic@CGraphicDataBank@@QBEPBVCGraphicInfo@@K@Z (donor 0x01D2D7A0); retail's uncalled out-of-line copy 0x0041C1B0 compiles MATCH (probe/ool/peekgraphic.cpp, tools/qx.py QLEN 0x3C); expanded at 0x00BB8F49..0x00BB8F81 |
| engine/CGraphicFrame.h | c2037 copy, unchanged | (`FABLE_RETAIL_CGRAPHICFRAME` + `_CTOR`, `_GETTEXTURE`, `_DTOR_FORCEINLINE`) | the pending c2018 / c2031 / c2037 text (not yet in rebuild/include) | see the comments in the file |
| engine/CEnginePrimitiveManagerParticleGroup.h | NEW | -- | `class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager` (Ego_r.pdb members at their offsets, byte-packed: CurrentWorldPos +0x0E; sizeof 0x28) with `long SetUpPointLights();` and `SetupPixelShader(...)` declarations | Ego_r.pdb layout (struct_layouts_egor.tsv) + FableWin / Ego_r ?SetUpPointLights@CEnginePrimitiveManagerParticleGroup@@QAEJXZ (landed 0x00BBC0B0); RTTI V; the landed 0x00BBC0B0 / 0x00BBC460 local models use the same offsets |

Defines this row also uses from existing repo opt-ins: FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE (inline GetEnable()),
FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL (SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode declarations; the header leaves the bodies to
the including row), FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO (+ _CENGINEINTERNALPRIMITIVEBASE_ATTACHED: GetPrimitiveManager(long) const),
FABLE_RETAIL_CENGINELIGHTINGMANAGER, FABLE_RETAIL_CENGINERESOURCEMANAGER (PeekGraphicBank), FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS,
FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS (expanded AttachTextureToStage), FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
(list::size() = STLport distance(); the TU's distance<const list iterator> COMDAT compiles MATCH against retail's 0x00BB9770).

Header debt / review at landing: the blend helper bodies live in the row (as engine/CEnginePrimitiveRenderUtil.h asks); the parked hand742 row
0x00BBE090 writes the same two bodies with local enum names -- if both land, move one text into a shared header opt-in. GFDebugPDynamicCast is
defined row-locally with the exact text of engine/CThing.h (the landed precedent, e.g. 0x00984D90).

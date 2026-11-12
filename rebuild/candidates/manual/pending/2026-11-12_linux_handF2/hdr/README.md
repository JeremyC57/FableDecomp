# chunk F2 header overlay

Each file is a full copy of the current repo header plus additions; `diff` against rebuild/include shows only added lines. Every
addition is behind a new `#ifdef FABLE_RETAIL_*`, so rows that do not define the macro compile as before. Nothing was copied from
another chunk's overlay.

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES | `CWideString GetManualSaveFileName(long) const` 0x00406610, `GetManualSaveDisplayName(long) const` 0x00406AE0 | 00406ae0 |
| engine/CBankFileAsync.h | FABLE_RETAIL_CBANKFILEASYNC_HANDLEFAILEDALLOCATION | `bool HandleFailedAllocation()` 0x009D5BF0 | 009d5bf0 |
| engine/CBankFileAsyncData.h | FABLE_RETAIL_CBANKFILEASYNCDATA_FREEBUFFER | `friend class CBankFileAsync`; private `void FreeBuffer()` 0x009D5410 (declaration) | 009d5bf0 |
| engine/CBankFileAsyncData.h | FABLE_RETAIL_CBANKFILEASYNCDATA_CHANGELOADINGPRIORITY | `void ChangeLoadingPriority(EThreadedFilePriority)` 0x009D5630 | 00be06b0 |
| stlport/fable_map.h | FABLE_RETAIL_STLP_SET_UPPER_BOUND (with FABLE_RETAIL_STLP_TREE_ERASE_KEY) | `_Rb_tree::upper_bound(const K&) const` returning const_iterator; `set::upper_bound(const K&) const` | 009d5bf0 |
| engine/CEngineStaticMapFileBlockManager.h | FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ADDREQUEST | `void AddRequest(long, long, bool)` 0x00BE06B0 | 00be06b0 |
| engine/CCombatSequence_WillForcePushLongRange.h | FABLE_RETAIL_CCOMBATSEQUENCE_WILLFORCEPUSHLONGRANGE (new branch; generated struct under `#else`) | retail class `: CCombatSequenceBase` (includer supplies the base), `virtual bool IsValid() const` 0x00909A00, sizeof 0x28 | 00909a00 |
| engine/CEngineSceneGridCell.h | FABLE_RETAIL_CENGINESCENEGRIDCELL_CHECKVISIBILITYRESULTS | private `bool CheckVisibilityResults(EEngineOcclusionLayer, unsigned long, unsigned long, bool)` 0x00B936E0; `friend class CEngineSceneGrid` | 00b939e0 |
| engine/C3DPrimitive2.h | FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS | `N3DPrimitive::CAnimatedBlock : CBaseBlock` (0x30 bytes, Ego_r members, GetVertexCount / GetGroupCount / GetGroup); typed `AnimatedBlocks`; `GetAnimatedBlock(index)`, `GetAnimatedBlockCount()`, `GetVertexCount()` | 00bcfb00 |
| engine/N3DPrimitive_CStaticBlock.h | FABLE_RETAIL_CBASEBLOCK_ACCESSORS (default branch) | CBaseBlock `GetPrimitiveCount()`, `GetStartIndex()`, `GetPrimitiveType()` (needs engine/EPrimitiveType.h) | 00bcfb00 |
| engine/CShaderRenderManager.h | FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW | inline bodies of `SetBoneTransforms(long, long, const CPreTransposedBoneMatrix*)` and the six-argument `DrawBuffersIndexed`; includes engine/CIndexBuffer.h and engine/CRenderManager.h (includer defines FABLE_RETAIL_CRENDERMANAGERCORE) | 00bcfb00 |
| engine/CShaderConstantRange.h | FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY | drops the declared copy constructor and destructor (implicit ones); the declarations stay under `#else` | 00bcfb00 |
| engine/CVertexShaderConstantLayout.h | FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETBONEMATRICES_INLINE | inline `GetBoneMatrices() const { return BoneMatrices; }`; the declaration stays under `#else` | 00bcfb00 |
| engine/CEngineVSConstantLayoutLightsBones.h | FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTLIGHTSBONES (new branch; generated struct under `#else`) | retail class `: CVertexShaderConstantLayout` (ctor, virtual dtor, PeekName), sizeof 0x14C | 00bcfb00 |
| engine/CEngineResourceManager.h | FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTLIGHTSBONES (retail branch) | inline `PeekVSConstantLayoutLightsBones()` over the opaque member | 00bcfb00 |

Grading note: all six rows were graded with this one overlay directory (no second directory needed).

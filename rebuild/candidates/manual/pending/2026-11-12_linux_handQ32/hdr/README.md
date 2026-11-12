# chunkQ32 header overlays

Full copies of the current repo headers with additions only, each behind a `#ifdef FABLE_RETAIL_*` opt-in (`diff` against the
repo shows only `>` lines; checked at hand-back). Grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ32/hdr`.
Inherited from chunk Q29 (unchanged, see its notes): engine/CTCMultiArrow.h, engine/CBankFileAsyncData.h,
engine/CMovableResourceMemoryPool.h. Nothing copied from chunk Q25's overlay.

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CEngineInternalPrimitiveAnimatedMesh.h | FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_BONESCALINGS (inside ORIGINAL_CACHE) | `void InitialiseBoneScalings(const C3DMesh2&) const;` (Ego_r/FableWin QBE) and `mutable` on Cache (the const method inserts into it) | 00bd2a90 |
| engine/C3DAnimationBlendState.h | FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH (+ SHAREDDATA) | inline FableWin `HasSkeletalMorph() const` / `PeekSkeletalMorph() const` | 00bd2a90 |
| engine/C3DMesh2.h | FABLE_RETAIL_C3DMESH2_BONES | inline Ego_r/FableWin `GetBoneCount() const` / `GetBone(unsigned long) const` (needs a complete C3DBone2) | 00bd2a90 |
| engine/CAIStateGroup_MinionReactToCrime.h | FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIME | the polymorphic retail class on CAIStateGroupBase (Ego_r members, ForceOpenBuilding a byte-packed CIntelligentPointer<CThingBuilding> at +0x1B; PDB method set, OnReplace public; private GetSomeoneElseGettingHelpMessage 0x00958770) | 00958b20 |
| engine/CThingBuilding.h | FABLE_RETAIL_CTHINGBUILDING_FORCEOPENCOUNT | inline Ego_r/FableWin `DecrementForceOpenCount()` (`dec [eax+0D0h]`) | 00958b20 |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SETDEEDMASK | `void SetOpinionDeedMask(long);` (Ego_r ?SetOpinionDeedMask@CTCCreatureOpinionOfHero@@QAEXJ@Z, 0x006821E0) | 00958b20 |
| stlport/fable_map.h | FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE | per-TU: STLport's `_Rb_tree(const Compare&, const A&)` and `~_Rb_tree()` bodies kept out of line (`__declspec(noinline)`, visible) -- retail calls 0x00451ADD / 0x00451B07 in that TU | 00465c54 |
| engine/CGraphicFrame.h | FABLE_RETAIL_CGRAPHICFRAMEDATA_INITIALISEASSURFACE (in the STATICBLOCKS_TEXTURE_HELPERS CGraphicFrameData) | `InitialiseAsSurface(...)` declaration + inline `GetWidth() const` / `GetHeight() const` (Ego_r QBEKXZ) | 009fc950 |
| engine/CTexture.h | FABLE_RETAIL_CTEXTURE_GETWIDTH | inline `GetWidth(unsigned long level) const { return GetDimensions(level).X; }` (Ego_r/FableWin ?GetWidth@CTexture@@QBEKK@Z) | 009fc950 |

00eb3ba0 and 00722740 need no new overlay (00eb3ba0 defines IsValid's real body in its own TU; 00722740 uses existing opt-ins).

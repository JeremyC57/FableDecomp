# hdr/ overlay (re-based by the coordinator from the earlier agent's overlay; the verbatim old overlay is ../hdr_prior, its README is ../hdr_prior/README.md)
- overlay re-based onto the current repo headers by the coordinator: 20 unchanged-base copies, 6 three-way merges, 5 new headers, 4 conflicts
- merged engine/CGraphicFrame.h: repo header changed since the overlay was made; three-way merge clean
- merged engine/CGraphicInfo.h: repo header changed since the overlay was made; three-way merge clean
- STALE engine/CSurface.h: the repo header changed and the three-way merge conflicts; this is the OLD overlay copy verbatim (so the draft still compiles) -- re-base it by hand onto the current rebuild/include/engine/CSurface.h (current text + your additions only) before the hand-back
- STALE engine/CIVCountedPointer.h: the repo header changed and the three-way merge conflicts; this is the OLD overlay copy verbatim (so the draft still compiles) -- re-base it by hand onto the current rebuild/include/engine/CIVCountedPointer.h (current text + your additions only) before the hand-back
- merged engine/CGraphicInfoFrame.h: repo header changed since the overlay was made; three-way merge clean
- merged engine/CEngineResourceManager.h: repo header changed since the overlay was made; three-way merge clean
- merged engine/CEnginePrimitiveRenderUtil.h: repo header changed since the overlay was made; three-way merge clean
- STALE engine/C2DVector.h: the repo header changed and the three-way merge conflicts; this is the OLD overlay copy verbatim (so the draft still compiles) -- re-base it by hand onto the current rebuild/include/engine/C2DVector.h (current text + your additions only) before the hand-back
- STALE engine/CRenderStateManager.h: the repo header changed and the three-way merge conflicts; this is the OLD overlay copy verbatim (so the draft still compiles) -- re-base it by hand onto the current rebuild/include/engine/CRenderStateManager.h (current text + your additions only) before the hand-back
- merged engine/CShaderRenderManager.h: repo header changed since the overlay was made; three-way merge clean
# bwf1 author session (2026-11-12)
- STALE resolved: engine/CSurface.h, engine/C2DVector.h, engine/CRenderStateManager.h overlays DROPPED (the row grades identically on the
  current repo headers; old copies kept in ../probe/stale_old/). engine/CIVCountedPointer.h RE-BASED = current repo header + two opt-ins
  from the old overlay: FABLE_RETAIL_CIVCOUNTEDPOINTER_GET (Get() const) and FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL (first branch of
  the Release chain).
- NEW engine/CEngineTreeFiler.h (current repo header + FABLE_RETAIL_CENGINETREEFILER_INLINES): CNodeHeader body, FindID, AddKey, Get/SetGenericObject,
  FindNode (forceinline, recursion = call 0x00BC9B30), AddNewKey decl 0x00BF3030, CEngineTreeFiler<T>::GetEntry/SetEntry.
- NEW engine/CEngineVertexBufferPool.h (current repo header + FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES): CEngineDynamic3DResource<T,N> body
  (Key[N], PoolNext), CListHeader() ctor, ReturnResource / GetResource / AddNewResource, CEngineVertexBufferPool::GetKey / GetBuffer, BuildNewBuffer decl 0x00BF3380.

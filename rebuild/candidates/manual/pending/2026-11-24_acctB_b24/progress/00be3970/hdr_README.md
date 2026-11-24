Overlay for rows 0x00BE3970 / 0x00BE4660 / 0x00BE4FE0 (B24C6, 2026-11-24): every file is a full copy of the CURRENT repo header plus
additive #ifdef sections (default branches unchanged).
- engine/CPatchTesselationEdgeStrip.h: FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDWATERVERT_EARLYOUT (B24C2, FableWin FindWaterVert
  65i/194b) and FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDVERT (declarations of FindVert / PeekVertexCount; FindVert = FableWin-exact
  while loop 48i/140b, FABLE_FORCEINLINE: plain inline leaves a call in 0x00BE3970, FableWin debug copy 0x02CBD3D0, no Ego_r proc).
- engine/CEdgeStipVertexArray.h: FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT -- Inferred (operator[] release ASSERT dead arm; FableWin
  operator[] 73i/245b v plain 31i/83b; graded 35 -> 25 blocks on 0x00BE3970, inert on the twins). See ../ESCALATE.md.
- engine/CLandscapeTesselatorBase.h: FABLE_RETAIL_CLANDSCAPETESSELATORBASE_ADDPOLY (B24C2; FableWin AddPoly 28i/84b).
- engine/CLandscapePatchTesselator.h: FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR_EDGE_HELPERS (FableWin helper declarations).
- engine/_quarantine/CEngineLandscapeMap.h: FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP (0x00BDC980 declaration).
- engine/CWaterPatchTesselatorBase.h: the twins' overlay copy (B24C2), unchanged.

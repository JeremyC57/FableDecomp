# Overlay opt-ins (B24C2; each file is a full copy of the CURRENT repo header plus one #ifdef section; default branch unchanged)
- engine/CPatchTesselationEdgeStrip.h: FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDWATERVERT_EARLYOUT -- FindWaterVert in the text whose /Od
  build equals FableWin.pdb ?FindWaterVert@CPatchTesselationEdgeStrip@@QBEKK@Z (65i/194b; the old body is 67i/199b); sibling FindVert 48i/140b
  is the same while loop. The old body stays the #else branch (landed 0x00BE6880 does not call FindWaterVert).
- engine/_quarantine/CEngineLandscapeMap.h: FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP -- declaration of GetEdgeStrip 0x00BDC980
  (manifest; Ego_r 0x009B52B0 / FableWin 0x02CC93B0 ?GetEdgeStrip@CEngineLandscapeMap@@QAEPAVCPatchTesselationEdgeStrip@@JJJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z).
- engine/CLandscapeTesselatorBase.h: FABLE_RETAIL_CLANDSCAPETESSELATORBASE_ADDPOLY -- AddPoly body (FableWin 84b/28i = our /Od), FABLE_FORCEINLINE.
- engine/CWaterPatchTesselatorBase.h: FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPER_BODIES -- the shared template bodies of
  FindFirstWaterAdjacentVertex / IterateThroughAdjacentVerts (both TesselateEdge instantiations), FABLE_FORCEINLINE with the evidence inline.

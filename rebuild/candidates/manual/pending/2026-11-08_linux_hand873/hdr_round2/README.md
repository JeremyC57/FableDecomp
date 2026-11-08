# Chunk 873 round-2 header overlay (`FABLE_HDR_OVERLAY=$S/w22/agent873/hdr`)

The round-1 overlay is merged into rebuild/include (kept for reference in `hdr_landed_r1/`, with its README). This overlay holds
only the round-2 changes, all for the parked row 0x00BE3970 CLandscapePatchTesselator::TesselateEdge (differ/00be3970.cpp); the
matched rows grade against rebuild/include alone. Every change to an existing header is a new nested opt-in (repo default paths
unchanged); one header is new.
- `engine/CPatchTesselationEdgeStrip.h`: `FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDVERT` (inside _ACCESSORS): FableWin.pdb
  ?FindVert@CPatchTesselationEdgeStrip@@QBEKK@Z (`__forceinline`: a plain inline out-of-class definition stayed out of line; retail
  expands both calls) and ?PeekVertexCount@..@@QBEJXZ.
- `engine/CLandscapePatchTesselator.h`: `FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR_HELPERS` (inside _CLANDSCAPEPATCHTESSELATOR): the
  Ego_r.pdb CTVertexLandscapeBackground layout (X, Y, Z, NormalX/Y/Z, UVPad1, V1 +0x15, U1 +0x16, UVPad2; was `Bytes[0x18]`) and
  the FableWin.pdb private helpers AddFanBaseVertex / AddDuplicateEdgeVertex / AddCompositeVertex (bodies in the row).
- `engine/CLandscapeBackgroundPatch.h`: `FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_VERTEX_NORMAL` (inside _LANDSCAPE_BACKGROUND_PATCH):
  static __fastcall SetTargetVertexNormal(CTVertexLandscapeBackground&, unsigned long) (Ego_r 0x009C67F0, FableWin 0x02CE09E0,
  retail 0x00BE66F0); its forward `class CTVertexLandscapeBackground;` sits before the enum under the same macro.
- NEW `engine/CEngineLandscapeMap.h`: method-only model, GetEdgeStrip(long, long, long, LANDSCAPE_EDGE_STRIP_SIDE) 0x00BDC980
  (Ego_r / FableWin ?GetEdgeStrip@CEngineLandscapeMap@@QAEPAVCPatchTesselationEdgeStrip@@JJJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z).

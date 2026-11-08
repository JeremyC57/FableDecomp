#ifndef FABLE_ENGINE_CENGINELANDSCAPEMAP_H
#define FABLE_ENGINE_CENGINELANDSCAPEMAP_H
// Hand-written (chunk 873, row 0x00BE3970 CLandscapePatchTesselator::TesselateEdge): CEngineLandscapeMap, the landscape half of a
// CEngineMap (CEngineMap::LandscapeMap +0x00). Ego_r.pdb sizeof 0x78 (PatchTree +0x04, Patches +0x08, PatchGridWidth +0x0C, ...);
// the layout is not modelled here, only the member function the tesselators call:
// Ego_r.pdb / FableWin.pdb ?GetEdgeStrip@CEngineLandscapeMap@@QAEPAVCPatchTesselationEdgeStrip@@JJJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z,
// retail 0x00BDC980 (thiscall, `ret 10h`).
class CPatchTesselationEdgeStrip;
enum LANDSCAPE_EDGE_STRIP_SIDE;                                          // MS forward enum
class CEngineLandscapeMap {
public:
    CPatchTesselationEdgeStrip* GetEdgeStrip(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);   // 0x00BDC980
};

#endif // FABLE_ENGINE_CENGINELANDSCAPEMAP_H

#ifndef FABLE_ENGINE_CWATERPATCHTESSELATORBASE_H
#define FABLE_ENGINE_CWATERPATCHTESSELATORBASE_H
// Hand-written (chunk 723): CWaterPatchTesselatorBase<T> : CLandscapeTesselatorBase (RTTI .?AV?$CWaterPatchTesselatorBase@V..@@@@),
// instantiated for CTVertexWaterBackground (vtable 0x012A3334, 0x38-byte vertex) and CTVertexSeaBackground (0x012A334C).
// Ego_r.pdb member BridgeVertexBuffer (CArray<T>) after the base: retail +0x14, sizeof 0x20 (CEngineWaterRenderer::Tesselator
// +0x564 / SeaTesselator +0x584). Ctor / dtor: Ego_r.pdb ??0 / ??1 ...QAE@XZ (the dtor is the implicit non-virtual one).
// The private overrides are the base's five slots.
#include "rebuild_abi.h"
#include "engine/CLandscapeTesselatorBase.h"
#ifdef FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPERS
class CEngineMap;
#endif
template <class T> class CWaterPatchTesselatorBase : public CLandscapeTesselatorBase {
public:
    CWaterPatchTesselatorBase();
private:
    virtual void TesselateEdge(CPatchTesselationEdgeStrip* strip, long a, long b, bool c, bool d, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices);   // slot 0
    virtual long GetBridgeVertexBufferSize();                            // slot 1
    virtual unsigned char* GetBridgeVertexBufferAsCArray();              // slot 2
    virtual long GetSizeOfVertex();                                      // slot 3
    virtual void SetBridgeVertexBufferSize(long size);                   // slot 4
#ifdef FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPERS
    // Retail opt-in (chunk 785, rows 0x00BE4660 / 0x00BE4FE0 TesselateEdge): the private helpers of both instantiations, Ego_r.pdb
    // ?FindFirstWaterAdjacentVertex@?$CWaterPatchTesselatorBase@V..@@@@AAEXAAPAVCPatchTesselationEdgeStrip@@AAJPBVCEngineMap@@_NJJJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z,
    // ?IterateThroughAdjacentVerts@..AAEXAAPAVCPatchTesselationEdgeStrip@@AAJPBVCEngineMap@@JJJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z,
    // ?AddDuplicateEdgeVertex@..AAEGPAVCPatchTesselationEdgeStrip@@J@Z and ?AddFanBaseVertex@..AAEGPAVCPatchTesselationEdgeStrip@@J@Z.
    // Retail has no out-of-line copy: TesselateEdge expands all four (bodies in the row TU).
    void FindFirstWaterAdjacentVertex(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, bool vertical, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);
    void IterateThroughAdjacentVerts(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);
    unsigned short AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index);
    unsigned short AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index);
#endif
public:
    CArray<T> BridgeVertexBuffer;                                        // +0x14
};

#endif // FABLE_ENGINE_CWATERPATCHTESSELATORBASE_H

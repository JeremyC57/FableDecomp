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
#ifdef FABLE_RETAIL_CWATERPATCHTESSELATORBASE_INLINE_CTOR
    // Retail opt-in (wfE6, row 0x00B73760): FableWin.pdb ??0?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@QAE@XZ / ..Sea.. (debug
    // 0x02CBDA30 / 0x02CBDB00), expanded by the CEngineWaterRenderer ctor (0x00B73A54..0x00B73AA4: base, vtable 0x012A3334 / 0x012A334C,
    // BridgeVertexBuffer zeros); no out-of-line copy in retail.
    CWaterPatchTesselatorBase() { }
#else
    CWaterPatchTesselatorBase();
#endif
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

#ifdef FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPER_BODIES
// Retail opt-in (B24C2, rows 0x00BE4660 / 0x00BE4FE0): the bodies of the two instantiation-independent private helpers, shared by both
// TesselateEdge instantiations (needs FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPERS, FABLE_RETAIL_CENGINEMAP,
// FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP and FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS). FABLE_FORCEINLINE: retail Fable.exe
// has no out-of-line copy of either (both TesselateEdge bodies expand them: two FindFirstWaterAdjacentVertex copies each, the second
// inside IterateThroughAdjacentVerts), the Ego_r.pdb tesselator module stream (Engine_landscape_tesselator.obj) has no proc record for
// them, and plain `inline` leaves both as calls under VC7.1's inline budget. Only the debug FableWin.pdb has out-of-line copies
// (FindFirstWaterAdjacentVertex 0x02CC5650 250 bytes, IterateThroughAdjacentVerts 0x02CC5790 307 bytes, Sea instantiation).
#include "engine/CEngineMap.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CPatchTesselationEdgeStrip.h"
template <class T>
FABLE_FORCEINLINE void CWaterPatchTesselatorBase<T>::FindFirstWaterAdjacentVertex(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, bool vertical, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    strip = map->GetLandscapeMap()->GetEdgeStrip(x, y, length, side);
    while (strip) {
        index = strip->FindWaterVert(vertical ? y : x);
        if (index < strip->PeekWaterVertexCount()) return;
        long size = strip->PeekSize();
        if (vertical) y += size;
        else x += size;
        length -= size;
        if (length <= 0) {
            strip = 0;
            index = 0;
            return;
        }
        strip = map->GetLandscapeMap()->GetEdgeStrip(x, y, length, side);
    }
}
template <class T>
FABLE_FORCEINLINE void CWaterPatchTesselatorBase<T>::IterateThroughAdjacentVerts(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(strip->GetWaterVertex(index).LandscapeVertexIndex);
    if (index < strip->PeekWaterVertexCount() - 1) {
        index++;
        return;
    }
    bool vertical = strip->PeekIsVertical();
    if (vertical) {
        length = length - strip->PeekEndPos() + y;
        y = strip->PeekEndPos();
        if (y == vert.WorldY) {
            y++;
            length--;
        }
    } else {
        length = length - strip->PeekEndPos() + x;
        x = strip->PeekEndPos();
        if (x == vert.WorldX) {
            x++;
            length--;
        }
    }
    if (length > 0) {
        FindFirstWaterAdjacentVertex(strip, index, map, vertical, x, y, length, side);
    } else {
        strip = 0;
        index = 0;
    }
}
#endif

#endif // FABLE_ENGINE_CWATERPATCHTESSELATORBASE_H

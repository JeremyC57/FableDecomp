// CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices) -- the sea instantiation of the water edge bridging (twin 0x00BE4660): walks the strip's water vertices and, for each bridged span, fans the adjacent strip's water vertices in between onto a fan-base vertex
// Grade: RELOCATION_MATCH
// Leaf: ?TesselateEdge@?$CWaterPatchTesselatorBase@VCTVertexSeaBackground@@@@EAEXPAVCPatchTesselationEdgeStrip@@JJ_N1W4LANDSCAPE_EDGE_STRIP_SIDE@@PAG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE4FE0, 2412 bytes, thiscall `ret 1Ch`, frame 0x68, no EH. Identity: rtti-vtables.tsv
//   CWaterPatchTesselatorBase<CTVertexSeaBackground> vtable 0x012A334C slot 0 (Ego_r.pdb / FableWin.pdb ?TesselateEdge@..@@EAEX..@Z,
//   private virtual). Water twin: vtable 0x012A3334 slot 0 = 0x00BE4660.
// Callees: CEngineLandscapeMap::GetEdgeStrip 0x00BDC980 (x4: two FindFirstWaterAdjacentVertex copies), CArray<CTVertexSeaBackground>::
//   push_back 0x00BE5950 (x4, the AddDuplicateEdgeVertex / AddFanBaseVertex expansions, each over a fresh uninitialised local), STLport
//   vector<unsigned short>::_M_insert_overflow 0x005135D0 (x10, the inlined AddPoly / CArray<unsigned short>::push_back).
// Helpers (Ego_r.pdb names): FindFirstWaterAdjacentVertex / IterateThroughAdjacentVerts (shared bodies, engine/CWaterPatchTesselatorBase.h),
//   AddPoly (engine/CLandscapeTesselatorBase.h), the sea AddDuplicateEdgeVertex / AddFanBaseVertex below; FindWaterVert in the
//   FableWin-sized form (engine/CPatchTesselationEdgeStrip.h FINDWATERVERT_EARLYOUT). FABLE_FORCEINLINE on the two sea helpers: retail has
//   no out-of-line copy (FableWin.pdb, the debug build, has 0x02CC3BA0 / 0x02CC3AB0), Ego_r.pdb's tesselator module stream has no proc
//   record for them, and plain `inline` leaves them as calls.
// Shape that matched: the adjacency test reads strip->GetWaterVertex(i) again for `start` (the water reference is not reused there), the
//   not-found branch stores previous = -1 before the index store and every bridged span ends in `continue` (one shared skip store).
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CEDGESTIPVERTEXARRAY_ACCESSORS
#define FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS
#define FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDWATERVERT_EARLYOUT
#define FABLE_RETAIL_CENGINEMAP
#define FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP
#define FABLE_RETAIL_CLANDSCAPETESSELATORBASE_ADDPOLY
#define FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPERS
#define FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPER_BODIES
#include "engine/CArray.h"
#include "engine/CEngineMap.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CTesselationData.h"
#include "engine/CPatchTesselationEdgeStrip.h"
#include "engine/CWaterPatchTesselatorBase.h"
#include "engine/CTVertexSeaBackground.h"

template <> void CArray<CTVertexSeaBackground>::push_back(const CTVertexSeaBackground& x);   // 0x00BE5950

template <>
FABLE_FORCEINLINE unsigned short CWaterPatchTesselatorBase<CTVertexSeaBackground>::AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexSeaBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(index);
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(water.LandscapeVertexIndex);
    v.X = vert.WorldX;
    v.Y = vert.WorldY;
    v.Z = water.Z;
    return (unsigned short)new_index;
}

template <>
FABLE_FORCEINLINE unsigned short CWaterPatchTesselatorBase<CTVertexSeaBackground>::AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexSeaBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(index);
    CPatchTesselationEdgeStrip::CWaterFanBaseVertex& water_fan = strip->GetWaterFanBaseVert(water.FanBaseIndex);
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(water.LandscapeVertexIndex);
    CPatchTesselationEdgeStrip::CFanBaseVertex& fan = strip->GetFanBaseVert(vert.FanBaseIndex);
    v.X = fan.WorldX;
    v.Y = fan.WorldY;
    v.Z = water_fan.Z;
    return (unsigned short)new_index;
}

template <class T>
void CWaterPatchTesselatorBase<T>::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices)
{
    long length = vertical ? Data->PeekHeight() : Data->PeekWidth();
    const CEngineMap* map = Data->PeekEngineMap();
    CPatchTesselationEdgeStrip* adjacent_strip;
    long adjacent_index = 0;
    FindFirstWaterAdjacentVertex(adjacent_strip, adjacent_index, map, vertical, x, y, length, side);
    long previous = -1;
    for (long i = 0; i < strip->PeekWaterVertexCount() - 1; i++) {
        CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(i);
        if (water.EdgeIndexBufferIndex != 0xffff) {
            long start = strip->GetMainAxisVertexWorldPos(strip->GetWaterVertex(i).LandscapeVertexIndex);
            long end = strip->GetMainAxisVertexWorldPos(strip->GetWaterVertex(i + 1).LandscapeVertexIndex);
            if (end - start > 1 && strip->GetWaterVertex(i + 1).LandscapeVertexIndex == water.LandscapeVertexIndex + 1) {
                bool found = false;
                long fan = -1;
                while (adjacent_strip && adjacent_strip->GetMainAxisVertexWorldPos(adjacent_strip->GetWaterVertex(adjacent_index).LandscapeVertexIndex) < end) {
                    if (adjacent_strip->GetMainAxisVertexWorldPos(adjacent_strip->GetWaterVertex(adjacent_index).LandscapeVertexIndex) > start) {
                        found = true;
                        if (fan < 0) fan = AddFanBaseVertex(strip, i);
                        if (previous < 0) previous = AddDuplicateEdgeVertex(strip, i);
                        long added = AddDuplicateEdgeVertex(adjacent_strip, adjacent_index);
                        if (flip) AddPoly(fan, added, previous);
                        else AddPoly(fan, previous, added);
                        previous = added;
                    }
                    IterateThroughAdjacentVerts(adjacent_strip, adjacent_index, map, x, y, length, side);
                }
                if (found) {
                    long added = AddDuplicateEdgeVertex(strip, i + 1);
                    if (flip) AddPoly(fan, added, previous);
                    else AddPoly(fan, previous, added);
                    previous = added;
                    if (water.FanBaseIndex != 0xffff) indices[water.EdgeIndexBufferIndex] = strip->GetWaterFanBaseVert(water.FanBaseIndex).VertexBufferIndex;
                    else indices[water.EdgeIndexBufferIndex] = 0xffff;
                } else {
                    previous = -1;
                    indices[water.EdgeIndexBufferIndex] = water.EdgeVertexBufferIndex;
                }
                continue;
            }
        }
        previous = -1;
    }
}

template void CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge(CPatchTesselationEdgeStrip*, long, long, bool, bool, LANDSCAPE_EDGE_STRIP_SIDE, unsigned short*);

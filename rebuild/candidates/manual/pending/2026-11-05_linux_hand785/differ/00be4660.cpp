// CWaterPatchTesselatorBase<CTVertexWaterBackground>::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices) -- bridge one water edge strip to the adjacent patches' strips: walk the adjacent water vertices in world order and fan every one-vertex span that holds some of them around its fan base vertex, redirecting the span's edge index to the fan base
// Grade: DIFFER(2380v2349) -- same template body as the sea row 0x00BE4FE0 (8 bytes off); call set, block order, vertex fills and all expansions are retail's; residue is register colouring of the inlined helpers' by-value parameter copies: retail keeps the first FindFirstWaterAdjacentVertex copy's length in edi and pushes side from eax with the map in a frame slot, ours gives side the register and homes length (+20 bytes in the first walk), and the main loop keeps i in a frame slot where retail has it in eax
// Next idea: the 160-combination local-order / scope search (attempts.log) moved only colouring; try the helpers as Lionhead in-class inline members (the PDB order: IterateThroughAdjacentVerts before FindFirstWaterAdjacentVertex) or the FindFirst loop over a local copy of the strip pointer
// Leaf: ?TesselateEdge@?$CWaterPatchTesselatorBase@VCTVertexWaterBackground@@@@EAEXPAVCPatchTesselationEdgeStrip@@JJ_N1W4LANDSCAPE_EDGE_STRIP_SIDE@@PAG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE4660, 2349 bytes, thiscall `ret 1Ch`, frame 0x11C, no EH. Identity: rtti-vtables.tsv
//   CWaterPatchTesselatorBase<CTVertexWaterBackground> vtable 0x012A3334 slot 0 (Ego_r.pdb ?TesselateEdge@..@@EAEX..@Z, private
//   virtual); the catalog `vfunc_0` is a placeholder. Sea twin: vtable 0x012A334C slot 0 = 0x00BE4FE0 (frame 0x68).
// Callees: CEngineLandscapeMap::GetEdgeStrip 0x00BDC980 (x4, two inlined FindFirstWaterAdjacentVertex copies), CArray<
//   CTVertexWaterBackground>::push_back 0x00BE4F90 (x4, the four AddDuplicateEdgeVertex / AddFanBaseVertex expansions, each over
//   a fresh uninitialised local), STLport vector<unsigned short>::_M_insert_overflow 0x005135D0 (x10, the inlined AddPoly /
//   CArray<unsigned short>::push_back). Helper names from Ego_r.pdb: FindFirstWaterAdjacentVertex, IterateThroughAdjacentVerts,
//   AddDuplicateEdgeVertex, AddFanBaseVertex (all private, no retail out-of-line copy), CLandscapeTesselatorBase::AddPoly,
//   CPatchTesselationEdgeStrip::FindWaterVert / GetMainAxisVertexWorldPos / GetWaterVertex / GetVert / GetFanBaseVert /
//   GetWaterFanBaseVert (inline accessors).
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CEDGESTIPVERTEXARRAY_ACCESSORS
#define FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS
#define FABLE_RETAIL_CWATERPATCHTESSELATORBASE_HELPERS
#define FABLE_RETAIL_CENGINEMAP
#include "engine/CArray.h"
#include "engine/CEngineMap.h"
#include "engine/CTesselationData.h"
#include "engine/CPatchTesselationEdgeStrip.h"
#include "engine/CWaterPatchTesselatorBase.h"
#include "engine/CTVertexWaterBackground.h"

class CEngineLandscapeMap {
public:
    CPatchTesselationEdgeStrip* GetEdgeStrip(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);   // 0x00BDC980
};

template <> void CArray<CTVertexWaterBackground>::push_back(const CTVertexWaterBackground& x);   // 0x00BE4F90

__forceinline void CLandscapeTesselatorBase::AddPoly(long a, long b, long c)
{
    BridgeIndexBuffer.push_back((unsigned short)a);
    BridgeIndexBuffer.push_back((unsigned short)b);
    BridgeIndexBuffer.push_back((unsigned short)c);
}

template <class T>
__forceinline void CWaterPatchTesselatorBase<T>::FindFirstWaterAdjacentVertex(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, bool vertical, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
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
__forceinline void CWaterPatchTesselatorBase<T>::IterateThroughAdjacentVerts(CPatchTesselationEdgeStrip*& strip, long& index, const CEngineMap* map, long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
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

template <>
__forceinline unsigned short CWaterPatchTesselatorBase<CTVertexWaterBackground>::AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexWaterBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(index);
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(water.LandscapeVertexIndex);
    v.X = vert.WorldX;
    v.Y = vert.WorldY;
    v.Z = water.Z;
    memcpy(v.ShoreLookUpArray, water.ShoreLookUpArray, sizeof(v.ShoreLookUpArray));
    return (unsigned short)new_index;
}

template <>
__forceinline unsigned short CWaterPatchTesselatorBase<CTVertexWaterBackground>::AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexWaterBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CWaterVertex& water = strip->GetWaterVertex(index);
    CPatchTesselationEdgeStrip::CWaterFanBaseVertex& water_fan = strip->GetWaterFanBaseVert(water.FanBaseIndex);
    CPatchTesselationEdgeStrip::CFanBaseVertex& fan = strip->GetFanBaseVert(strip->GetVert(water.LandscapeVertexIndex).FanBaseIndex);
    v.X = fan.WorldX;
    v.Y = fan.WorldY;
    v.Z = water_fan.Z;
    memcpy(v.ShoreLookUpArray, water_fan.ShoreLookUpArray, sizeof(v.ShoreLookUpArray));
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
            long start = strip->GetMainAxisVertexWorldPos(water.LandscapeVertexIndex);
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
                    continue;
                }
                indices[water.EdgeIndexBufferIndex] = water.EdgeVertexBufferIndex;
            }
        }
        previous = -1;
    }
}

template void CWaterPatchTesselatorBase<CTVertexWaterBackground>::TesselateEdge(CPatchTesselationEdgeStrip*, long, long, bool, bool, LANDSCAPE_EDGE_STRIP_SIDE, unsigned short*);

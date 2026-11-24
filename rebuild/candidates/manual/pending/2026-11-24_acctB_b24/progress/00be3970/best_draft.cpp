// CLandscapePatchTesselator::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices) -- bridge one landscape edge strip to the adjacent patches' strips: for every span of the strip longer than one unit, fan the adjacent strips' vertices that fall inside it around the span's fan base vertex, then redirect the span's edge index
// Grade: DIFFER(2392v2390) -- 25 dd blocks (B24C5, overlay c5/hdr): head only -- -1 constant ebp v edx (push ebp placement), FindVert zero sharing, loop-top start through vert v Verts.Data reload
// Next: find the source form that makes VC7.1 keep `vert` as a pointer (an opaque accessor, vert used before the start/end tests, or a different GetMainAxisVertexWorldPos spelling); TE_AXIS variants (GetVert / if-return / local ref), vert after start/end and start read through vert tried, worse or equal. 19 grades.
// Retail-asm: GFFloatToLongNear (engine/GFFloatToLongNear.h) at 0x00BE3DD0 / 0x00BE3E1F `fld dword ptr [esp+44h] ; fistp dword ptr [esp+48h/4Ch]` (float homed, bare dword fistp, low byte stored): a row that matches grades retail_asm.
// Retail: Fable.exe @ 0x00BE3970, 2390 bytes (true extent; 0x00BE42D0 is a separate function), thiscall `ret 1Ch`, frame 0x6C. Identity: vtable 0x012A2B3C slot 0;
//   Ego_r.pdb 0x009C3350 / FableWin.pdb 0x02CBCC00. Helpers (FableWin.pdb, no retail copy): AddFanBaseVertex, AddDuplicateEdgeVertex, AddCompositeVertex,
//   CLandscapeTesselatorBase::AddPoly, CPatchTesselationEdgeStrip::FindVert / GetVert / GetFanBaseVert / GetMainAxisVertexWorldPos / PeekVertexCount.
//   Callees: CEngineLandscapeMap::GetEdgeStrip 0x00BDC980 (x2), vector<CTVertexLandscapeBackground>::_M_insert_overflow 0x00B6D0B0 (3 expanded push_backs) and
//   push_back 0x00BE4380 (the 4th, called), CLandscapeBackgroundPatch::SetTargetVertexNormal 0x00BE66F0 (x4), vector<unsigned short>::_M_insert_overflow 0x005135D0 (x10).
//   Grades with FABLE_HDR_OVERLAY=$A/hdr (CPatchTesselationEdgeStrip.h _FINDVERT, CLandscapePatchTesselator.h _HELPERS, CLandscapeBackgroundPatch.h
//   _VERTEX_NORMAL, new CEngineLandscapeMap.h).
// Leaf: ?TesselateEdge@CLandscapePatchTesselator@@EAEXPAVCPatchTesselationEdgeStrip@@JJ_N1W4LANDSCAPE_EDGE_STRIP_SIDE@@PAG@Z
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_PROBE_EDGESTIP_ASSERT
#define FABLE_PROBE_EDGESTIP_ASSERT_NC FABLE_RETAIL_ASSERT
#define FABLE_PROBE_EDGESTIP_ASSERT_C FABLE_RETAIL_ASSERT
#define FABLE_PROBE_EDGESTIP_COND index < Count
#define FABLE_PROBE_FINDVERT_INLINE FABLE_FORCEINLINE
#define FABLE_RETAIL_CEDGESTIPVERTEXARRAY_ACCESSORS
#define FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS
#define FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDVERT
#define FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR
#define FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR_EDGE_HELPERS
#define FABLE_RETAIL_LANDSCAPE_BUILD_DEFAULT_SECTION_PASS28
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_CENGINEMAP
#define FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP
#include "engine/CArray.h"
#include "engine/CEngineMap.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CTesselationData.h"
#include "engine/CPatchTesselationEdgeStrip.h"
#include "engine/CLandscapePatchTesselator.h"
#include "engine/CLandscapeBackgroundPatch.h"
#include "engine/GFFloatToLongNear.h"

__forceinline void CLandscapeTesselatorBase::AddPoly(long a, long b, long c)
{
    BridgeIndexBuffer.push_back((unsigned short)a);
    BridgeIndexBuffer.push_back((unsigned short)b);
    BridgeIndexBuffer.push_back((unsigned short)c);
}

__forceinline unsigned short CLandscapePatchTesselator::AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexLandscapeBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(index);
    CPatchTesselationEdgeStrip::CFanBaseVertex& fan = strip->GetFanBaseVert(vert.FanBaseIndex);
    v.X = fan.WorldX;
    v.Y = fan.WorldY;
    v.Z = fan.WorldZ;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(v, fan.Normal);
    v.U1 = fan.U;
    v.V1 = fan.V;
    return (unsigned short)new_index;
}

__forceinline unsigned short CLandscapePatchTesselator::AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexLandscapeBackground& v = BridgeVertexBuffer[new_index];
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(index);
    v.X = vert.WorldX;
    v.Y = vert.WorldY;
    v.Z = vert.WorldZ;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(v, vert.Normal);
    v.U1 = vert.U;
    v.V1 = vert.V;
    return (unsigned short)new_index;
}

__forceinline unsigned short CLandscapePatchTesselator::AddCompositeVertex(CPatchTesselationEdgeStrip* strip, long index, CPatchTesselationEdgeStrip* adjacent_strip, long adjacent_index)
{
    CPatchTesselationEdgeStrip::CVertex& adjacent = adjacent_strip->GetVert(adjacent_index);
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(index);
    CPatchTesselationEdgeStrip::CVertex& next = strip->GetVert(index + 1);
    long new_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexLandscapeBackground& v = BridgeVertexBuffer[new_index];
    v.X = adjacent.WorldX;
    v.Y = adjacent.WorldY;
    v.Z = adjacent.WorldZ;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(v, adjacent.Normal);
    if (strip->PeekIsVertical()) {
        v.U1 = vert.U;
        v.V1 = (unsigned char)GFFloatToLongNear(vert.V + (float)(next.V - vert.V) * (adjacent.WorldY - vert.WorldY) / (next.WorldY - vert.WorldY));
    } else {
        v.U1 = (unsigned char)GFFloatToLongNear(vert.U + (float)(next.U - vert.U) * (adjacent.WorldX - vert.WorldX) / (next.WorldX - vert.WorldX));
        v.V1 = vert.V;
    }
    return (unsigned short)new_index;
}

void CLandscapePatchTesselator::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices)
{
    long length = vertical ? Data->PeekHeight() : Data->PeekWidth();
    const CEngineMap* map = Data->PeekEngineMap();
    long world_x = Data->PeekWorldX();
    long world_y = Data->PeekWorldY();
    long width = Data->PeekWidth();
    long height = Data->PeekHeight();
    CPatchTesselationEdgeStrip* adjacent_strip = map->GetLandscapeMap()->GetEdgeStrip(x, y, length, side);
    long adjacent_index = 0;
    if (adjacent_strip) adjacent_index = adjacent_strip->FindVert(vertical ? world_y : world_x);
    long previous = -1;
    for (long i = 0; i < strip->PeekVertexCount() - 1; i++) {
        CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(i);
        long start = strip->GetMainAxisVertexWorldPos(i);
        long end = strip->GetMainAxisVertexWorldPos(i + 1);
        if (end - start <= 1) {
            previous = -1;
        } else {
        bool found = false;
        long fan = -1;
        if (adjacent_strip) {
        while (adjacent_strip->GetMainAxisVertexWorldPos(adjacent_index) < end) {
            if (adjacent_strip->GetMainAxisVertexWorldPos(adjacent_index) > start) {
                found = true;
                if (fan < 0) fan = AddFanBaseVertex(strip, i);
                if (previous < 0) previous = AddDuplicateEdgeVertex(strip, i);
                long added = AddCompositeVertex(strip, i, adjacent_strip, adjacent_index);
                if (flip) AddPoly(fan, added, previous);
                else AddPoly(fan, previous, added);
                previous = added;
            }
            long pos = adjacent_strip->GetMainAxisVertexWorldPos(adjacent_index);
            if (++adjacent_index == adjacent_strip->PeekVertexCount()) {
                if (vertical) adjacent_strip = map->GetLandscapeMap()->GetEdgeStrip(x, pos, height - pos + world_y, side);
                else adjacent_strip = map->GetLandscapeMap()->GetEdgeStrip(pos, y, width - pos + world_x, side);
                if (!adjacent_strip) break;
                adjacent_index = adjacent_strip->FindVert(pos) + 1;
            }
        }
        if (found) {
            long added = AddDuplicateEdgeVertex(strip, i + 1);
            if (flip) AddPoly(fan, added, previous);
            else AddPoly(fan, previous, added);
            previous = added;
            indices[vert.IndexBufferIndex] = strip->GetFanBaseVert(vert.FanBaseIndex).VertexBufferIndex;
        } else {
            previous = -1;
            indices[vert.IndexBufferIndex] = vert.VertexBufferIndex;
        }
        }
        }
    }
}

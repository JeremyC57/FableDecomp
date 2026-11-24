// CLandscapePatchTesselator::TesselateEdge(CPatchTesselationEdgeStrip* strip, long x, long y, bool vertical, bool flip, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices) -- bridge one landscape edge strip to the adjacent patches' strips: for every span of the strip longer than one unit, fan the adjacent strips' vertices that fall inside it around the span's fan base vertex, then redirect the span's edge index
// Grade: DIFFER(2392v2390) -- 25 dd blocks, all in the head: retail loads strip->Verts.Data twice at the loop top (vert at 0x00BE3A61, start/end at 0x00BE3A75) and indexes start as [Data+t]; ours CSEs one Data load through vert. Params / locals carry FableWin.pdb S_BPREL32 names (module Engine_landscape_tesselator.obj).
// Next: see NOTES.md / ESCALATE.md B24C12 (c2 traces: retail's loop top = two unpaired Data loads; only __assume / volatile between vert0 and start reproduce it).
// Retail-asm: GFFloatToLongNear (engine/GFFloatToLongNear.h) at 0x00BE3DD0 / 0x00BE3E1F `fld dword ptr [esp+44h] ; fistp dword ptr [esp+48h/4Ch]` (float homed, bare dword fistp, low byte stored): a row that matches grades retail_asm.
// Retail: Fable.exe @ 0x00BE3970, 2390 bytes (true extent; 0x00BE42D0 is a separate function), thiscall `ret 1Ch`, frame 0x6C. Identity: vtable 0x012A2B3C slot 0;
//   Ego_r.pdb 0x009C3350 / FableWin.pdb 0x02CBCC00. Helpers (FableWin.pdb, no retail copy): AddFanBaseVertex, AddDuplicateEdgeVertex, AddCompositeVertex,
//   CLandscapeTesselatorBase::AddPoly, CPatchTesselationEdgeStrip::FindVert / GetVert / GetFanBaseVert / GetMainAxisVertexWorldPos / PeekVertexCount.
//   Callees: CEngineLandscapeMap::GetEdgeStrip 0x00BDC980 (x2), vector<CTVertexLandscapeBackground>::_M_insert_overflow 0x00B6D0B0 (3 expanded push_backs) and
//   push_back 0x00BE4380 (the 4th, called), CLandscapeBackgroundPatch::SetTargetVertexNormal 0x00BE66F0 (x4), vector<unsigned short>::_M_insert_overflow 0x005135D0 (x10).
//   Grades with FABLE_HDR_OVERLAY=$W/hdr (CPatchTesselationEdgeStrip.h FINDVERT + FINDWATERVERT_EARLYOUT, CEdgeStipVertexArray.h INDEX_ASSERT (Inferred),
//   CLandscapeTesselatorBase.h ADDPOLY, CLandscapePatchTesselator.h EDGE_HELPERS, CEngineLandscapeMap.h GETEDGESTRIP, CWaterPatchTesselatorBase.h).
// Leaf: ?TesselateEdge@CLandscapePatchTesselator@@EAEXPAVCPatchTesselationEdgeStrip@@JJ_N1W4LANDSCAPE_EDGE_STRIP_SIDE@@PAG@Z
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT
#define FABLE_RETAIL_CLANDSCAPETESSELATORBASE_ADDPOLY
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

FABLE_FORCEINLINE unsigned short CLandscapePatchTesselator::AddFanBaseVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long vert_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexLandscapeBackground& new_vert = BridgeVertexBuffer[vert_index];
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(index);
    CPatchTesselationEdgeStrip::CFanBaseVertex& fan_vert = strip->GetFanBaseVert(vert.FanBaseIndex);
    new_vert.X = fan_vert.WorldX;
    new_vert.Y = fan_vert.WorldY;
    new_vert.Z = fan_vert.WorldZ;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(new_vert, fan_vert.Normal);
    new_vert.U1 = fan_vert.U;
    new_vert.V1 = fan_vert.V;
    return (unsigned short)vert_index;
}

FABLE_FORCEINLINE unsigned short CLandscapePatchTesselator::AddDuplicateEdgeVertex(CPatchTesselationEdgeStrip* strip, long index)
{
    long vert_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexLandscapeBackground& new_vert = BridgeVertexBuffer[vert_index];
    CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(index);
    new_vert.X = vert.WorldX;
    new_vert.Y = vert.WorldY;
    new_vert.Z = vert.WorldZ;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(new_vert, vert.Normal);
    new_vert.U1 = vert.U;
    new_vert.V1 = vert.V;
    return (unsigned short)vert_index;
}

FABLE_FORCEINLINE unsigned short CLandscapePatchTesselator::AddCompositeVertex(CPatchTesselationEdgeStrip* strip, long index, CPatchTesselationEdgeStrip* adj_strip, long adj_index)
{
    CPatchTesselationEdgeStrip::CVertex& avert = adj_strip->GetVert(adj_index);
    CPatchTesselationEdgeStrip::CVertex& vert0 = strip->GetVert(index);
    CPatchTesselationEdgeStrip::CVertex& vert1 = strip->GetVert(index + 1);
    long vert_index = BridgeVertexBuffer.size();
    BridgeVertexBuffer.push_back();
    CTVertexLandscapeBackground& new_vert = BridgeVertexBuffer[vert_index];
    new_vert.X = avert.WorldX;
    new_vert.Y = avert.WorldY;
    new_vert.Z = avert.WorldZ;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(new_vert, avert.Normal);
    if (strip->PeekIsVertical()) {
        new_vert.U1 = vert0.U;
        new_vert.V1 = (unsigned char)GFFloatToLongNear(vert0.V + (float)(vert1.V - vert0.V) * (avert.WorldY - vert0.WorldY) / (vert1.WorldY - vert0.WorldY));
    } else {
        new_vert.U1 = (unsigned char)GFFloatToLongNear(vert0.U + (float)(vert1.U - vert0.U) * (avert.WorldX - vert0.WorldX) / (vert1.WorldX - vert0.WorldX));
        new_vert.V1 = vert0.V;
    }
    return (unsigned short)vert_index;
}

void CLandscapePatchTesselator::TesselateEdge(CPatchTesselationEdgeStrip* strip, long strip_x, long strip_y, bool is_left_right_edge, bool is_right_top_edge, LANDSCAPE_EDGE_STRIP_SIDE opposite_edge, unsigned short* pindex)
{
    long strip_size = is_left_right_edge ? Data->PeekHeight() : Data->PeekWidth();
    const CEngineMap* engine_map = Data->PeekEngineMap();
    long world_x = Data->PeekWorldX();
    long world_y = Data->PeekWorldY();
    long width = Data->PeekWidth();
    long height = Data->PeekHeight();
    CPatchTesselationEdgeStrip* adj_strip = engine_map->GetLandscapeMap()->GetEdgeStrip(strip_x, strip_y, strip_size, opposite_edge);
    long adj_strip_index = 0;
    if (adj_strip) {
        adj_strip_index = adj_strip->FindVert(is_left_right_edge ? world_y : world_x);
    }
    long last_new_edge_vertex = -1;
    for (long n = 0; n < strip->PeekVertexCount() - 1; n++) {
        CPatchTesselationEdgeStrip::CVertex& vert0 = strip->GetVert(n);
        long vert0_main_axis_pos = strip->GetMainAxisVertexWorldPos(n);
        long vert1_main_axis_pos = strip->GetMainAxisVertexWorldPos(n + 1);
        if (vert1_main_axis_pos - vert0_main_axis_pos <= 1) {
            last_new_edge_vertex = -1;
        } else {
            bool poly_needs_hiding = false;
            long duplicate_fan_vertex = -1;
            if (adj_strip) {
                while (adj_strip->GetMainAxisVertexWorldPos(adj_strip_index) < vert1_main_axis_pos) {
                    if (adj_strip->GetMainAxisVertexWorldPos(adj_strip_index) > vert0_main_axis_pos) {
                        poly_needs_hiding = true;
                        if (duplicate_fan_vertex < 0) duplicate_fan_vertex = AddFanBaseVertex(strip, n);
                        if (last_new_edge_vertex < 0) last_new_edge_vertex = AddDuplicateEdgeVertex(strip, n);
                        long new_vertex = AddCompositeVertex(strip, n, adj_strip, adj_strip_index);
                        if (is_right_top_edge) AddPoly(duplicate_fan_vertex, new_vertex, last_new_edge_vertex);
                        else AddPoly(duplicate_fan_vertex, last_new_edge_vertex, new_vertex);
                        last_new_edge_vertex = new_vertex;
                    }
                    long old_pos = adj_strip->GetMainAxisVertexWorldPos(adj_strip_index);
                    if (++adj_strip_index == adj_strip->PeekVertexCount()) {
                        if (is_left_right_edge) adj_strip = engine_map->GetLandscapeMap()->GetEdgeStrip(strip_x, old_pos, height - old_pos + world_y, opposite_edge);
                        else adj_strip = engine_map->GetLandscapeMap()->GetEdgeStrip(old_pos, strip_y, width - old_pos + world_x, opposite_edge);
                        if (!adj_strip) break;
                        adj_strip_index = adj_strip->FindVert(old_pos) + 1;
                    }
                }
                if (poly_needs_hiding) {
                    long new_vertex = AddDuplicateEdgeVertex(strip, n + 1);
                    if (is_right_top_edge) AddPoly(duplicate_fan_vertex, new_vertex, last_new_edge_vertex);
                    else AddPoly(duplicate_fan_vertex, last_new_edge_vertex, new_vertex);
                    last_new_edge_vertex = new_vertex;
                    pindex[vert0.IndexBufferIndex] = strip->GetFanBaseVert(vert0.FanBaseIndex).VertexBufferIndex;
                } else {
                    last_new_edge_vertex = -1;
                    pindex[vert0.IndexBufferIndex] = vert0.VertexBufferIndex;
                }
            }
        }
    }
}

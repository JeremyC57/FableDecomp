#ifndef FABLE_ENGINE_CPATCHTESSELATIONEDGESTRIP_H
#define FABLE_ENGINE_CPATCHTESSELATIONEDGESTRIP_H
// Retail layout (hand-written, chunk 706): RTTI .?AVCPatchTesselationEdgeStrip@@ : CBaseClassNonCopyable : CBaseClass, vtable
// 0x012A8318; Ego_r.pdb sizeof 0x2C with StartPos +0x04, Size +0x06, the four CEdgeStipVertexArray<T> members +0x08..+0x27 and
// Tesselated +0x28 / Vertical +0x29 (struct_layouts_egor.tsv); the four vertex types are nested structs (class key U in both PDB
// name sets). Checked against retail: CLandscapeBackgroundPatch::GetEdgeStrip 0x00BE5C60 returns +0x08 / +0x34 / +0x60 / +0x8C
// for EdgeStrips[0..3] (stride 0x2C). Member functions are not modelled yet.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEdgeStipVertexArray.h"

class CPatchTesselationEdgeStrip : public CBaseClassNonCopyable {
public:
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    unsigned short                             StartPos;            // +0x04
    unsigned short                             Size;                // +0x06
    CEdgeStipVertexArray<CVertex>              Verts;               // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>       FanBaseVerts;        // +0x10
    CEdgeStipVertexArray<CWaterVertex>         WaterVerts;          // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex>  WaterFanBaseVerts;   // +0x20
    bool                                       Tesselated;          // +0x28
    bool                                       Vertical;            // +0x29
};

FABLE_STATIC_ASSERT(sizeof(CPatchTesselationEdgeStrip) == 0x2C);

#endif // FABLE_ENGINE_CPATCHTESSELATIONEDGESTRIP_H

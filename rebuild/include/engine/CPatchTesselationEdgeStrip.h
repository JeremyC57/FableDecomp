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
#ifdef FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS
    // Retail opt-in (chunk 785, rows 0x00BE4660 / 0x00BE4FE0 CWaterPatchTesselatorBase<T>::TesselateEdge): the Ego_r.pdb inline
    // accessors (?GetWaterVertex@..@@QAEAAUCWaterVertex@1@K@Z, ?GetVert@, ?GetFanBaseVert@, ?GetWaterFanBaseVert@, ?FindWaterVert@..QBEKK@Z,
    // ?GetMainAxisVertexWorldPos@..QAEJK@Z, ?PeekIsVertical@, ?PeekSize@, ?PeekEndPos@, ?PeekWaterVertexCount@); retail has no
    // out-of-line copy of any of them (both tesselators expand them in place).
    inline CWaterVertex& GetWaterVertex(unsigned long index);
    inline CVertex& GetVert(unsigned long index);
    inline CFanBaseVertex& GetFanBaseVert(unsigned long index);
    inline CWaterFanBaseVertex& GetWaterFanBaseVert(unsigned long index);
    inline unsigned long FindWaterVert(unsigned long pos) const;
    inline long GetMainAxisVertexWorldPos(unsigned long index);
    bool PeekIsVertical() const { return Vertical; }
    long PeekSize() const { return Size; }
    long PeekEndPos() const { return StartPos + Size; }
    long PeekWaterVertexCount() const { return WaterVerts.Size(); }
#endif
};
#ifdef FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_ACCESSORS
// The four nested vertex structs (class key U in both PDB name sets; Ego_r.pdb layouts, struct_layouts_egor.tsv), checked against the
// retail tesselators: CVertex 0x14 (WorldX +8 / WorldY +0x0A / FanBaseIndex +0x10), CFanBaseVertex 0x10 (WorldX +8 / WorldY +0x0A),
// CWaterVertex 0x3C (EdgeVertexBufferIndex +0x34, EdgeIndexBufferIndex +0x36, FanBaseIndex +0x38, LandscapeVertexIndex +0x3A),
// CWaterFanBaseVertex 0x38 (VertexBufferIndex +0x34).
struct CPatchTesselationEdgeStrip::CVertex {
    float          WorldZ;                                      // +0x00
    unsigned long  Normal;                                      // +0x04
    unsigned short WorldX;                                      // +0x08
    unsigned short WorldY;                                      // +0x0A
    unsigned short VertexBufferIndex;                           // +0x0C
    unsigned short IndexBufferIndex;                            // +0x0E
    unsigned short FanBaseIndex;                                // +0x10
    unsigned char  U;                                           // +0x12
    unsigned char  V;                                           // +0x13
};
struct CPatchTesselationEdgeStrip::CFanBaseVertex {
    float          WorldZ;                                      // +0x00
    unsigned long  Normal;                                      // +0x04
    unsigned short WorldX;                                      // +0x08
    unsigned short WorldY;                                      // +0x0A
    unsigned short VertexBufferIndex;                           // +0x0C
    unsigned char  U;                                           // +0x0E
    unsigned char  V;                                           // +0x0F
};
struct CPatchTesselationEdgeStrip::CWaterVertex {
    float          Z;                                           // +0x00
    float          ShoreLookUpArray[12];                        // +0x04
    unsigned short EdgeVertexBufferIndex;                       // +0x34
    unsigned short EdgeIndexBufferIndex;                        // +0x36
    unsigned short FanBaseIndex;                                // +0x38
    unsigned short LandscapeVertexIndex;                        // +0x3A
};
struct CPatchTesselationEdgeStrip::CWaterFanBaseVertex {
    float          Z;                                           // +0x00
    float          ShoreLookUpArray[12];                        // +0x04
    unsigned short VertexBufferIndex;                           // +0x34
};
inline CPatchTesselationEdgeStrip::CWaterVertex& CPatchTesselationEdgeStrip::GetWaterVertex(unsigned long index) { return WaterVerts[index]; }
inline CPatchTesselationEdgeStrip::CVertex& CPatchTesselationEdgeStrip::GetVert(unsigned long index) { return Verts[index]; }
inline CPatchTesselationEdgeStrip::CFanBaseVertex& CPatchTesselationEdgeStrip::GetFanBaseVert(unsigned long index) { return FanBaseVerts[index]; }
inline CPatchTesselationEdgeStrip::CWaterFanBaseVertex& CPatchTesselationEdgeStrip::GetWaterFanBaseVert(unsigned long index) { return WaterFanBaseVerts[index]; }
__forceinline unsigned long CPatchTesselationEdgeStrip::FindWaterVert(unsigned long pos) const
{
    unsigned long i = 0;
    if (WaterVerts.Size()) {
        if (Vertical) {
            for (; i < (unsigned long)WaterVerts.Size(); i++)
                if (Verts[WaterVerts[i].LandscapeVertexIndex].WorldY >= pos) break;
        } else {
            for (; i < (unsigned long)WaterVerts.Size(); i++)
                if (Verts[WaterVerts[i].LandscapeVertexIndex].WorldX >= pos) break;
        }
    }
    return i;
}
inline long CPatchTesselationEdgeStrip::GetMainAxisVertexWorldPos(unsigned long index)
{
    return Vertical ? Verts[index].WorldY : Verts[index].WorldX;
}
#endif

FABLE_STATIC_ASSERT(sizeof(CPatchTesselationEdgeStrip) == 0x2C);

#endif // FABLE_ENGINE_CPATCHTESSELATIONEDGESTRIP_H

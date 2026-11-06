#ifndef FABLE_ENGINE_CTESSELATIONDATA_H
#define FABLE_ENGINE_CTESSELATIONDATA_H
// Retail layout (hand-written, chunk 785): RTTI .?AVCTesselationData@@ : CBaseClass (ghidra_out/rtti_bases.tsv), vtable 0x012A800C
// (slot 0 scalar deleting dtor 0x00BE8D20). Members from Ego_r.pdb (struct_layouts_egor.tsv, sizeof 0x5C, no containers, so the
// retail offsets are the donor's): PatchWorldX +0x04, PatchWorldY +0x08, Width +0x0C, Height +0x10, EngineMap +0x14, IndexBuffer
// +0x18, EdgeStrips[4] +0x1C (CLandscapeTesselatorBase::TesselatePatch 0x00BE3910 walks +0x1C..+0x28), BridgeVertexBuffers[4]
// +0x2C, BridgeIndexBuffers[4] +0x3C, BridgePolyCount[4] +0x4C. The water tesselators 0x00BE4660 / 0x00BE4FE0 read Width +0x0C /
// Height +0x10 / EngineMap +0x14 through the inline Peek accessors (Ego_r.pdb ?PeekWidth@CTesselationData@@QAEJXZ,
// ?PeekHeight@..QAEJXZ, ?PeekEngineMap@..QAEPBVCEngineMap@@XZ; no out-of-line copy in retail). Other members are declared only.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
class CEngineMap;
class CIndexBuffer;
class CVertexBufferWin32;
class CPatchTesselationEdgeStrip;
class CTesselationData : public CBaseClass {
public:
    CTesselationData(const CEngineMap* map, long world_x, long world_y, unsigned short width, unsigned short height);
    virtual ~CTesselationData();
    long PeekWorldX() { return PatchWorldX; }
    long PeekWorldY() { return PatchWorldY; }
    long PeekWidth() { return Width; }
    long PeekHeight() { return Height; }
    const CEngineMap* PeekEngineMap() { return EngineMap; }
private:
    long                        PatchWorldX;                     // +0x04
    long                        PatchWorldY;                     // +0x08
    long                        Width;                           // +0x0C
    long                        Height;                          // +0x10
    CEngineMap*                 EngineMap;                       // +0x14
    CIndexBuffer*               IndexBuffer;                     // +0x18
    CPatchTesselationEdgeStrip* EdgeStrips[4];                   // +0x1C
    CVertexBufferWin32*         BridgeVertexBuffers[4];          // +0x2C
    CIndexBuffer*               BridgeIndexBuffers[4];           // +0x3C
    long                        BridgePolyCount[4];              // +0x4C
};
FABLE_STATIC_ASSERT(sizeof(CTesselationData) == 0x5C);

#endif // FABLE_ENGINE_CTESSELATIONDATA_H

#ifndef FABLE_ENGINE_CLANDSCAPETESSELATORBASE_H
#define FABLE_ENGINE_CLANDSCAPETESSELATORBASE_H
// Hand-written (chunk 723): CLandscapeTesselatorBase, polymorphic root (RTTI .?AVCLandscapeTesselatorBase@@, no bases) of
// CLandscapePatchTesselator and CWaterPatchTesselatorBase<T>. Ego_r.pdb members: BridgeIndexBuffer +0x04 (CArray<unsigned short>,
// 12 bytes in retail), Data +0x10 (retail; Ego_r +0x14); retail sizeof 0x14. The five private virtuals are pure here; their slot
// order is the derived vtables' (0x012A3334 / 0x012A334C: 0 TesselateEdge 0x00BE4660, 1 GetBridgeVertexBufferSize 0x00B7EC40,
// 2 GetBridgeVertexBufferAsCArray 0x00B7EC60, 3 GetSizeOfVertex 0x00B7EC30, 4 SetBridgeVertexBufferSize 0x00B7FCE0). No virtual
// destructor: Ego_r.pdb's ??1CLandscapeTesselatorBase@@QAE@XZ is the implicit non-virtual one, which stores no vtable (the
// CEngineWaterRenderer destructor 0x00B71880 frees +0x14 then +0x04 of each tesselator and nothing else).
#include "rebuild_abi.h"
#include "engine/CArray.h"
class CMovableResource;
class CPatchTesselationEdgeStrip;
class CTesselationData;
enum LANDSCAPE_EDGE_STRIP_SIDE;                                          // MS forward enum
class CLandscapeTesselatorBase {
public:
#ifdef FABLE_RETAIL_CLANDSCAPETESSELATORBASE_INLINE_CTOR
    // Retail opt-in (wfE6, row 0x00B73760): Ego_r.pdb ??0CLandscapeTesselatorBase@@QAE@XZ is the implicit ctor, expanded in place by the
    // CEngineWaterRenderer ctor (0x00B73A54: BridgeIndexBuffer zeros, the base vtable store dead); no out-of-line copy in retail.
    // Retail opt-in (wfE7, row 0x00B69000): ??0CLandscapeTesselatorBase@@QAE@XZ inline in retail (the CEngineLandscapeRenderer ctor zeroes
    // BridgeIndexBuffer at +0x670..+0x678 in place and leaves Data +0x67C unset; the base vtable store is dead under the derived one).
    CLandscapeTesselatorBase() { }
#else
    CLandscapeTesselatorBase();                                          // ??0CLandscapeTesselatorBase@@QAE@XZ
#endif
    bool TesselatePatch(CTesselationData* data, CMovableResource* owner);   // ?TesselatePatch@CLandscapeTesselatorBase@@QAE_NPAVCTesselationData@@PAVCMovableResource@@@Z
protected:
    void AddPoly(long a, long b, long c);                                // ?AddPoly@CLandscapeTesselatorBase@@IAEXJJJ@Z
    bool BuildBridgeVertexAndIndexBuffers(LANDSCAPE_EDGE_STRIP_SIDE side, CMovableResource* owner);
    unsigned char* GetBridgeIndexBufferAsCArray();                       // ?GetBridgeIndexBufferAsCArray@CLandscapeTesselatorBase@@IAEPAEXZ
    long GetBridgeIndexBufferSize();                                     // ?GetBridgeIndexBufferSize@CLandscapeTesselatorBase@@IAEJXZ
    void SetBridgeIndexBufferSize(long size);                            // ?SetBridgeIndexBufferSize@CLandscapeTesselatorBase@@IAEXJ@Z
    bool TesselateEdgeGeneric(LANDSCAPE_EDGE_STRIP_SIDE side, CPatchTesselationEdgeStrip* strip, CMovableResource* owner);
private:
    virtual void TesselateEdge(CPatchTesselationEdgeStrip* strip, long a, long b, bool c, bool d, LANDSCAPE_EDGE_STRIP_SIDE side, unsigned short* indices) = 0;   // slot 0
    virtual long GetBridgeVertexBufferSize() = 0;                        // slot 1
    virtual unsigned char* GetBridgeVertexBufferAsCArray() = 0;          // slot 2
    virtual long GetSizeOfVertex() = 0;                                  // slot 3
    virtual void SetBridgeVertexBufferSize(long size) = 0;               // slot 4
public:
    CArray<unsigned short> BridgeIndexBuffer;                            // +0x04
    CTesselationData*      Data;                                         // +0x10
};
FABLE_STATIC_ASSERT_AT(37, sizeof(CLandscapeTesselatorBase) == 0x14);
#ifdef FABLE_RETAIL_CLANDSCAPETESSELATORBASE_ADDPOLY
// Retail opt-in (B24C2, rows 0x00BE4660 / 0x00BE4FE0): ?AddPoly@CLandscapeTesselatorBase@@IAEXJJJ@Z (FableWin.pdb 84 bytes / 28 instructions,
// equal to this body's plain /Od build). Retail Fable.exe has no out-of-line copy (the tesselators expand the three
// CArray<unsigned short>::push_back calls in place); needs FABLE_RETAIL_CARRAY_PUSHBACK.
FABLE_FORCEINLINE void CLandscapeTesselatorBase::AddPoly(long a, long b, long c)
{
    BridgeIndexBuffer.push_back((unsigned short)a);
    BridgeIndexBuffer.push_back((unsigned short)b);
    BridgeIndexBuffer.push_back((unsigned short)c);
}
#endif

#endif // FABLE_ENGINE_CLANDSCAPETESSELATORBASE_H

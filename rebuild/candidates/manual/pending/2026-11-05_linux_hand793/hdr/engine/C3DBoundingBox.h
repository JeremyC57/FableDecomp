#ifndef FABLE_ENGINE_C3DBOUNDINGBOX_H
#define FABLE_ENGINE_C3DBOUNDINGBOX_H
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX
#include "stlport/fable_containers.h"
#endif
// Layout: Ego_r TPI C3DBoundingBox has precisely two C3DVector members;
// ReadMapInfoBlock writes all 24 bytes at block+0x40, caller copies six words.
class C3DBoundingBox {
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX
    // Retail branch (pass 12 header wave). BuildFromPoints(const vector<C3DVector>&): Ego_r 0x00BD0D30 / FableWin 0x033145B0
    // ?BuildFromPoints@C3DBoundingBox@@QAEXABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z, out of line in retail
    // (0x00AC2BE0 calls it). GetCenter: Ego_r/FableWin ?GetCenter@C3DBoundingBox@@QBE?BVC3DVector@@XZ, inline in retail;
    // GetLightsInArea 0x00B46600..0x00B46645 adds Min.X + Max.X but Max.Y + Min.Y / Max.Z + Min.Z, then scales by 0.5f
    // (0x0122F59C): (MaxCoord + MinCoord) * 0.5f (chunk 721 round 4); 0x00AC2BE0 is byte-identical with that form.
public:
    void BuildFromPoints(const std::vector<C3DVector>&);
    const C3DVector GetCenter() const { return (MaxCoord + MinCoord) * 0.5f; }
    // CalcShortestDistanceToPointSquared: Ego_r `?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z`;
    //   declared only -- its body carries the engine's hand-SSE kernel, so each retail_asm row that expands it defines it
    //   (landed 0x00BF7DF0, chunk 721 0x00B46280).
    float CalcShortestDistanceToPointSquared(const C3DVector& point) const;
    // FableWin ?PeekMinCoord@C3DBoundingBox@@QBEABVC3DVector@@XZ / ?PeekMaxCoord@..: inline (no out-of-line copy in Fable.exe); the
    // members are private, so callers that read the corners (GetLightsInArea 0x00B46280) go through them.
    const C3DVector& PeekMinCoord() const { return MinCoord; }
    const C3DVector& PeekMaxCoord() const { return MaxCoord; }
#endif
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX_OBB
    // Chunk 793 (row 0x00ADE080): Ego_r.pdb ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z -- the axis-aligned box
    // around a box transformed by a 3x4 matrix (an SSE path behind g_SSEAvailable 0x013D2880, else x87).
    void InitialiseFromOBB(const C3DBoundingBox& obb, const class CMatrix3x4& transform);
#endif
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif

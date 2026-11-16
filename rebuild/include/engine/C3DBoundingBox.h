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
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX_GETDIMENTIONS
    // Overlay opt-in (wfA4, row 0x00BAA870): FableWin.pdb ?GetDimentions@C3DBoundingBox@@QBE?BVC3DVector@@XZ (debug copy 15i/35b; no
    // Ego_r.pdb or Fable.exe out-of-line copy): the corner difference. Body only; no layout change.
    const C3DVector GetDimentions() const { return MaxCoord - MinCoord; }
#endif
#if defined(FABLE_RETAIL_C3DBOUNDINGBOX_GETCENTER_PROBE1)
    const C3DVector GetCenter() const { return MinCoord + GetDimentions() * 0.5f; }
#elif defined(FABLE_RETAIL_C3DBOUNDINGBOX_GETCENTER_PROBE2)
    const C3DVector GetCenter() const { C3DVector centre = (MaxCoord + MinCoord) * 0.5f; return centre; }
#elif defined(FABLE_RETAIL_C3DBOUNDINGBOX_GETCENTER_PROBE3)
    const C3DVector GetCenter() const { return (MaxCoord + MinCoord) / 2.0f; }
#elif defined(FABLE_RETAIL_C3DBOUNDINGBOX_CENTER_MIN_FIRST)
    const C3DVector GetCenter()const{return (MinCoord+MaxCoord)*0.5f;}
#else
    const C3DVector GetCenter() const { return (MaxCoord + MinCoord) * 0.5f; }
#endif
    // CalcShortestDistanceToPointSquared: Ego_r `?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z`;
    //   declared only -- its body carries the engine's hand-SSE kernel, so each retail_asm row that expands it defines it
    //   (landed 0x00BF7DF0, chunk 721 0x00B46280).
    float CalcShortestDistanceToPointSquared(const C3DVector& point) const;
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX_PASS22_DISTANCE
    // Both original PDBs; native 00B9C500, expanded in 00BF7BC0.
    float CalcShortestDistanceToPoint(const C3DVector& point) const;
#endif
    // FableWin ?PeekMinCoord@C3DBoundingBox@@QBEABVC3DVector@@XZ / ?PeekMaxCoord@..: inline (no out-of-line copy in Fable.exe); the
    // members are private, so callers that read the corners (GetLightsInArea 0x00B46280) go through them.
    const C3DVector& PeekMinCoord() const { return MinCoord; }
    const C3DVector& PeekMaxCoord() const { return MaxCoord; }
#endif
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX_OBB
    // Row 0x00ADE080: Ego_r.pdb 0x00BCFDA0 / FableWin.pdb 0x03313830 ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z --
    // the axis-aligned box around a box transformed by a 3x4 matrix (an SSE path behind g_SSEAvailable 0x013D2880, else x87).
public:
    void InitialiseFromOBB(const C3DBoundingBox& obb, const class CMatrix3x4& transform);
#endif
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX_SETCENTERDIAG
    // Retail opt-in (wfE8, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData): FableWin.pdb ?SetCenterDiag@C3DBoundingBox@@QAEXABVC3DVector@@0@Z,
    // inline in retail (0x00A30172..0x00A301FF: MinCoord = center - diagonal stored, then MaxCoord = center + diagonal).
public:
    FABLE_FORCEINLINE void SetCenterDiag(const C3DVector& center, const C3DVector& diagonal) { MinCoord = center - diagonal; MaxCoord = center + diagonal; }
#endif
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif

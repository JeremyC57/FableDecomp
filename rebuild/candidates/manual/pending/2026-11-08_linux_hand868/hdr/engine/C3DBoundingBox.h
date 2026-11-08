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
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX_SETCENTERDIAG
    // Nested opt-in (chunk 868, row 0x00A2F740 C3DMeshStats::InitialiseFromFileData): FableWin.pdb ?SetMinMax@C3DBoundingBox@@QAEXABVC3DVector@@0@Z
    // and ?SetCenterDiag@C3DBoundingBox@@QAEXABVC3DVector@@0@Z, inline in retail (0x00A30172..0x00A301FF: MinCoord = center - diagonal,
    // MaxCoord = center + diagonal, from the XBDB chunk's two vectors, each difference / sum through a stack temporary, the minimum
    // stored before the maximum is computed). SetCenterDiag is __forceinline under the owner's 2026-11-01 rule for Lionhead math
    // helpers with no out-of-line copy in Ego_r.pdb or Fable.exe: with the C3DVector operators expanded inside it, plain inline
    // leaves it called out of line at the row's `if (box)` site (chunk 868 grades r3 / r6).
    void SetMinMax(const C3DVector& minCoord, const C3DVector& maxCoord) { MinCoord = minCoord; MaxCoord = maxCoord; }
    __forceinline void SetCenterDiag(const C3DVector& center, const C3DVector& diagonal) { MinCoord = center - diagonal; MaxCoord = center + diagonal; }
#endif
#endif
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif

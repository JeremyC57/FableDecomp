#ifndef FABLE_ENGINE_C3DBOUNDINGBOX_H
#define FABLE_ENGINE_C3DBOUNDINGBOX_H
#include "engine/C3DVector.h"
// Layout: Ego_r TPI C3DBoundingBox has precisely two C3DVector members;
// ReadMapInfoBlock writes all 24 bytes at block+0x40, caller copies six words.
#ifdef FABLE_RETAIL_C3DBOUNDINGBOX
// Retail branch (opt-in): the members are read directly by the inlined accessors below.
// GetCenter: Ego_r/FableWin `?GetCenter@C3DBoundingBox@@QBE?AVC3DVector@@XZ`; retail GetLightsInArea 0x00B46600..0x00B46645
//   adds Min.X + Max.X but Max.Y + Min.Y / Max.Z + Min.Z, then scales by 0.5f (0x0122F59C), which VC7.1 gives for
//   (MaxCoord + MinCoord) * 0.5f.
// CalcShortestDistanceToPointSquared: Ego_r `?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z`;
//   declared only -- its body carries the engine's hand-SSE kernel, so each retail_asm row that expands it defines it
//   (see landed 0x00BF7DF0 and 0x00B46280).
class C3DBoundingBox {
public:
    C3DVector MinCoord;
    C3DVector MaxCoord;
    const C3DVector GetCenter() const { return (MaxCoord + MinCoord) * 0.5f; }
    float CalcShortestDistanceToPointSquared(const C3DVector& point) const;
};
#else
class C3DBoundingBox {
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif
#endif

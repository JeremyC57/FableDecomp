#ifndef FABLE_ENGINE_C3DVERTEX2_H
#define FABLE_ENGINE_C3DVERTEX2_H
// C3DVertex2 (Ego_r class V, size 0x20): Position +0, Normal +0x0C (C3DVector), U +0x18, V +0x1C.
// GetPosition: FableWin ?GetPosition@C3DVertex2@@QBE?BVC3DVector@@XZ 0x032F9E50 (const by-value return), inline in retail.
#include "engine/C3DVector.h"
class C3DVertex2 {
public:
    C3DVector Position,Normal;
    float U,V;
    const C3DVector GetPosition() const { return Position; }
    const C3DVector GetNormal() const { return Normal; }
    float GetU() const { return U; }
    float GetV() const { return V; }
};
#endif

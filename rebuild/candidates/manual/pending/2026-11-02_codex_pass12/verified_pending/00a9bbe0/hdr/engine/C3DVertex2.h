#ifndef FABLE_ENGINE_C3DVERTEX2_H
#define FABLE_ENGINE_C3DVERTEX2_H
#include "engine/C3DVector.h"
// PDB class V, full size32. The const vector getter returns are by value.
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

#ifndef FABLE_ENGINE_C3DPACKEDNORMAL_H
#define FABLE_ENGINE_C3DPACKEDNORMAL_H
#ifdef FABLE_RETAIL_BACKGROUND_VERTEX_HELPERS_PASS28
#include "engine/C3DVector.h"
// Ego_r records the unsigned VectorBits at +0, sizeof 4. FableWin 02CE9960/02CE9990/02CE99D0 prove the constructor, getter and initializer.
class C3DPackedNormal {
public:
    C3DPackedNormal(const C3DVector& normal) { Initialise(normal); }
    unsigned long GetAsPackedInt() const { return VectorBits; }
    void Initialise(const C3DVector&);
private:
    unsigned long VectorBits;
};
#endif
#endif

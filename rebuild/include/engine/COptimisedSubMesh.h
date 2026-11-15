#ifndef FABLE_ENGINE_COPTIMISEDSUBMESH_H
#define FABLE_ENGINE_COPTIMISEDSUBMESH_H
#ifdef FABLE_RETAIL_COPTIMISEDSUBMESH_PASS22
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/COptimisedBone.h"
namespace QuadricOptimiserInternals {
class COptimisedPrimitive;
// Ego_r members Primitives+0/Bones+16; native STLport arrays are12bytes, so Bones+12.
// FableWin034CB4A0 original destructor destroys Bones then Primitives.
class COptimisedSubMesh {public:
    CArray<CCountedPointer<COptimisedPrimitive> > Primitives;
    CArray<COptimisedBone> Bones;
    ~COptimisedSubMesh();
};
FABLE_STATIC_ASSERT(sizeof(COptimisedSubMesh)==24);
}
#endif
#endif

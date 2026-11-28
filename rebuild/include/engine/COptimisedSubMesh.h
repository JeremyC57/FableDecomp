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
#ifdef FABLE_RETAIL_QUADRIC_MESH_CTOR_PASS30
    ~COptimisedSubMesh() {}
#else
    ~COptimisedSubMesh();
#endif
};
FABLE_STATIC_ASSERT_AT(16, sizeof(COptimisedSubMesh)==24);
}
#ifdef FABLE_RETAIL_QUADRIC_MESH_CTOR_PASS30
// The original mesh deleting destructor calls this 37-byte vector owner at 00AD83A0.
namespace std {
template<> vector<QuadricOptimiserInternals::COptimisedSubMesh>::~vector();
}
#endif
#endif
#endif

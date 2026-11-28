#ifndef FABLE_ENGINE_COPTIMISEDMESH_H
#define FABLE_ENGINE_COPTIMISEDMESH_H
#include "engine/C3DOptimisedMeshBase.h"
#include "engine/COptimisedSubMesh.h"
namespace QuadricOptimiserInternals {
class COptimisedMesh : public C3DOptimisedMeshBase {
public:
    COptimisedMesh(C3DMeshFile&);
    virtual void RemoveBones(const std::set<CCharString>&);
    virtual void CopyTo(C3DMeshFile&);
private:
    CArray<COptimisedSubMesh> SubMeshes;
};
}
FABLE_STATIC_ASSERT_AT(43193,sizeof(QuadricOptimiserInternals::COptimisedMesh)==16);
#endif

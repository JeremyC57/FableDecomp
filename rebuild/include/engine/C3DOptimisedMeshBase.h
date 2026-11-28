#ifndef FABLE_ENGINE_C3DOPTIMISEDMESHBASE_H
#define FABLE_ENGINE_C3DOPTIMISEDMESHBASE_H
#include "engine/CBaseClassNonCopyable.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
class C3DMeshFile;
class C3DOptimisedMeshBase : public CBaseClassNonCopyable {
public:
    virtual ~C3DOptimisedMeshBase() {}
    virtual void RemoveBones(const std::set<CCharString>&) = 0;
    virtual void CopyTo(C3DMeshFile&) = 0;
};
FABLE_STATIC_ASSERT_AT(43192,sizeof(C3DOptimisedMeshBase)==4);
#endif

#ifndef FABLE_ENGINE_COPTIMISEDBONE_H
#define FABLE_ENGINE_COPTIMISEDBONE_H
#ifdef FABLE_RETAIL_COPTIMISEDSUBMESH_PASS22
#include "fable_string.h"
#include "rebuild_abi.h"
namespace QuadricOptimiserInternals {
// Original Ego_r member names/types and offsets; native00AD1B20 copies the same16-byte record.
class COptimisedBone {public:
#ifdef FABLE_RETAIL_QUADRIC_MESH_CTOR_PASS30
    COptimisedBone(const CCharString& name,long parent) : BoneName(name),Parent(parent),Remapped(false),NewIndex(-1) {}
#endif
    CCharString BoneName;
    long Parent;
    bool Remapped;
    long NewIndex;
};
FABLE_STATIC_ASSERT_AT(14, sizeof(COptimisedBone)==16);
}
#endif
#endif

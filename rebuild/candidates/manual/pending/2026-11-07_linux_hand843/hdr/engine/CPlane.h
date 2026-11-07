#ifndef FABLE_ENGINE_CPLANE_H
#define FABLE_ENGINE_CPLANE_H
// Retail layout (chunk 843): Ego_r.pdb CPlane (struct_layouts_egor.tsv: Normal C3DVector +0x0, Distance float +0xC, sizeof 0x10;
// class key V in pdb_class_keys.tsv). Initialise overloads from Ego_r.pdb: ?Initialise@CPlane@@QAE_NABVC3DVector@@M@Z (normal and
// distance; out of line in retail at 0x00A42140, called by CEngineCamera::SetupGamut 0x00B2FD60 as `push d ; push &normal`),
// ?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z (three points) and ?Initialise@CPlane@@QAE_NABVC3DVector@@0@Z (point and normal).
// Constructors: FableWin.pdb ??0CPlane@@QAE@XZ, ??0CPlane@@QAE@ABVC3DVector@@M@Z and the user copy constructor ??0CPlane@@QAE@ABV0@@Z
// (debug copies; no Ego_r.pdb or retail out-of-line copy: CIPhysicsFacesSegmentBase::CalculateDividingPlane 0x00A86110 builds its
// by-value result in place, Normal then Distance). A new header (no includer before chunk 843).
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CPlane {
public:
    CPlane() { }
    CPlane(const C3DVector& normal, float distance) : Normal(normal), Distance(distance) { }
    CPlane(const CPlane& other) : Normal(other.Normal), Distance(other.Distance) { }
    bool Initialise(const C3DVector& normal, float distance);                         // 0x00A42140
    bool Initialise(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2);
    bool Initialise(const C3DVector& point, const C3DVector& normal);
    C3DVector Normal;                                                                  // +0x0
    float Distance;                                                                    // +0xC
};
FABLE_STATIC_ASSERT(sizeof(CPlane) == 0x10);
#endif // FABLE_ENGINE_CPLANE_H

// Leaf: ?Sizeof@CPhysicsFacesSegmentBranch@@MBEJXZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CPhysicsFacesSegmentBranch.h under FABLE_RETAIL_CPHYSICSFACESSEGMENTBRANCH_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CPhysicsFacesSegmentBranch_Methods::Sizeof); the retail RTTI vftable placement names this PDB member here. The local
//   CIPhysicsFacesSegmentBase model (dtor slot 0, Sizeof slot 1) is dropped: the opt-in pulls engine/CIPhysicsFacesSegmentBase.h (Sizeof slot 1, +4).
#define FABLE_RETAIL_CPHYSICSFACESSEGMENTBRANCH_VIRTUAL_MODEL
#include "engine/CPhysicsFacesSegmentBranch.h"  // retyped onto the PDB layout; byte parity re-verified

long CPhysicsFacesSegmentBranch::Sizeof() const {
    return this->PositiveChild->Sizeof() + this->NegativeChild->Sizeof() + 0x1c;
}

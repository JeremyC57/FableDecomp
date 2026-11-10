#ifndef FABLE_ENGINE_NCAMERAMODES_CPHYSICSSTATE_H
#define FABLE_ENGINE_NCAMERAMODES_CPHYSICSSTATE_H
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
// Original complete class: both PDBs64 bytes with two12-byte lists; native
// uses two4-byte STLport lists, preserving all original fields at size48.
namespace NCameraModes { class CPhysicsState {
public:
 C3DVector Position,Velocity,Acceleration;
 std::list<std::pair<C3DVector,float> > Attractors,IncidentalAttractors;
 bool CullPhysics;
}; }
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CPhysicsState)==48);
#endif

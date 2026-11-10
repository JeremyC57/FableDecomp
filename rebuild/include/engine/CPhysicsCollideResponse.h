#ifndef FABLE_ENGINE_CPHYSICSCOLLIDERESPONSE_H
#define FABLE_ENGINE_CPHYSICSCOLLIDERESPONSE_H
// Both original PDB records and native006afe60: size28, fields0,12,24.
#include "engine/C3DVector.h"
class CPhysicsCollideResponse {
public:
    C3DVector CollidePos;
    C3DVector CollideNormal;
    float Friction;
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsCollideResponse)==28);
#endif

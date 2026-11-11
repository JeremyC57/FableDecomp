#ifndef FABLE_ENGINE_CPHYSICSCOLLIDERESPONSE_H
#define FABLE_ENGINE_CPHYSICSCOLLIDERESPONSE_H
// Both original PDB records and native006afe60: size28, fields0,12,24.
#include "engine/C3DVector.h"
class CPhysicsCollideResponse {
public:
#ifdef FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR
    // Retail opt-in (chunk 2004, row 0x00725500): Ego_r.pdb ??0CPhysicsCollideResponse@@QAE@XZ, inline in retail -- only Friction is
    // set (1.0f stored once at entry, 0x0072556D; the landed 0x00725290 UpdateOnSurface model's constructor). No layout change.
    CPhysicsCollideResponse() : Friction(1.0f) { }
#endif
    C3DVector CollidePos;
    C3DVector CollideNormal;
    float Friction;
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsCollideResponse)==28);
#endif

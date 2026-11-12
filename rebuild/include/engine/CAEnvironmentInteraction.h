#ifndef FABLE_ENGINE_CAENVIRONMENTINTERACTION_H
#define FABLE_ENGINE_CAENVIRONMENTINTERACTION_H
#include "engine/C3DVector.h"
#include "fable_string.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CAEnvironmentInteraction {
public:
 virtual ~CAEnvironmentInteraction();
 virtual bool IsParticleCollidingAt(const C3DVector&,const C3DVector&,const bool,C3DVector&,C3DVector&,bool&)const=0;
 virtual bool IsWindActive()const;
 virtual C3DVector GetWindVelocity()const=0;
 virtual C3DVector GetGravityVector()const;
 virtual C3DVector GetVectorParam(const CCharString&)const;
 virtual void GetCameraPos(C3DVector&)const=0;
};
}
#endif

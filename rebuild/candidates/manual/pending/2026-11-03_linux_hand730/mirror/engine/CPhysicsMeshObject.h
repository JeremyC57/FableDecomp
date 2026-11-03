#ifndef FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#include "engine/CBaseClass.h"
class CCharString;class C3DVector;class CMeshIntersectionData;
// Ego_r TPI size40, CBaseClass base, Mesh+4, PositionMatrix+8, Scale+38.
// Native GetPhysicsMeshObject 006b0850 and destructor00a127d0 agree.
class CPhysicsMeshObject : public CBaseClass {
private:
    unsigned char Mesh[4]; // CIVCountedPointer<const CPhysicsMesh>
    unsigned char PositionMatrix[48]; // CMatrix3x4
    float Scale;
    float OO_Scale;
public:
    virtual ~CPhysicsMeshObject();
    bool HasContainmentVolume(const CCharString&) const;
    bool GetPointIntersectionFromTo(const C3DVector&,const C3DVector&,CMeshIntersectionData&) const;
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==0x40);
#endif

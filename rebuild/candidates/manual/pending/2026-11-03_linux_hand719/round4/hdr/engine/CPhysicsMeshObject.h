#ifndef FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#include "engine/CBaseClass.h"
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT
#include "stlport/fable_containers.h"
#endif
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
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT
    // Retail opt-in additions (2026-11-03, chunk 719 row 0x00509A20, agent719c): declarations only (Ego_r.pdb 0x00B7F2C0 /
    // 0x00B7F200, FableWin.pdb 0x031D19C0 / 0x031D1900); no layout change.
    void GetAllHelperPointNames(std::vector<CCharString>& names) const;                 // 0x00A11820 ?GetAllHelperPointNames@CPhysicsMeshObject@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;   // 0x00A11760 ?GetHelperPointPositionStatic@CPhysicsMeshObject@@QBE?AVC3DVector@@ABVCCharString@@J@Z
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==0x40);
#endif

#ifndef FABLE_ENGINE_CIPHYSICSFACESSEGMENTBASE_H
#define FABLE_ENGINE_CIPHYSICSFACESSEGMENTBASE_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CPhysicsMesh;
class C3DVector;
// Both original PDBs: Ego_r TPI0xd4a5, FableWin TPI0xd2b3, size4.
// The five original virtual slots occupy0,4,8,12,16; no instance data.
class CIPhysicsFacesSegmentBase {
public:
    virtual bool HasChildren() const = 0;
    virtual long Sizeof() const = 0;
    virtual long GetNoFaces() const = 0;
    virtual ~CIPhysicsFacesSegmentBase();
    // Native00a86ea0: thiscall, five stack parameters,ret20; both PDB symbols QBEX.
    void GetCollidedWithTriangles(const CPhysicsMesh*,float,const C3DVector&,
        const C3DVector&,std::vector<long>&) const;
protected:
    virtual void GetPossibleCollisionSet(float,const C3DVector&,const C3DVector&,
        std::vector<long>&) const = 0;
};
FABLE_STATIC_ASSERT(sizeof(CIPhysicsFacesSegmentBase)==4);
#endif

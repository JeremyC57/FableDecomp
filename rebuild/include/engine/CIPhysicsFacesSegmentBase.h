#ifndef FABLE_ENGINE_CIPHYSICSFACESSEGMENTBASE_H
#define FABLE_ENGINE_CIPHYSICSFACESSEGMENTBASE_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CIPHYSICS_DIVIDING_PLANE_PASS24
#include "fable_compiler.h"
#include "engine/CArray.h"
#include "engine/CPlane.h"
#include "engine/C3DTriangle2.h"
#endif
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
#ifdef FABLE_RETAIL_CIPHYSICS_DIVIDING_PLANE_PASS24
    // Retail opt-in (Codex pass 24): both PDBs spell the protected static at 0x00A86110.
    static CPlane FABLE_FASTCALL CalculateDividingPlane(const CArray<C3DVector>& point_list, const CArray<C3DTriangle2>& faces_list, const CArray<unsigned short>& triangle_indices, long level);
#endif
    virtual void GetPossibleCollisionSet(float,const C3DVector&,const C3DVector&,
        std::vector<long>&) const = 0;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CIPhysicsFacesSegmentBase)==4);
#endif

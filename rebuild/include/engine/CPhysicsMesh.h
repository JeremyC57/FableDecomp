#ifndef FABLE_ENGINE_CPHYSICSMESH_H
#define FABLE_ENGINE_CPHYSICSMESH_H
// Retail model (Codex 2026-11-10): both original PDB class records identify
// every member below. Native 00a17620 reads BoundingSphere at0x74 and Radius at0x80.
// Composite members unused by this row retain their complete native storage.
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/C3DBoundingSphere.h"
class CPhysicsMesh : public CIVCountedPointeeBase {
public:
    virtual ~CPhysicsMesh();
    const C3DBoundingSphere& GetBoundingSphere() const { return BoundingSphere; }
    bool HasBoundingSphereIntersectedFromTo(float sphere_radius,
        const C3DVector& from, const C3DVector& to) const;
private:
    unsigned long ReferenceCount;                     //0x08
    bool NavigationLineListInfo;                       //0x0c
    unsigned char _pad_0d[3];
    unsigned char HelperPoints[16];                    //0x10 CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> >
    unsigned char DummyObjects[16];                    //0x20 CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >
    unsigned char TotalFaceIndices[12];                //0x30 CArray<C3DTriangle2>
    unsigned char TotalPoints[12];                     //0x3c CArray<C3DVector>
    unsigned char NavigationLines[12];                 //0x48 CArray<CArray<C2DLineF> >
    unsigned char NavigationContainmentVolumes[16];    //0x54 CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >
    unsigned char ContainmentVolumes[16];              //0x64 CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<CCharString,C3DMeshVolume> >
    C3DBoundingSphere BoundingSphere;                  //0x74
    unsigned char BoundingBox[24];                    //0x84 C3DBoundingBox
    unsigned char BoundingCircle[12];                 //0x9c CCircle
    C3DVector CentreOfGravity;                         //0xa8
    unsigned char RootSegment[8];                     //0xb4 CCountedPointer<const CIPhysicsFacesSegmentBase>
    class CMovableResourceMemoryPool* MemoryPool;       //0xbc
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMesh)==0xc0);
#endif

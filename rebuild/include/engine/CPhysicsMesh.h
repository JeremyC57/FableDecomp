#ifndef FABLE_ENGINE_CPHYSICSMESH_H
#define FABLE_ENGINE_CPHYSICSMESH_H
// Retail model (Codex 2026-11-10): both original PDB class records identify
// every member below. Native 00a17620 reads BoundingSphere at0x74 and Radius at0x80.
// Composite members unused by this row retain their complete native storage.
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/C3DBoundingSphere.h"
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
#include "engine/CVectorMap.h"
class C3DMeshVolume;
#endif
class CPhysicsMesh : public CIVCountedPointeeBase {
public:
    virtual ~CPhysicsMesh();
    const C3DBoundingSphere& GetBoundingSphere() const { return BoundingSphere; }
    bool HasBoundingSphereIntersectedFromTo(float sphere_radius,
        const C3DVector& from, const C3DVector& to) const;
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
    // Retail opt-in (wf5, row 0x00A11D10 CPhysicsMeshObject::GetNavigationLevelAt): the member's Ego_r.pdb type (same offset and
    // size), FableWin.pdb's inline ?GetNavigationContainmentVolumes@CPhysicsMesh@@QBEABV?$CVectorMap@JVC3DMeshVolume@@..@@XZ and
    // Ego_r.pdb / FableWin.pdb ?GetNavigationLevelAt@CPhysicsMesh@@QBEJABVC3DVector@@@Z (out of line at retail 0x00A73D10).
    const CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >& GetNavigationContainmentVolumes() const { return NavigationContainmentVolumes; }
    long GetNavigationLevelAt(const C3DVector& position) const;   // 0x00A73D10
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME
    // ?IsInContainmentVolume@CPhysicsMesh@@QBE_NABVCCharString@@ABVC3DVector@@M@Z, out of line at retail 0x00A6CA20 (landed), and
    // ?PeekRoughProportionOfSphereContained@CPhysicsMesh@@QBEMABVCCharString@@ABVC3DVector@@M@Z at 0x00A6CA80. Declarations only.
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const;   // 0x00A6CA20
    float PeekRoughProportionOfSphereContained(const CCharString& name, const C3DVector& position, float radius) const;   // 0x00A6CA80
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_BOUNDINGBOXINTERSECTED
    // Retail opt-in (wf5, row 0x00A11F20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox): Ego_r.pdb / FableWin.pdb
    // ?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z, out of line at retail 0x00A17690 (landed). Declaration
    // only: no layout change.
    bool HasBoundingBoxIntersectedFromTo(float sphere_radius,
        const C3DVector& from, const C3DVector& to) const;   // 0x00A17690
#endif
private:
    unsigned long ReferenceCount;                     //0x08
    bool NavigationLineListInfo;                       //0x0c
    unsigned char _pad_0d[3];
    unsigned char HelperPoints[16];                    //0x10 CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> >
    unsigned char DummyObjects[16];                    //0x20 CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >
    unsigned char TotalFaceIndices[12];                //0x30 CArray<C3DTriangle2>
    unsigned char TotalPoints[12];                     //0x3c CArray<C3DVector>
    unsigned char NavigationLines[12];                 //0x48 CArray<CArray<C2DLineF> >
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
    CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> > NavigationContainmentVolumes;    //0x54
#else
    unsigned char NavigationContainmentVolumes[16];    //0x54 CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >
#endif
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

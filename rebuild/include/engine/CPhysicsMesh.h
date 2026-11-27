#ifndef FABLE_ENGINE_CPHYSICSMESH_H
#define FABLE_ENGINE_CPHYSICSMESH_H
// Retail model (Codex 2026-11-10): both original PDB class records identify
// every member below. Native 00a17620 reads BoundingSphere at0x74 and Radius at0x80.
// Composite members unused by this row retain their complete native storage.
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/C3DBoundingSphere.h"
#ifdef FABLE_RETAIL_CPHYSICSMESH_BOUNDS
#include "engine/C3DBoundingBox.h"
#include "engine/CCircle.h"
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_GETRIGIDBODYBOUNDINGPOINTS
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
#include "engine/CVectorMap.h"
class C3DMeshVolume;
#endif
#if defined(FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO) && !defined(FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23)
#include "engine/CArray.h"
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS
#include "engine/CArray.h"
#include "engine/C2DLineF.h"
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#include "engine/CArray.h"
#include "engine/C3DTriangle2.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CIPhysicsFacesSegmentBase.h"
#include "engine/CCountedPointer.h"
#endif
#ifdef FABLE_RETAIL_PHYSICS_MESH_FILE_MAPS_PASS29
#include "engine/C3DHelperPoint.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CSymbolMap.h"
class C3DMeshFile;
#endif
class CPhysicsMesh : public CIVCountedPointeeBase {
public:
    virtual ~CPhysicsMesh();
#ifdef FABLE_RETAIL_CPHYSICSMESH_CAST_PASS29
    bool GetPointIntersectionFromTo(const C3DVector&, const C3DVector&, CMeshIntersectionData&) const;
    bool GetSphereIntersectionFromTo(float, const C3DVector&, const C3DVector&, CMeshIntersectionData&) const;
#endif
    const C3DBoundingSphere& GetBoundingSphere() const { return BoundingSphere; }
    bool HasBoundingSphereIntersectedFromTo(float sphere_radius,
        const C3DVector& from, const C3DVector& to) const;
#ifdef FABLE_RETAIL_CPHYSICSMESH_SIZEOF
    // NF68 opt-in (row 0x00A25BD0 CPhysicsMeshInfo::BeginLoading): Ego_r.pdb 0x00B4D8F0 / FableWin.pdb 0x030F8DD0
    // ?Sizeof@CPhysicsMesh@@QBEJXZ (retail 0x00A6C900, out of line; the landed CPhysicsMeshInfo::Sizeof 0x00A26370 binds it). Declaration only.
    long Sizeof() const;                                                 // 0x00A6C900
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
    // Retail opt-in (wf5, row 0x00A11D10 CPhysicsMeshObject::GetNavigationLevelAt): the member's Ego_r.pdb type (same offset and
    // size), FableWin.pdb's inline ?GetNavigationContainmentVolumes@CPhysicsMesh@@QBEABV?$CVectorMap@JVC3DMeshVolume@@..@@XZ and
    // Ego_r.pdb / FableWin.pdb ?GetNavigationLevelAt@CPhysicsMesh@@QBEJABVC3DVector@@@Z (out of line at retail 0x00A73D10).
    const CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >& GetNavigationContainmentVolumes() const { return NavigationContainmentVolumes; }
    long GetNavigationLevelAt(const C3DVector& position) const;   // 0x00A73D10
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS
    // NF394 opt-in (rows 0x00A73CF0 / 0x00A127C0): Ego_r.pdb (0x00C12630, lib_physics_mesh_2_navigation.obj) / FableWin.pdb (0x033C7680)
    // ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ (public, const, long), out of line at retail 0x00A73CF0 (the only referrer is
    // CPhysicsMeshObject::GetNumberOfNavigationLineLevels 0x00A127C0's tail jump): NavigationLines.size() (FableWin's body calls
    // ?size@?$vector@V?$CArray@VC2DLineF@@@@..@std@@QBEIXZ on the member). With it NavigationLines is typed (Ego_r.pdb
    // CArray<CArray<C2DLineF> >, same 12 bytes at +0x48). Declaration only otherwise.
    long GetNumberOfNavigationLineLevels() const;                        // 0x00A73CF0
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_BOUNDS
    // Row 0x00724630: FableWin.pdb inline ?GetBoundingBox@CPhysicsMesh@@QBEABVC3DBoundingBox@@XZ and
    // ?GetBoundingCircle@CPhysicsMesh@@QBEABVCCircle@@XZ (the members below at +0x84 / +0x9c; CCircle from engine/CCircle.h).
    // No layout change.
    const C3DBoundingBox& GetBoundingBox() const { return BoundingBox; }
    const CCircle& GetBoundingCircle() const { return BoundingCircle; }
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME
    // ?IsInContainmentVolume@CPhysicsMesh@@QBE_NABVCCharString@@ABVC3DVector@@M@Z, out of line at retail 0x00A6CA20 (landed), and
    // ?PeekRoughProportionOfSphereContained@CPhysicsMesh@@QBEMABVCCharString@@ABVC3DVector@@M@Z at 0x00A6CA80. Declarations only.
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const;   // 0x00A6CA20
    float PeekRoughProportionOfSphereContained(const CCharString& name, const C3DVector& position, float radius) const;   // 0x00A6CA80
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_GETRIGIDBODYBOUNDINGPOINTS
    // Nested opt-in (NF138, q4802; rows 0x00A6CB20 and its caller 0x00A12840 CPhysicsMeshObject::GetRigidBodyBoundingPoints): Ego_r.pdb
    // 0x00B4DCE0 / FableWin.pdb 0x030F7040 ?GetRigidBodyBoundingPoints@CPhysicsMesh@@QBEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
    // (public, const; the points by STLport vector reference), out of line in retail at 0x00A6CB20. Declaration only: no layout change.
    void GetRigidBodyBoundingPoints(std::vector<C3DVector>& points) const;   // 0x00A6CB20
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_BOUNDINGBOXINTERSECTED
    // Retail opt-in (wf5, row 0x00A11F20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox): Ego_r.pdb / FableWin.pdb
    // ?HasBoundingBoxIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z, out of line at retail 0x00A17690 (landed). Declaration
    // only: no layout change.
    bool HasBoundingBoxIntersectedFromTo(float sphere_radius,
        const C3DVector& from, const C3DVector& to) const;   // 0x00A17690
#endif
private:
#ifdef FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO
    // Retail opt-in (egod2a, row 0x00A6C640): Ego_r.pdb / FableWin.pdb ?CalculateBoundingInfo@CPhysicsMesh@@AAEXXZ (private).
    // With it TotalPoints is typed (CArray<C3DVector>, same 12 bytes at +0x3C). Declaration only.
    void CalculateBoundingInfo();                                        // 0x00A6C640
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_FULL_COLLISION_PASS23
    // Both PDBs: private const; native caller00a187ac and debug031994f0.
    bool GetPointIntersectionFromToUsingFullCollision(const C3DVector& from,
        const C3DVector& to, CMeshIntersectionData& data) const;
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
    // Both original PDBs: private const methods; caller00a18798 and getter00a6cbb0.
    bool GetPointIntersectionFromToUsingSegmentation(const C3DVector& from,
        const C3DVector& to, CMeshIntersectionData& data) const;
    std::vector<long>& GetSingleInstanceCollisionSet() const;
    // Original FableWin03199990 and native inline body00a17822..a17a45.
    // Inline-budget: native00a17822..a17a45 expansion; ordinary/flags/depth fail in recorded probes.
    FABLE_FORCEINLINE bool HasCollidedWithTriangleAt(const C3DTriangle2&,const C3DVector&,
        const C3DVector&,float,C3DVector&,C3DVector&) const;
#endif
#ifdef FABLE_RETAIL_PHYSICS_MESH_FILE_MAPS_PASS29
    void GetHelperDataFromFileData(const C3DMeshFile&);
    void GetContainmentDataFromFileData(const C3DMeshFile&);
#endif
    unsigned long ReferenceCount;                     //0x08
    bool NavigationLineListInfo;                       //0x0c
    unsigned char _pad_0d[3];
#ifdef FABLE_RETAIL_PHYSICS_MESH_FILE_MAPS_PASS29
    CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> > HelperPoints;
#else
    unsigned char HelperPoints[16];                    //0x10 CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> >
#endif
#ifdef FABLE_RETAIL_PHYSICS_MESH_FILE_MAPS_PASS29
    CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> > DummyObjects;
#else
    unsigned char DummyObjects[16];                    //0x20 CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
    // Original field types in both PDBs; native00a17822/+30,00a1782f/+3c.
    CArray<C3DTriangle2> TotalFaceIndices;
    CArray<C3DVector> TotalPoints;
#elif defined(FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO)
    unsigned char TotalFaceIndices[12];                //0x30 CArray<C3DTriangle2>
    CArray<C3DVector> TotalPoints;                     //0x3c
#else
    unsigned char TotalFaceIndices[12];                //0x30 CArray<C3DTriangle2>
    unsigned char TotalPoints[12];                     //0x3c CArray<C3DVector>
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATIONLINELEVELS
    CArray<CArray<C2DLineF> > NavigationLines;         //0x48
#else
    unsigned char NavigationLines[12];                 //0x48 CArray<CArray<C2DLineF> >
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
    CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> > NavigationContainmentVolumes;    //0x54
#else
    unsigned char NavigationContainmentVolumes[16];    //0x54 CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >
#endif
#ifdef FABLE_RETAIL_PHYSICS_MESH_FILE_MAPS_PASS29
    CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<CCharString,C3DMeshVolume> > ContainmentVolumes;
#else
    unsigned char ContainmentVolumes[16];              //0x64 CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<CCharString,C3DMeshVolume> >
#endif
    C3DBoundingSphere BoundingSphere;                  //0x74
#ifdef FABLE_RETAIL_CPHYSICSMESH_BOUNDS
    C3DBoundingBox BoundingBox;                        //0x84
    CCircle BoundingCircle;                            //0x9c
#else
    unsigned char BoundingBox[24];                    //0x84 C3DBoundingBox
    unsigned char BoundingCircle[12];                 //0x9c CCircle
#endif
    C3DVector CentreOfGravity;                         //0xa8
#ifdef FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
    // Original const pointee in both PDBs; native00a177b6 reads+B4.
    CCountedPointer<const CIPhysicsFacesSegmentBase> RootSegment;
#else
    unsigned char RootSegment[8];                     //0xb4 CCountedPointer<const CIPhysicsFacesSegmentBase>
#endif
    class CMovableResourceMemoryPool* MemoryPool;       //0xbc
};
FABLE_STATIC_ASSERT_AT(77, sizeof(CPhysicsMesh)==0xc0);
#endif

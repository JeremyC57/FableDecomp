#ifndef FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#define FABLE_ENGINE_CPHYSICSMESHOBJECT_H
#include "engine/CBaseClass.h"
class CCharString;class C3DVector;class CMeshIntersectionData;
// Ego_r TPI size40, CBaseClass base, Mesh+4, PositionMatrix+8, Scale+38.
// Native GetPhysicsMeshObject 006b0850 and destructor00a127d0 agree.
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#include "engine/CMatrix3x4.h"
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT
#include "engine/CIVCountedPointer.h"
class CPhysicsMesh;
#endif
class CPhysicsMeshObject : public CBaseClass {
private:
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT
    // Retail opt-in (chunk 734, row 0x004A1CB0 CWorld::PreparePrimitivesForEngine): the member's Ego_r.pdb type (same offset and
    // size) and FableWin.pdb's inline ?GetMesh@CPhysicsMeshObject@@QBE?AV?$CIVCountedPointer@$$CBVCPhysicsMesh@@@@XZ (no Ego_r.pdb
    // copy; PreparePrimitivesForEngine copies the pointer and AddRefs it in place, 0x004A2084..8B). Needs FABLE_RETAIL_CIVCOUNTEDPOINTER
    // (the counted copy constructor) and the includer's complete CPhysicsMesh for the returned pointer's operator->.
    CIVCountedPointer<const CPhysicsMesh> Mesh;
public:
    CIVCountedPointer<const CPhysicsMesh> GetMesh() const { return Mesh; }
private:
#else
    unsigned char Mesh[4]; // CIVCountedPointer<const CPhysicsMesh>
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
    // Chunk 756 (row 0x00A11C20): the member's Ego_r.pdb type (CMatrix3x4, same offset and size).
    CMatrix3x4 PositionMatrix;
#else
    unsigned char PositionMatrix[48]; // CMatrix3x4
#endif
    float Scale;
    float OO_Scale;
public:
    virtual ~CPhysicsMeshObject();
    bool HasContainmentVolume(const CCharString&) const;
    bool GetPointIntersectionFromTo(const C3DVector&,const C3DVector&,CMeshIntersectionData&) const;
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION
    // Retail opt-in (chunk Q8, row 0x006A4FB0 CThingPhysical::GetFirstCollision): Ego_r.pdb / FableWin.pdb
    // ?GetSphereIntersectionFromTo@CPhysicsMeshObject@@QBE_NMABVC3DVector@@0AAVCMeshIntersectionData@@@Z, out of line at retail
    // 0x00A120D0. Declaration only: no layout change.
    bool GetSphereIntersectionFromTo(float,const C3DVector&,const C3DVector&,CMeshIntersectionData&) const;   // 0x00A120D0
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES
    // Chunk 756 (rows 0x00875F00 / 0x00A11C20): Ego_r.pdb ?GetBoundingBoxXEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ (0x00A11C20)
    // and ?GetBoundingBoxYEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ (0x00A11C90), out of line, by value.
    C3DVector GetBoundingBoxXEdge() const;
    C3DVector GetBoundingBoxYEdge() const;
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION
    // Retail opt-in (chunk Q23, row 0x0063F7C0 ForEachInRoughArea<CThingPhysical, CFilter_Bind3<...>, CThingFindNavigationLayerAt>):
    // Ego_r.pdb ?GetNavigationLevelAt@CPhysicsMeshObject@@QBEJABVC3DVector@@@Z and ?IsPointContainedInBoundingBox@CPhysicsMeshObject@@
    // QBE_NABVC3DVector@@@Z, out of line in retail at 0x00A11D10 / 0x00A12890 (the catalog's CMainGameComponent::BeginInputLoading at
    // 0x00A12890 is a propagated label; calls at 0x0063F9EB / 0x0063F9A4 on a GetPhysicsMeshObject temporary). Declarations only.
    long GetNavigationLevelAt(const C3DVector& pos) const;               // 0x00A11D10
    bool IsPointContainedInBoundingBox(const C3DVector& pos) const;      // 0x00A12890
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME
    // Ego_r.pdb / FableWin.pdb ?IsInContainmentVolume@CPhysicsMeshObject@@QBE_NABVCCharString@@ABVC3DVector@@M@Z (retail 0x00A128E0)
    // and ?PeekRoughProportionOfSphereContainedInVolume@CPhysicsMeshObject@@QBEMABVCCharString@@ABVC3DVector@@M@Z (retail 0x00A12AB0).
    // Declarations only.
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& pos, float radius) const;   // 0x00A128E0
    float PeekRoughProportionOfSphereContainedInVolume(const CCharString& name, const C3DVector& pos, float radius) const;   // 0x00A12AB0
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS
    // Retail opt-in (agentNF27, rows 0x00A12890 / 0x0063F150 / 0x0063F1E0): Ego_r.pdb / FableWin.pdb
    // ?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z (retail 0x00A11F20: the radius scaled by OO_Scale
    // +0x3C, the point taken into mesh space by the inverse PositionMatrix), the callee of IsPointContainedInBoundingBox 0x00A12890
    // (`push 0 ; push pos ; call 0x00A11F20` with ecx passed through, i.e. the sphere test with a zero radius). IsPointContainedIn-
    // BoundingBox is declared here too unless FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION (above) already declares it. Declarations
    // only: no layout change.
    bool IsSpherePartContainedInBoundingBox(const C3DVector& pos, float radius) const;   // 0x00A11F20
#ifndef FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION
    bool IsPointContainedInBoundingBox(const C3DVector& pos) const;                      // 0x00A12890
#endif
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX
    // Retail opt-in (wf1, row 0x00A114B0): Ego_r.pdb / FableWin.pdb
    // ?SetPositionMatrix@CPhysicsMeshObject@@QAEXABVC3DVector@@ABVCRightHandedSet@@M@Z, out of line at retail 0x00A114B0 (sole caller
    // the constructor 0x00A12CC0). Declaration only: no layout change.
    void SetPositionMatrix(const C3DVector& position,const class CRightHandedSet& axes,float scale);   // 0x00A114B0
#endif
#ifdef FABLE_RETAIL_CPHYSICSMESHOBJECT_CENTRE
    // Nested opt-in (chunk 760, row 0x00A11AE0): Ego_r.pdb ?GetBoundingBoxCentre@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ.
    C3DVector GetBoundingBoxCentre() const;                              // 0x00A11AE0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==0x40);
#endif

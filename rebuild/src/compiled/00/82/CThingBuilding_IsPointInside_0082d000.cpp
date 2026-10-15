// CThingBuilding::IsPointInside(const C3DVector& pos) -- whether a point lies inside the building's physics mesh containment volume
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D000, 90 bytes.
// Callees: CTCPhysicsBase virtual slot 93 (`call [eax+174h]`, vtable 0x0125D2C4 -> 0x006B13B0: the mesh handle, null
//          in the base), CTCPhysicsBase::GetPhysicsMeshObject @ 0x006B0850 (returns a 0x40-byte CPhysicsMeshObject by
//          value through the hidden pointer), CPhysicsMeshObject::IsInContainmentVolume(const CCharString&, const
//          C3DVector&, float) @ 0x00A128E0, ~CPhysicsMeshObject @ 0x00A127D0. Global: the volume-name CCharString @ 0x013BAC98.
// Layout: the physics component pointer at retail +0x60 of the thing (CThing::GetPosition reads `[this+60h] + 0Ch`).
// Shape: the physics pointer for the test and the virtual call is a named local (loaded straight into the dead
//   this-register ecx); the getter re-reads the member after the virtual call. The mesh object is a 0x40-byte frame local (`sub esp,40h`) built in place by the by-value getter; the
//   bool result rides in bl across its destructor; the 0.0f tolerance is pushed as an immediate 0.
class CCharString;
struct C3DVector;

extern CCharString g_containmentVolumeName;     // 0x013BAC98

struct CPhysicsMeshObject {
    unsigned char _body[0x40];
    ~CPhysicsMeshObject();                                                      // 0x00A127D0
    bool IsInContainmentVolume(const CCharString& volume, const C3DVector& pos, float tolerance);  // 0x00A128E0
};

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual const void* GetPhysicsMesh();               // slot 93 (0x006B13B0 in the base: returns 0)
    CPhysicsMeshObject GetPhysicsMeshObject();          // 0x006B0850
};

struct CThingBuilding {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* m_pPhysics;                         // +0x60
    bool IsPointInside(const C3DVector& pos);
};

bool CThingBuilding::IsPointInside(const C3DVector& pos)
{
    CTCPhysicsBase* physics = m_pPhysics;
    if (physics && physics->GetPhysicsMesh()) {
        CPhysicsMeshObject mesh = m_pPhysics->GetPhysicsMeshObject();
        return mesh.IsInContainmentVolume(g_containmentVolumeName, pos, 0.0f);
    }
    return false;
}
// CTCCameraPointDefinitionBase::SetCoordSystem(CThing* thing) -- take the thing's position as CoordBase and its orientation as CoordAxis, each only when the camera point uses relative coordinates / orientation
//
// Retail 0x0075C210, 101 bytes (`ret 4`). Callees: CThing::GetPosition() const @ 0x004C73D0 (returns
// const C3DVector&), virtual slot 72 (+0x120) of the thing's +0x60 object (returns the orientation
// as a const CRightHandedSet&).
// Layout: PDB CTCCameraPointDefinitionBase (packed; same model as the landed ctor 0x0075C180):
// CoordBase +0x16, CoordAxis (Up, Forward) +0x22, UsingRelativeCoords +0x3a, UsingRelativeOrientation +0x3b.
// Shape: plain member assignments with the implicit operator= (dword moves through the `lea` /
// `add esi,22h` destination register).
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
};

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};

struct CPhysics;

struct CThing {
    unsigned char        _pad_0x00[0x60];
    CPhysics*            m_pPhysics;                 // +0x60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};

struct CTCCameraPointDefinitionBase {
    void*           __vftable;                      // +0x00 (CTCBase)
    CThing*         ParentThing;                    // +0x04
    unsigned char   _pad08[4];
    bool            CutInto;                        // +0x0c
    bool            CutOutOf;                       // +0x0d
    bool            TestAngleBeforeActivation;      // +0x0e
    unsigned char   _pad0f;
    float           FOV;                            // +0x10
    bool            SelfTerminate;                  // +0x14
    bool            HeroIsSubject;                  // +0x15
    C3DVector       CoordBase;                      // +0x16
    CRightHandedSet CoordAxis;                      // +0x22
    bool            UsingRelativeCoords;            // +0x3a
    bool            UsingRelativeOrientation;       // +0x3b
    bool            Activated;                      // +0x3c
    unsigned char   _pad3d[3];

    void SetCoordSystem(CThing* thing);
};
#pragma pack(pop)

void CTCCameraPointDefinitionBase::SetCoordSystem(CThing* thing)
{
    if (UsingRelativeCoords)
        CoordBase = thing->GetPosition();
    if (UsingRelativeOrientation)
        CoordAxis = thing->m_pPhysics->GetOrientation();
}
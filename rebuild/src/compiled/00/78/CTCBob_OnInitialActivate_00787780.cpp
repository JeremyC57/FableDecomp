// CTCBob::OnInitialActivate() -- snapshot the parent's physics orientation set and position into Rhset / Pos and clear ForceVisible
//
// Retail 0x00787780, 86 bytes. Callee: the physics component's virtual slot 72 (`call [edx+120h]`,
// returns a const CRightHandedSet&). The parent thing's physics component is CThing +0x60
// (PhysicsTC, Ego_r.pdb +0x70; retail CThing sits 0x10 lower); its Position is CTCPhysicsBase +0x0c.
// Layout (Ego_r.pdb CTCBob): Rhset +0x0c (two C3DVectors, 24 bytes), Pos +0x24, ForceVisible +0x30.
// Shape: the physics pointer is a named local (kept in edi across the virtual call and re-based with
// `add edi,0Ch` for the position); both copies are whole-struct assignments (`lea ecx,[esi+N]` then
// dword moves); the ForceVisible clear is the last statement.

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward, Up; };

struct CTCPhysicsBase
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
    unsigned char _pad_0x04[0x8];                   // +0x04 (CTCBase)
    C3DVector     Position;                         // +0x0c
};

struct CThing
{
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

#pragma pack(push, 1)
struct CTCBob
{
    void*           __vftable;                      // +0x00 (CTCBase)
    CThing*         ParentThing;                    // +0x04 (CTCBase)
    unsigned char   _pad_0x08[0x4];                 // +0x08 (CTCBase flags)
    CRightHandedSet Rhset;                          // +0x0c
    C3DVector       Pos;                            // +0x24
    bool            ForceVisible;                   // +0x30

    void OnInitialActivate();
};
#pragma pack(pop)

void CTCBob::OnInitialActivate()
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    Rhset = physics->GetRHSet();
    Pos = physics->Position;
    ForceVisible = false;
}
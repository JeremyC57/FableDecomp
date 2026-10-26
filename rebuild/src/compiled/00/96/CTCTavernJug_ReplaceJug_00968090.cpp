// CTCTavernJug::ReplaceJug() -- puts the jug back: when the parent thing has a physics component, resets its position to Pos, its orientation to RHSet, and calls slot 36 with false
// Retail: Fable.exe @ 0x00968090, 54 bytes. Callers: CAIStateGroup_TavernServer::OnReplace (0x009674C2).
// Callees: the physics component's virtual slots 31 (+0x7C, CTCPhysicsBase::ResetPosition(const
//          C3DVector&)), 75 (+0x12C, orientation reset; empty in CTCPhysicsBase 0x006B12C0) and 36
//          (+0x90, one argument; empty in CTCPhysicsBase 0x006B10B0) -- slots 75/36 named descriptively.
// Layout: Ego_r.pdb CTCTavernJug (0x38, packed): Fullness +0x0C, OnBar +0x10, RHSet +0x11,
// Pos +0x29 (no engine header; `#pragma pack(1)` reproduces the PDB's unaligned members). The
// parent thing (+4) holds the physics component at +0x60 (as the landed 0x008F2F00).
// Shape: the component is a named local tested once (esi); `&Pos` is `lea eax,[edi+29h]`, `&RHSet`
//        advances `this` in place (`add edi,11h`) because `this` is dead afterwards.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };          // 0x18 bytes

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& pos);           // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetPhysicsActive(bool active);                 // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual void ResetOrientation(const CRightHandedSet& rhSet); // slot 75 (+0x12c)
};

class CThing { public:
    unsigned char _pad[0x60];
    CTCPhysicsBase* m_pPhysics;                                 // +0x60
};

#pragma pack(push, 1)
class CTCTavernJug { public:
    void* __vftable;                                            // +0x00
    CThing* m_pParentThing;                                     // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];
    long Fullness;                                              // +0x0c
    bool OnBar;                                                 // +0x10
    CRightHandedSet RHSet;                                      // +0x11
    C3DVector Pos;                                              // +0x29
    void ReplaceJug();
};
#pragma pack(pop)

void CTCTavernJug::ReplaceJug()
{
    CTCPhysicsBase* physics = m_pParentThing->m_pPhysics;
    if (physics) {
        physics->ResetPosition(Pos);
        physics->ResetOrientation(RHSet);
        physics->SetPhysicsActive(false);
    }
}
// CTCTavernTankard::ReplaceTankard() -- puts the tankard back: resets the parent thing's physics component to Pos and RHSet and calls its slot 36 with false (no null test, unlike ReplaceJug)
// Retail: Fable.exe @ 0x00947C30, 50 bytes. Callers: CAIStateGroup_GoToPub::FreeSeat (0x009487FD),
//         CAIStateGroup_EatAtTable::OnReplace (0x0096D157).
// Callees: the physics component's virtual slots 31 (+0x7C, CTCPhysicsBase::ResetPosition(const
//          C3DVector&)), 32 (+0x80, orientation set; empty in CTCPhysicsBase 0x006B1070) and 36
//          (+0x90; empty in CTCPhysicsBase 0x006B10B0) -- slots 32/36 named descriptively.
// Layout: Ego_r.pdb CTCTavernTankard (0x3C): Fullness +0x0C, RHSet +0x10, Pos +0x28, PBarmaid +0x34
// (no engine header). The parent thing (+4) holds the physics component at +0x60.
// Shape: the component is a named local (esi); `&Pos` is `lea eax,[edi+28h]`, `&RHSet` advances
//        `this` in place (`add edi,10h`). Compare ReplaceJug 0x00968090 (null-tested, slot 75).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };          // 0x18 bytes

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& pos);           // slot 31 (+0x7c)
    virtual void SetOrientation(const CRightHandedSet& rhSet);  // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetPhysicsActive(bool active);                 // slot 36 (+0x90)
};

struct CThing {
    unsigned char _pad[0x60];
    CTCPhysicsBase* m_pPhysics;                                 // +0x60
};

struct CThingAICreature;

struct CTCTavernTankard {
    void* __vftable;                                            // +0x00
    CThing* m_pParentThing;                                     // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];
    float Fullness;                                             // +0x0c
    CRightHandedSet RHSet;                                      // +0x10
    C3DVector Pos;                                              // +0x28
    unsigned char PBarmaid[0x8];                                // +0x34 CIntelligentPointer<CThingAICreature>
    void ReplaceTankard();
};

void CTCTavernTankard::ReplaceTankard()
{
    CTCPhysicsBase* physics = m_pParentThing->m_pPhysics;
    physics->ResetPosition(Pos);
    physics->SetOrientation(RHSet);
    physics->SetPhysicsActive(false);
}
// CCreatureAction_NymphGetHitDie::FrameUpdate() -- base FrameUpdate, then lower the nymph towards the ground: Z = ground height below it + HeightOnInit * (1 - progress)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9F40, length 125 bytes (+ int3 padding).
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// twice), CThing::GetPosition @ 0x004C73D0 (const C3DVector&), the action's progress getter @ 0x006925E0 (float; manifest label
// "CInputTypeMouseMovementEvent::operator==" is propagated), CCreatureActionBase::GetWorldMap @ 0x00692C10,
// CWorldMap::GetHeightZBelowPos(const C3DVector*) @ 0x0063E480, the physics component's virtual slot 30 (`call [edx+78h]` on thing+0x60).
// Shape: the position is copied by value (three integer moves into the frame), `1.0f - progress` is `fsubr [1.0f]`, the height
// offset `HeightOnInit * (...)` is a named float local parked in the frame word below the vector, the new Z is the ground height plus
// that offset; the physics component comes through an inline accessor (straight into ecx). HeightOnInit is the engine header's
// member (PDB +0x160, retail +0x148: CCombatActionBase is 0x114 bytes in retail).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CPhysicsComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPositionOnGround(const C3DVector& position);   // slot 30 (+0x78)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsComponent* m_pPhysics;          // +0x60
    const C3DVector& GetPosition() const;   // 0x004C73D0
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CWorldMap {
    float GetHeightZBelowPos(const C3DVector* position);   // 0x0063E480
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void FrameUpdate();                     // 0x00859D80
    float GetProgress();                    // 0x006925E0
    CWorldMap* GetWorldMap();               // 0x00692C10
};
struct CCreatureAction_NymphGetHitDie : CCombatActionBase {
    unsigned char _pad114[0x148 - 0x114];   // +0x114
    float HeightOnInit;                     // +0x148 (PDB +0x160)
    void FrameUpdate();
};

void CCreatureAction_NymphGetHitDie::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    C3DVector position = PCreature.GetPItem()->GetPosition();
    float height = HeightOnInit * (1.0f - GetProgress());
    position.Z = GetWorldMap()->GetHeightZBelowPos(&position) + height;
    PCreature.GetPItem()->GetPhysics()->SetPositionOnGround(position);
}
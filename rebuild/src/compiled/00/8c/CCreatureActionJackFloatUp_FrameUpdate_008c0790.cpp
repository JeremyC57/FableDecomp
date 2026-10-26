// CCreatureAction_JackFloatUp::FrameUpdate() -- base FrameUpdate; while moving, raise the creature by FloatSpeed per frame, clamped to StartHeight + FloatHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0790, length 140 bytes (+ int3 padding).
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// twice), CThing::GetPosition @ 0x004C73D0 (`[this+60h]+0Ch`, a const C3DVector&), the physics component's virtual slot 31 (`call [edx+7Ch]`
// on thing+0x60, SetPosition(const C3DVector&)). Global: the integer frame rate @ 0x01375550 (`fild`).
// Shape: the per-frame rise is a C3DVector(0, 0, FloatSpeed / (float)frameRate) built before the PCreature fetch: only its Z is
// read, so the two zero stores are dead and dropped but the vector keeps its three frame words (frame 0x1c, rise Z at [esp+20h]; a
// plain float local shares the limit's word and shrinks the frame to 0x10); the new position is an inline-constructed C3DVector (arguments right to left: Z = rise + pos.Z first, then Y on the FPU, X as a dword move);
// the limit `FloatHeight + StartHeight` (right operand loaded first) is a named float local, `fcomp ; test ah,41h ; jne` is
// `Z > limit`, and the clamp copies the limit as a dword. Members from the engine header (PDB +0x12c.. sit 0x18 high: retail
// CCombatActionBase is 0x114 bytes). The physics component is reached through an inline accessor so it loads straight into ecx.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CPhysicsComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
class CThing { public:
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

extern long g_frameRate;                    // 0x01375550 frames per second

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void FrameUpdate();                     // 0x00859D80
};
class CCreatureAction_JackFloatUp : public CCombatActionBase { public:
    float FloatHeight;                      // +0x114 (PDB +0x12c)
    float FloatSpeed;                       // +0x118 (PDB +0x130)
    bool  Moving;                           // +0x11c (PDB +0x134)
    float StartHeight;                      // +0x120 (PDB +0x138)
    void FrameUpdate();
};

void CCreatureAction_JackFloatUp::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (Moving) {
        C3DVector rise(0.0f, 0.0f, FloatSpeed / (float)g_frameRate);
        const C3DVector& position = PCreature.GetPItem()->GetPosition();
        C3DVector target(position.X, position.Y, position.Z + rise.Z);
        float limit = FloatHeight + StartHeight;
        if (target.Z > limit)
            target.Z = limit;
        PCreature.GetPItem()->GetPhysics()->SetPosition(target);
    }
}
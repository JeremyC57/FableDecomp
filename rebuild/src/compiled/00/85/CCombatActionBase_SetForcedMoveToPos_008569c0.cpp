// CCombatActionBase::SetForcedMoveToPos(const C3DVector&) -- flag the preferred final position and store (pos - creature position) as the preferred movement vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008569C0, length 93 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingPhysical::GetPosition
// @ 0x004C73D0 (`[this+60h]+0Ch`, const C3DVector&; the manifest's "CVertexBufferWin32::DoSizeof" is a
// propagated label).
// Members (retail layout, Ego_r.pdb names): PreferredFinalPosSet at +0xdd, PreferredMovementVector at +0x108
// (the PDB build has them at +0xf1/+0x120; retail's CCombatActionBase is 0x114 bytes, so modelled locally).
// Shape: the flag store is scheduled before GetPItem; the inline vector subtraction builds its 12-byte POD
// result in the frame (x and y stored, z computed while the first two dwords are copied) and the member is
// assigned by three register moves; `add esi,108h` forms the member address.
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    C3DVector result;
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    result.Z = a.Z - b.Z;
    return result;
}

struct CThingCreatureBase_Calls {              // engine/CThingCreatureBase.h exists but is not used: only its GetPosition call is used
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Calls* GetPItem() const;       // 0x00A01B50
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xdd - 0x10];          // +0x10
    bool PreferredFinalPosSet;                  // +0xdd
    unsigned char _padde[0x108 - 0xde];         // +0xde
    C3DVector PreferredMovementVector;          // +0x108
    void SetForcedMoveToPos(const C3DVector& pos);
};

void CCombatActionBase::SetForcedMoveToPos(const C3DVector& pos)
{
    PreferredFinalPosSet = true;
    PreferredMovementVector = pos - PCreature.GetPItem()->GetPosition();
}
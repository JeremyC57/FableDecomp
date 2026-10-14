// CBodyOwner_00c55030::Reset() -- release the object at +0x220 (virtual slot 1), reset the two rigid-body records at +0xdc and +0x100 (0x00C6B720), clear the words at +0x22c and +0x238
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55030, length 64 bytes.
// Callees: virtual slot 1 (+4) of the object at +0x220, the record reset @ 0x00C6B720 twice (labelled "CTCPhysicsRigidBody::GetFacingVector", propagated). Shape: one zero
// register for the two clears, `this` in esi. The chunk label "CGameScriptInterface::Validate" is propagated; named by address.
struct CReleasable_00c55030 {
    virtual void v00();
    virtual void Release();                        // slot 1 (+0x04)
};
struct SBodyRecord_00c55030 {
    unsigned char _body[0x24];
    void Reset();                                  // 0x00C6B720
};

struct CBodyOwner_00c55030 {
    unsigned char          _pad[0xdc];
    SBodyRecord_00c55030   m_bodyA;                // +0xdc
    SBodyRecord_00c55030   m_bodyB;                // +0x100
    unsigned char          _pad124[0xfc];
    CReleasable_00c55030*  m_pTarget;              // +0x220
    unsigned char          _pad224[8];
    long                   m_22c;                  // +0x22c
    unsigned char          _pad230[8];
    long                   m_238;                  // +0x238
    void Reset();
};

void CBodyOwner_00c55030::Reset()
{
    m_pTarget->Release();
    m_bodyA.Reset();
    m_bodyB.Reset();
    m_22c = 0;
    m_238 = 0;
}
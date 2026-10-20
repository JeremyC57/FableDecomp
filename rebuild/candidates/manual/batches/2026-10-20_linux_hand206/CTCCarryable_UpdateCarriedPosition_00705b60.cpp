// CTCCarryable::UpdateCarriedPosition(bool forceUpdate) -- move a carried (live) thing to where its carrier holds it: face it along the carried orientation and, when the carried position is on the map and differs from the current one by more than the epsilon on any axis, set the physics position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705B60, 197 bytes (`ret 4`, 0x24-byte frame: the position and the 24-byte CRightHandedSet).
// Callees: CTCCarryable::GetCarriedPositionInfo(C3DVector*, CRightHandedSet*, bool, float) @ 0x007055D0 (time 1.0f),
// CTCBase::PeekWorldMap() const @ 0x00686D20, CWorldMap::IsPosInMap(const C3DVector&) @ 0x004FC600, CThing::GetPosition()
// @ 0x004C73D0, the physics TC's virtual slots 75 SetFacingAnglesFromRHSet (+0x12c) and 30 SetPosition (+0x78);
// g_epsilon_0129ba3c @ 0x0129BA3C (1e-4f).
// Layout: CThing PhysicsTC +0x60 (retail; PDB +0x70), dead bit +0x91.
// Shape: `||` of three `(float)fabs(cur - new) > eps` tests (fabs intrinsic over a double prototype; `test ah,41h ; je` into the
// shared SetPosition block); the current position is a reference local kept in ecx; IsPosInMap's argument is pushed before
// the world-map accessor runs.
#pragma intrinsic(fabs)
extern "C" double __cdecl fabs(double);
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCPhysicsBase {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
    virtual void SetPosition(const C3DVector& position);                  // slot 30 (+0x78)
    V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49)
    V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59) V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69)
    V(70) V(71) V(72) V(73) V(74)
#undef V
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& set);    // slot 75 (+0x12c)
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   Dead : 1;                           // +0x91 bit 0
    unsigned char   _91_1 : 7;
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector& position) const;   // 0x004FC600
};
struct CTCCarryable {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    const CWorldMap& PeekWorldMap() const;              // 0x00686D20 (CTCBase)
    bool GetCarriedPositionInfo(C3DVector* position, CRightHandedSet* orientation, bool forceUpdate, float time);   // 0x007055D0
    void UpdateCarriedPosition(bool forceUpdate);
};

void CTCCarryable::UpdateCarriedPosition(bool forceUpdate)
{
    if (ParentThing->Dead)
        return;
    CRightHandedSet orientation;
    C3DVector position;
    if (!GetCarriedPositionInfo(&position, &orientation, forceUpdate, 1.0f))
        return;
    ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(orientation);
    if (!PeekWorldMap().IsPosInMap(position))
        return;
    const C3DVector& current = ParentThing->GetPosition();
    if ((float)fabs(current.X - position.X) > g_epsilon_0129ba3c
     || (float)fabs(current.Y - position.Y) > g_epsilon_0129ba3c
     || (float)fabs(current.Z - position.Z) > g_epsilon_0129ba3c)
        ParentThing->PhysicsTC->SetPosition(position);
}

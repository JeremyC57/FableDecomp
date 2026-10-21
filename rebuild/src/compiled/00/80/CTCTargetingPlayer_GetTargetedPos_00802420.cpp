// CTCTargetingPlayer::GetTargetedPos(C3DVector& pos, float t) -- in first-person targeting the targeted position (plus its 3D offset) interpolated between the last and the current frame; otherwise the live target's fire point at t; false when there is neither
// Leaf: GetTargetedPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802420, 366 bytes (`ret 8`).
// Callees: this's virtual GetTargetedThing (slot 33, +0x84); CThing::GetPlayer 0x004C7A10; CPlayer::IsFirstPersonTargeting
//          0x004874D0; CTCBase::GetWorld() const 0x00686A80; GetTargetFirePoint(const CWorld*, const CThing*, float) 0x007EA2F0
//          (__fastcall, C3DVector by hidden pointer).
// Layout: rebuild/include/engine/CTCTargetingPlayer.h is the PDB (the two 16-byte PDB vectors at +0x68/+0x78 are 12 bytes in
//          retail, as the landed PostProcessTargetedThing 0x00803B20 notes), so the positions sit 8 bytes lower: TargetedPos
//          +0x84, OldTargetedPos +0x90, TargetedPosOffset3D +0x9c, OldTargetedPosOffset3D +0xa8; local model.
// Shape: `(current - old) * t + old` through C3DVector's inline operators, the offset added with +=.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
struct CPlayer {
    bool IsFirstPersonTargeting();              // 0x004874D0
};
struct CWorld;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CPlayer* GetPlayer();                       // 0x004C7A10
};
C3DVector __fastcall GetTargetFirePoint(const CWorld* world, const CThing* target, float t);   // 0x007EA2F0
struct CTCTargetingPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTargetedThing();         // slot 33 (+0x84)
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x84 - 0x08];
    C3DVector TargetedPos;                      // +0x84
    C3DVector OldTargetedPos;                   // +0x90
    C3DVector TargetedPosOffset3D;              // +0x9c
    C3DVector OldTargetedPosOffset3D;           // +0xa8
    const CWorld* GetWorld() const;             // 0x00686A80 (CTCBase)
    bool GetTargetedPos(C3DVector& pos, float t);
};

bool CTCTargetingPlayer::GetTargetedPos(C3DVector& pos, float t)
{
    CThing* target = GetTargetedThing();
    if (ParentThing->GetPlayer()->IsFirstPersonTargeting()) {
        pos = (TargetedPos - OldTargetedPos) * t + OldTargetedPos;
        pos += (TargetedPosOffset3D - OldTargetedPosOffset3D) * t + OldTargetedPosOffset3D;
        return true;
    }
    if (target && !(target->m_flags91 & 1)) {
        pos = GetTargetFirePoint(GetWorld(), target, t);
        return true;
    }
    return false;
}
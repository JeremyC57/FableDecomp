// CTCCameraPointDefinitionBase::IsAngleValid() const -- for a point that tests its angle before activation: true only when the point's active camera position lies on the same side of the hero as the current game camera (positive 2D dot product)
// Grade: DIFFER(ours v retail) -- x87 term order only: ours multiplies the Y term first (fld tc.Y; fmul st(1)), retail multiplies the X difference by toCamera.X first then the Y term; next: the sum as d = x*x'; d += y*y'
// Leaf: IsAngleValid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C530, length 192 bytes
// (?IsAngleValid@CTCCameraPointDefinitionBase@@ABE_NXZ).
// Callees: CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::GetControlledCreature
// @ 0x00487DD0, the camera manager's (world +0x34) virtual slot 11 (+0x2C, C3DVector by hidden pointer), CThing::GetPosition
// @ 0x004C73D0, the point's own virtual GetActiveCameraPosition (slot 35, +0x8C, C3DVector by hidden pointer); 0.0f @ 0x0122DEDC.
// Layout = Ego_r.pdb CTCCameraPointDefinitionBase: TestAngleBeforeActivation +0x0E.
// Shape: the camera direction from the hero is a 3D local with only x and y filled (its unused z keeps a frame word) built
// before the virtual call; the dot product of the two
// hero-relative directions is compared `> 0.0f`; the early exits return byte constants, the comparison an int-sized 1/0.
struct C3DVector {
    float X, Y, Z;
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CPlayer {
    CThing* GetControlledCreature();            // 0x00487DD0
};
struct CGameCameraManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual C3DVector PeekCurrentPos() const;      // slot 11 (+0x2C) 0x0069E1F0
};
struct CWorld {
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;         // +0x34
    CGameCameraManager* GetCameraManager() const { return PCameraManager; }
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
inline C2DVector DeltaXY(const C3DVector& a, const C3DVector& b)
{
    C2DVector d;
    d.X = a.X - b.X;
    d.Y = a.Y - b.Y;
    return d;
}
class CTCCameraPointDefinitionBase {
public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31)
    VF(32) VF(33) VF(34)
    virtual C3DVector GetActiveCameraPosition() const;   // slot 35 (+0x8C)
    void*   ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    CutInto;                            // +0x0c
    bool    CutOutOf;                           // +0x0d
    bool    TestAngleBeforeActivation;          // +0x0e
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    bool IsAngleValid() const;
};

bool CTCCameraPointDefinitionBase::IsAngleValid() const
{
    if (TestAngleBeforeActivation) {
        CThing* hero = GetWorld()->GetCurrentPlayer()->GetControlledCreature();
        if (hero && !hero->IsDead()) {
            C3DVector camera = GetWorld()->GetCameraManager()->PeekCurrentPos();
            const C3DVector& heroPosition = hero->GetPosition();
            C3DVector toCamera;
            toCamera.X = camera.X - heroPosition.X;
            toCamera.Y = camera.Y - heroPosition.Y;
            C2DVector toActive = DeltaXY(GetActiveCameraPosition(), heroPosition);
            return toActive.X * toCamera.X + toActive.Y * toCamera.Y > 0.0f;
        }
        return false;
    }
    return true;
}

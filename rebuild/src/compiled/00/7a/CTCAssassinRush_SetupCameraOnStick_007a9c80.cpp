// CTCAssassinRush::SetupCameraOnStick() -- read the hero's camera-stick helper point; store it relative to the hero (LocalRelativeVectorForCamera) and the camera relative to it (RelativeVectorForCamera), then hand over to the camera setup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A9C80, length 221 bytes (+ int3 padding).
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560 (on
// ParentThing+0x64, the helper-name CCharString global 0x013BAC4C), CThing::GetPosition @ 0x004C73D0, CTCBase::GetWorld
// @ 0x00686A70, CGameCameraManager::PeekCurrentPos (virtual slot 11, +0x2C, C3DVector by hidden pointer; world +0x34), and the next
// member @ 0x007A9B10 (manifest "CCreatureAction_ReleasePhysicalShieldSpell::FinishAction" is propagated; it is called on
// `this`, name unproven).
// Layout: Ego_r.pdb CTCAssassinRush LocalRelativeVectorForCamera +0x78 / RelativeVectorForCamera +0x84 sit at retail +0x74 /
// +0x80 (the ItemAlphaList vector is 12 bytes in retail, 16 in the PDB).
// Shape: C3DVector has a user copy constructor and no operator=, so each difference built by `return C3DVector(...)` lands in a
// frame temporary and is copied through a `lea` pointer register; the hero position is copied by value (x, y on the x87
// stack, z through the frame); the camera manager comes through an inline accessor so it loads straight into ecx.
struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
    C3DVector operator-(const C3DVector& b) const
    {
        return C3DVector(X - b.X, Y - b.Y, Z - b.Z);
    }
};
struct CCharString {
    void* PStringData;
};
extern CCharString g_CameraStickHelperName;     // 0x013BAC4C
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
};
class CTCAssassinRush {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x74 - 0x08];
    C3DVector LocalRelativeVectorForCamera;     // +0x74
    C3DVector RelativeVectorForCamera;          // +0x80
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void SetupCameraFromStick_007a9b10();                // 0x007A9B10 (descriptive)
    void SetupCameraOnStick();
};

void CTCAssassinRush::SetupCameraOnStick()
{
    CTCGraphicAppearance* appearance = ParentThing->PGraphicAppearance;
    if (appearance == 0)
        return;
    C3DVector stick;
    if (!appearance->GetHelperPoint(g_CameraStickHelperName, 0, 0, &stick, 1.0f))
        return;
    C3DVector heroPosition = ParentThing->GetPosition();
    LocalRelativeVectorForCamera = stick - heroPosition;
    RelativeVectorForCamera = GetWorld()->GetCameraManager()->PeekCurrentPos() - stick;
    SetupCameraFromStick_007a9b10();
}
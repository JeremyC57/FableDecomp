// NCameraModes::CSimplePointBase::OnDie() -- put the camera back where it was when the mode started, as a single cut from the current camera
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873570, 53 bytes, plain `ret`.
// Leaf: ?OnDie@CSimplePointBase
// Name: vtable 0x01277644 slot 22 (slot 21 OnInit 0x008734F0 saves StartPos); FableWin.pdb ?OnDie@CSimplePointBase@NCameraModes@@UAEXXZ.
// Layout: retail CSimplePointBase (landed ctor 0x00873A10): StartPos +0xa4, Position +0xb0, LookAtPos +0xbc; the root CBase has
//         LookAt +0x10, the camera manager +0x40 and GoTo (CPhysicsState, Position first) +0x54.
//         The camera manager: Camera +0x04, OldCamera +0x3c, the cut flag +0xb0 (see OnInit 0x008734F0).
// Shape: SetPosition(StartPos); then an inline manager helper (`this` read once into ecx, so no reload after the byte store): SingleCut = true; OldCamera = Camera (a 14-dword `rep movsd`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CCamera {                                     // 0x38 bytes (Ego_r.pdb CGameCameraManager::Camera/OldCamera)
    unsigned long _words[0x38 / 4];
};
struct CGameCameraManager {                          // retail offsets (engine/CGameCameraManager.h is the PDB layout:
    void*         _vftable;                          // its STL members are wider, so +0xb0 there is AvailableCamPoints)
    CCamera       Camera;                            // +0x04
    CCamera       OldCamera;                         // +0x3c
    unsigned char _pad74[0xb0 - 0x74];
    bool          SingleCut;                         // +0xb0 (PDB +0xbc; the retail STLport list/vector members are narrower)
    void SetSingleCut() { SingleCut = true; OldCamera = Camera; }   // descriptive inline helper
};
namespace NCameraModes {
struct CPhysicsState {
    C3DVector Position;                              // +0x00
    C3DVector Velocity;                              // +0x0c
};
struct CBase {
    virtual ~CBase();                                // slot 0
    virtual void Update();                           // slot 1
    unsigned char       _pad04[0x10 - 0x04];
    C3DVector           LookAt;                      // +0x10
    unsigned char       _pad1c[0x40 - 0x1c];
    CGameCameraManager* CameraManager;               // +0x40
    unsigned char       _pad44[0x54 - 0x44];
    CPhysicsState       GoTo;                        // +0x54
    unsigned char       _pad6c[0xa4 - 0x6c];
    const C3DVector& GetPosition() const { return GoTo.Position; }
    void SetPosition(const C3DVector& position) { GoTo.Position = position; }
    void SetObservedPos(const C3DVector& position) { LookAt = position; }
    CGameCameraManager& GetCameraManager() { return *CameraManager; }
};
struct CSimplePointBase : CBase {                    // vtable 0x01277644
    C3DVector StartPos;                              // +0xa4
    C3DVector Position;                              // +0xb0
    C3DVector LookAtPos;                             // +0xbc
    bool      AllowLookAround;                       // +0xc8
    virtual void OnDie();
};
void CSimplePointBase::OnDie()
{
    SetPosition(StartPos);
    GetCameraManager().SetSingleCut();
}
}
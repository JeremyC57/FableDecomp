// CWorld::GetCurrentSoundCamera(const CInterpolationInfo&) const -- the camera the sound listener follows: the render camera without a sound system or in the editor, the free camera while the player uses it, otherwise the current game camera, moved onto the hero's interpolated position unless a scripted camera mode is running
// Leaf: GetCurrentSoundCamera
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049DAA0, 291 bytes (304 with padding), `ret 8` (hidden CCamera result + info).
//   PDB ?GetCurrentSoundCamera@CWorld@@QBE?AVCCamera@@ABVCInterpolationInfo@@@Z.
// Callees: CWorld virtual slots 5 GetCurrentRenderCamera(const CInterpolationInfo&) const (0x0049DA00) and 6
//   GetCurrentRenderCamera() const (0x0049DA60) (rtti-vtables 0x012390F0; overloads in reverse declaration order),
//   g_pMainGameComponent 0x013B86A0 slot 9 IsEditorActive (0x004197B0), CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetFreeCamera(float) const 0x00487180, CPlayer::GetHeroThing 0x00487DC0 (x2), the current game camera
//   (+0x34) slots 10 (+0x28, its camera manager by out reference) and 1 (+0x04, the camera at a fraction),
//   CGameCameraManager::IsUsingScriptedCameraMode 0x00697AE0, CTCPhysicsBase::GetInterpolatedPosition(float) 0x0049D150
//   (landed), CCamera::CCamera(const CCamera&) 0x0049D380 (landed).
// Data: 0x013B86E0, a global pointer tested for null (named g_pSoundManager descriptively).
// Layout: CPlayer UsingFreeCamera +0x220 (chunk 178), CThing physics component +0x60, CInterpolationInfo fraction +4;
//   CWorld PlayerManager +0x0C, PCurrentGameCamera +0x34 (engine/CWorld.h).
// Shape: the fraction is read first into the dead `info` slot; the physics pointer is a named local so GetHeroThing and
//   the +0x60 load come before the argument pushes; the camera returned with its position replaced is a named local
//   copied out through the copy ctor (no NRVO).
struct C3DVector {
    float X, Y, Z;
};
struct CRightHandedSet { C3DVector Up, Forward; };
class CCamera {
public:
    C3DVector       WorldPosition;            // +0x00
    CRightHandedSet RHSet;                    // +0x0c
    bool            HeightLocked;             // +0x24
    bool            ViewVecZLocked;           // +0x25
    unsigned long   FOVFlags;                 // +0x28
    float           HorizontalFOV;            // +0x2c
    float           VerticalFOV;              // +0x30
    float           Zoom;                     // +0x34
    CCamera(const CCamera& o);                // 0x0049D380
};
struct CInterpolationInfo {
    long  Frame;                              // +0x00
    float Fraction;                           // +0x04
};
struct CTCPhysicsBase {
    C3DVector GetInterpolatedPosition(float t) const;   // 0x0049D150
};
struct CThing {
    unsigned char   _pad[0x60];
    CTCPhysicsBase* PPhysics;                 // +0x60
};
struct CPlayer {
    unsigned char _pad[0x220];
    bool          UsingFreeCamera;            // +0x220
    CCamera GetFreeCamera(float t) const;     // 0x00487180
    CThing* GetHeroThing();                   // 0x00487DC0
};
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CGameCameraManager { bool IsUsingScriptedCameraMode() const; };   // 0x00697AE0
struct CAIGameCameraBase {
    virtual void v00();
    virtual CCamera GetCamera(float t) const;          // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool GetCameraManager(CGameCameraManager*& manager);   // slot 10 (+0x28)
};
struct CMainGameComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsEditorActive();                     // slot 9 (+0x24)
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
extern void* g_pSoundManager;                          // 0x013B86E0
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CCamera GetCurrentRenderCamera() const;    // slot 6 (+0x18) (overloads sit in reverse order)
    virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo& info) const;   // slot 5 (+0x14)
    unsigned char      _pad04[0x8];
    CPlayerManager*    PlayerManager;                  // +0x0C
    unsigned char      _pad10[0x24];
    CAIGameCameraBase* PCurrentGameCamera;             // +0x34
    CCamera GetCurrentSoundCamera(const CInterpolationInfo& info) const;
};
CCamera CWorld::GetCurrentSoundCamera(const CInterpolationInfo& info) const
{
    float t = info.Fraction;
    if (!g_pSoundManager)
        return GetCurrentRenderCamera(info);
    if (g_pMainGameComponent->IsEditorActive())
        return GetCurrentRenderCamera();
    CPlayer* player = PlayerManager->GetMainPlayer();
    if (player->UsingFreeCamera)
        return player->GetFreeCamera(t);
    CGameCameraManager* manager;
    if (PCurrentGameCamera->GetCameraManager(manager) && !manager->IsUsingScriptedCameraMode() && player->GetHeroThing()) {
        CCamera camera = PCurrentGameCamera->GetCamera(t);
        const CTCPhysicsBase* physics = player->GetHeroThing()->PPhysics;
        camera.WorldPosition = physics->GetInterpolatedPosition(t);
        return camera;
    }
    return PCurrentGameCamera->GetCamera(t);
}

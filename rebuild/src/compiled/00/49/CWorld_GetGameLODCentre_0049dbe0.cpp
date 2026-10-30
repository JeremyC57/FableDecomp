// CWorld::GetGameLODCentre() const -- returns the level-of-detail centre, recomputed at most once per frame: the main player's free-camera position while the free camera is in use, else the current game camera's render position
// Leaf: ?GetGameLODCentre@CWorld@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049DBE0, 163 bytes, `ret 4` (hidden C3DVector result).
//   PDB name ?GetGameLODCentre@CWorld@@UBE?AVC3DVector@@XZ.
// Callees: CMainGameComponent frame getter 0x00416225 (Component +8, twice), CPlayerManager::GetMainPlayer 0x00449970
//   (PlayerManager +0xc, twice), CAIGameCameraBase slot 1 GetRenderCamera(float) (+4, CCamera by value; PCurrentGameCamera
//   +0x34).
// Layout: engine/CWorld.h carries Ego_r.pdb offsets, retail is 0xc lower past the three 16->12 byte containers at +0xac:
//   FrameCachedLODCentre +0xe8, CachedLODCentre +0xec (local model, mutable cache); CPlayer UsingFreeCamera +0x220,
//   CurrentFreeCamera +0x40 (CCamera, WorldPosition first).
// Shape: the recompute guard is `frame == -1 || frame != GetFrame()`; both arms assign
//   through C3DVector's implicit operator= over a user copy ctor (the `lea ecx,[esi+0ECh]` pointer copy, tails merged);
//   the result is copied out of the member.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CRightHandedSet { public: C3DVector Right, Up; };
class CCamera { public:                                   // 0x38 bytes (Ego_r.pdb)
    C3DVector       WorldPosition;                 // +0x00
    CRightHandedSet RHSet;                         // +0x0c
    bool            HeightLocked;                  // +0x24
    bool            ViewVecZLocked;                // +0x25
    unsigned long   FOVFlags;                      // +0x28
    float           HorizontalFOV;                 // +0x2c
    float           VerticalFOV;                   // +0x30
    float           Zoom;                          // +0x34
};
class CAIGameCameraBase { public:
    virtual ~CAIGameCameraBase();
    virtual CCamera GetRenderCamera(float interpolation) const;   // slot 1 (+4)
};
class CPlayer { public:
    unsigned char _pad00[0x40];
    CCamera       CurrentFreeCamera;               // +0x40
    unsigned char _pad78[0x220 - 0x78];
    bool          UsingFreeCamera;                 // +0x220
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent { public:
    long GetFrame();                               // 0x00416225
};
class CWorld { public:
    virtual void v00();
    unsigned char       _base04[4];                // +0x04
    CMainGameComponent* Component;                 // +0x08
    CPlayerManager*     PlayerManager;             // +0x0c
    unsigned char       _pad10[0x34 - 0x10];
    CAIGameCameraBase*  PCurrentGameCamera;        // +0x34
    unsigned char       _pad38[0xe8 - 0x38];
    mutable long        FrameCachedLODCentre;      // +0xe8
    mutable C3DVector   CachedLODCentre;           // +0xec
    virtual C3DVector GetGameLODCentre() const;
};
C3DVector CWorld::GetGameLODCentre() const
{
    if (FrameCachedLODCentre == -1 || FrameCachedLODCentre != Component->GetFrame()) {
        if (PlayerManager->GetMainPlayer().UsingFreeCamera)
            CachedLODCentre = PlayerManager->GetMainPlayer().CurrentFreeCamera.WorldPosition;
        else
            CachedLODCentre = PCurrentGameCamera->GetRenderCamera(0.0f).WorldPosition;
        FrameCachedLODCentre = Component->GetFrame();
    }
    return CachedLODCentre;
}
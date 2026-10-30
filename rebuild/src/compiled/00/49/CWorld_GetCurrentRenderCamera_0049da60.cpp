// CWorld::GetCurrentRenderCamera() const -- the render camera with no interpolation (fraction 1.0): the main player's free camera while it is in use, otherwise the current game camera
// Leaf: ?GetCurrentRenderCamera@CWorld@@UBE?AVCCamera@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049DA60, 63 bytes (+1 int3 padding), thiscall, `ret 4` (hidden CCamera result).
//   CWorld vtable 0x012390F0 slot 6 (the landed GetCurrentSoundCamera 0x0049DAA0 calls it in the editor).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetFreeCamera(float) const 0x00487180, the current game
//   camera's (+0x34) virtual slot 1 (+0x04).
// Layout: local model (engine/CWorld.h models the vptr as a data member, so CWorld's virtuals cannot be declared on it):
//   PlayerManager +0x0C, PCurrentGameCamera +0x34; CPlayer UsingFreeCamera +0x220.
// Shape: the shared 1.0f argument is pushed once ahead of the branch (`push 3F800000h` before `je`).
// Layout (supersedes any local-model note above): engine/CPlayer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CPlayer CPlayerDonor
#include "engine/CPlayer.h"
#undef CPlayer
class C3DVector { public:
    float X, Y, Z;
};
class CRightHandedSet { public: C3DVector Up, Forward; };
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
struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame; // +0x00
    float WFInterpolate;                       // +0x04
};
class CInterpolationInfo {
public:
    CInterpolationInfoSet Current;            // +0x00
    CInterpolationInfoSet Last;               // +0x08
    CInterpolationInfoSet PausedCurrent;      // +0x10
    CInterpolationInfoSet PausedLast;         // +0x18
    CInterpolationInfoSet BulletTimeCurrent;  // +0x20
    CInterpolationInfoSet BulletTimeLast;     // +0x28
    long                  WFServerCurrentTime; // +0x30
};
class CPlayer : public CPlayerDonor {
 public:
    CCamera GetFreeCamera(float t) const;     // 0x00487180
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
   // 0x00449970
class CAIGameCameraBase { public:
    virtual void v00();
    virtual CCamera GetCamera(float t) const;          // slot 1 (+0x04)
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CCamera GetCurrentRenderCamera() const;    // slot 6 (+0x18) (overloads sit in reverse order)
    virtual CCamera GetCurrentRenderCamera(const CInterpolationInfo& info) const;   // slot 5 (+0x14)
    unsigned char      _pad04[0x8];
    CPlayerManager*    PlayerManager;                  // +0x0c
    unsigned char      _pad10[0x24];
    CAIGameCameraBase* PCurrentGameCamera;             // +0x34
    unsigned char      _pad38[0x6c];
    bool               Paused;                         // +0xa4
};

CCamera CWorld::GetCurrentRenderCamera() const
{
    CPlayer* player = &PlayerManager->GetMainPlayer();
    if (player->UsingFreeCamera)
        return player->GetFreeCamera(1.0f);
    return PCurrentGameCamera->GetCamera(1.0f);
}
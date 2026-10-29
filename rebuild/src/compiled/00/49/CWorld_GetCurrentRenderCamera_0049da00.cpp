// CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const -- the camera the frame is rendered from: the main player's free camera while it is in use (at the paused interpolation fraction when the world is paused), otherwise the current game camera at the frame's fraction
// Leaf: ?GetCurrentRenderCamera@CWorld@@UBE?AVCCamera@@ABVCInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049DA00, 95 bytes (+1 int3 padding), thiscall, `ret 8` (hidden CCamera result + info).
//   CWorld vtable 0x012390F0 slot 5 (the landed GetCurrentSoundCamera 0x0049DAA0 calls it).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetFreeCamera(float) const 0x00487180, the current game
//   camera's (+0x34) virtual slot 1 (+0x04, the camera at a fraction).
// Layout: local model (engine/CWorld.h models the vptr as a data member, so CWorld's virtuals cannot be declared on it):
//   PlayerManager +0x0C, PCurrentGameCamera +0x34, Paused +0xA4 (header offsets agree); CPlayer UsingFreeCamera +0x220;
//   Ego_r.pdb CInterpolationInfo (Current +0x00, PausedCurrent +0x10; CInterpolationInfoSet WFInterpolate +0x04).
// Shape: the selected fraction is homed in the dead `info` argument slot; both returns construct straight into the
//   hidden result (RVO through the callees' own hidden pointers).
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
class CPlayerManager { public: CPlayer& GetMainPlayer(); };   // 0x00449970
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

CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const
{
    CPlayer* player = &PlayerManager->GetMainPlayer();
    if (player->UsingFreeCamera) {
        float t = Paused ? info.PausedCurrent.WFInterpolate : info.Current.WFInterpolate;
        return player->GetFreeCamera(t);
    }
    return PCurrentGameCamera->GetCamera(info.Current.WFInterpolate);
}
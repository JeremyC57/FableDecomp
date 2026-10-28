// NCameraModes::CBase::Die() -- camera-mode shutdown: runs the mode's own pure-virtual hook (slot 22), then on a single cut restores the FOV, roll angle and observed-position Z offset to their defaults
// Leaf: ?Die@CBase@NCameraModes@@QAEXXZ
// Retail: Fable.exe @ 0x0084D0A0, 50 bytes, thiscall `ret`.
// Identity (proven): the only caller is CGameCameraManager::SetCameraMode 0x00697E20 (landed), whose relocation audit and
//   source call `CurrentMode.Data->Die()` here; ?Die@CBase@NCameraModes@@QAEXXZ is in FableWin.pdb. The catalog label
//   "CBase::Die" agrees (module only).
// Body: slot 22 (+0x58) is pure virtual in NCameraModes::CBase's own vtable 0x0124DD74 (`_purecall`), so its name is not in
//   the PDB sets (descriptive `OnDie`). Then if CameraManager (+0x40)->SingleCut (+0xb0; Ego_r.pdb CGameCameraManager
//   SingleCut +0xbc, as the landed SetCameraMode model pins): CameraFOV = DefaultFOV, RollAngle = DefaultRollAngle,
//   ObservedPositionZOffset = DefaultObservedPositionZOffset.
// Layout (Ego_r.pdb CBase, retail): DefaultFOV +0x04, DefaultRollAngle +0x08, DefaultObservedPositionZOffset +0x0c, LookAt
//   (CPhysicsState) +0x10, CameraManager +0x40 (Ego_r +0x50), CameraFOV +0x84 / RollAngle +0x88 / ObservedPositionZOffset
//   +0x8c (Ego_r +0xa8..: each CPhysicsState is 0x10 smaller in retail and the SubComponents vector 4).
class CGameCameraManager {
public:
    unsigned char _pad00[0xb0];
    bool SingleCut;                                      // +0xb0
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void OnDie() = 0;                            // slot 22 (+0x58), pure in CBase (descriptive name)
    float DefaultFOV;                                    // +0x04
    float DefaultRollAngle;                              // +0x08
    float DefaultObservedPositionZOffset;                // +0x0c
    unsigned char LookAt[0x30];                          // +0x10 CPhysicsState (retail 0x30)
    CGameCameraManager* CameraManager;                   // +0x40
    unsigned char _pad44[0x84 - 0x44];
    float CameraFOV;                                     // +0x84
    float RollAngle;                                     // +0x88
    float ObservedPositionZOffset;                       // +0x8c
    void Die();
};
}
void NCameraModes::CBase::Die()
{
    OnDie();
    if (CameraManager->SingleCut)
    {
        CameraFOV = DefaultFOV;
        RollAngle = DefaultRollAngle;
        ObservedPositionZOffset = DefaultObservedPositionZOffset;
    }
}
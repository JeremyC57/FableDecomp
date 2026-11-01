// NCameraModes::CCameraPointModeBase::OnDie() -- point-camera shutdown hook: resets the working FOV to 0.2 and runs the leaf hook
// Leaf: ?OnDie@CCameraPointModeBase@NCameraModes@@UAEXXZ
// Retail: Fable.exe @ 0x0084FDB0, 15 bytes, thiscall, ends in a tail `jmp [eax+0x64]` (no ret of its own).
// Identity (proven, two sources): (1) vtable 0x0124DDDC (NCameraModes::CCameraPointModeBase RTTI) slot 22 (+0x58); slot 22
//   is the pure `OnDie` hook of NCameraModes::CBase (landed CBase::Die 0x0084D0A0 calls it) and is OnDie in the sibling
//   vtables CFirstPerson 0x012776AC (0x008735B0), CMoveOnCylinder 0x0125A064 (0x0085C450), CSimplePointBase 0x01277644
//   (0x00873570). (2) Ego_r.pdb name set: ?OnDie@CCameraPointModeBase@NCameraModes@@UAEXXZ (donor 0x008502B0, 0x20 bytes,
//   followed by ?Update@ / ?OnInit@ / ?GetPhysicsMode@ in the same order as retail slots 1 / 21 / 26 at 0x0084FDC0 /
//   0x0084FEA0 / 0x0084FF90).
// Body: CameraFOV (+0x84, CBase; see landed CBase::Die / CBase::Reset) = 0.2f, then slot 25 (+0x64). Slot 25 is the empty
//   `ret` 0x0056E290; FableWin.pdb lists ?OnInitLeaf@ / ?OnDieLeaf@CCameraPointModeBase in that order (021FED80 / 021FED90),
//   matching slots 24 (0x0056E280) / 25 (0x0056E290).
namespace NCameraModes {
class CBase {
public:
    virtual ~CBase();                                    // slot 0
    virtual void Update() = 0;                           // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20();
    virtual void OnInit() = 0;                           // slot 21
    virtual void OnDie() = 0;                            // slot 22
    virtual void OnReset();                              // slot 23
    float DefaultFOV;                                    // +0x04
    float DefaultRollAngle;                              // +0x08
    float DefaultObservedPositionZOffset;                // +0x0c
    unsigned char LookAt[0x30];                          // +0x10 CPhysicsState (retail 0x30)
    void* CameraManager;                                 // +0x40
    unsigned char _pad44[0x84 - 0x44];
    float CameraFOV;                                     // +0x84
    float RollAngle;                                     // +0x88
    float ObservedPositionZOffset;                       // +0x8c
};
class CCameraPointModeBase : public CBase {
public:
    virtual void OnDie();                                // slot 22
    virtual void OnInitLeaf();                           // slot 24
    virtual void OnDieLeaf();                            // slot 25
};
void CCameraPointModeBase::OnDie()
{
    CameraFOV = 0.2f;
    OnDieLeaf();
}
}
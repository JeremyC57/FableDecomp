// NCameraModes::CProjectileWeapon::OnInit() -- projectile-weapon camera start: make the switch a single cut; if the camera already in use is a projectile-weapon camera, carry its orbit (phi, height, deltas, zoom FOV, axes) over, else set the params up from the def with the zoom FOV at the camera FOV; then run one Update
// Leaf: ?OnInit@CProjectileWeapon@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00878F50, 276 bytes, tail `jmp [vtbl+4]` (Update).
// Identity: vtable 0x01277804 (RTTI `.?AVCProjectileWeapon@NCameraModes@@`) slot 21; slot 22 is the landed OnDie 0x00878B40
//   (as CSimplePointBase 0x008734F0 / 0x00873570 have them). Ego_r.pdb `?OnInit@CProjectileWeapon@NCameraModes@@UAEXXZ`.
// Callees: CGameCameraManager::HasCameraMode(NCameraModes::EMode) const 0x00697AC0 (body: CurrentMode->GetModeType() == mode;
//   the only QBE _N(EMode) member in both PDBs), CProjectileWeapon::SetUpParams 0x00878CE0 (landed, private), Update (slot 1).
// Inlines: CGameCameraManager::SetSingleCut() (PDB QAEXXZ: OldCamera = Camera, the 14-dword `rep movsd`, then SingleCut);
//   CGameCameraManager::GetCameraMode<CProjectileWeapon>(CProjectileWeapon**) (FableWin.pdb template: HasCameraMode of
//   T::StatGetModeType(), the folded `push 0Fh`, then the current mode cast); CProjectileWeapon::StatGetModeType() (SI, 0x0F:
//   vtable slot 2 0x0087B5F0 is `mov eax,0Fh ; ret`).
// Layout (retail = Ego_r.pdb - 0x24 for the mode, as the landed SetUpParams 0x00878CE0): CBase CameraManager +0x40,
//   CameraFOV +0x84; CProjectileWeapon XAxis +0xa9, YAxis +0xb5, ZAxis +0xc1, Phi +0xd0 .. DeltaLockDeltaHeightParam +0xec,
//   ZoomInFOV +0x104 (Attractors is a 12-byte vector). CGameCameraManager Camera +0x04, OldCamera +0x3c, CurrentMode +0x98,
//   SingleCut +0xb0 (Ego_r +0x9c / +0xbc: narrower STLport members). Header debt: engine/CGameCameraManager.h is the PDB layout.
#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

class CRightHandedSet { public:
    C3DVector Up, Forward;
};
class CCamera { public:                                     // 0x38 bytes
    C3DVector       WorldPosition;
    CRightHandedSet RHSet;
    bool            HeightLocked, ViewVecZLocked;
    unsigned long   FOVFlags;
    float           HorizontalFOV, VerticalFOV, Zoom;
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
};

namespace NCameraModes {
enum EMode { EMode_ProjectileWeapon = 15 };                 // descriptive enumerator (value from slot 2)
class CBase;
}

class CGameCameraManager { public:
    unsigned char                           _pad00[0x04];
    CCamera                                 Camera;         // +0x04
    CCamera                                 OldCamera;      // +0x3c
    unsigned char                           _pad74[0x98 - 0x74];
    CCountedPointer<NCameraModes::CBase>    CurrentMode;    // +0x98
    unsigned char                           _padA0[0xb0 - 0xa0];
    bool                                    SingleCut;      // +0xb0
    bool HasCameraMode(NCameraModes::EMode mode) const;     // 0x00697AC0
    NCameraModes::CBase* GetCameraMode() { return CurrentMode.Data; }
    template <class T> bool GetCameraMode(T** mode)
    {
        if (HasCameraMode(T::StatGetModeType())) {
            *mode = (T*)GetCameraMode();
            return true;
        }
        return false;
    }
    void SetSingleCut() { OldCamera = Camera; SingleCut = true; }
};

namespace NCameraModes {
#pragma pack(push, 1)
class CBase { public:
    virtual ~CBase();                                       // slot 0
    virtual void Update();                                  // slot 1
    unsigned char       _pad04[0x40 - 0x04];
    CGameCameraManager* CameraManager;                      // +0x40
    unsigned char       _pad44[0x84 - 0x44];
    float               CameraFOV;                          // +0x84
    unsigned char       _pad88[0xa4 - 0x88];
};
class CProjectileWeapon : public CBase { public:            // vtable 0x01277804
    long          ShotFiredTimer;                           // +0xa4
    bool          Finished;                                 // +0xa8
    C3DVector     XAxis;                                    // +0xa9
    C3DVector     YAxis;                                    // +0xb5
    C3DVector     ZAxis;                                    // +0xc1
    unsigned char _padcd[0xd0 - 0xcd];
    float         Phi;                                      // +0xd0
    float         HeightParam;                              // +0xd4
    float         DeltaPhi;                                 // +0xd8
    float         DeltaHeightParam;                         // +0xdc
    float         LockDeltaPhi;                             // +0xe0
    float         LockDeltaHeightParam;                     // +0xe4
    float         DeltaLockDeltaPhi;                        // +0xe8
    float         DeltaLockDeltaHeightParam;                // +0xec
    unsigned char _padf0[0x104 - 0xf0];
    float         ZoomInFOV;                                // +0x104
    static EMode StatGetModeType() { return EMode_ProjectileWeapon; }
    virtual void Update();
    virtual void OnInit();
private:
    void SetUpParams();                                     // 0x00878CE0
};
#pragma pack(pop)

void CProjectileWeapon::OnInit()
{
    CameraManager->SetSingleCut();
    CProjectileWeapon* previous;
    if (CameraManager->GetCameraMode(&previous)) {
        LockDeltaPhi = previous->LockDeltaPhi;
        LockDeltaHeightParam = previous->LockDeltaHeightParam;
        DeltaLockDeltaPhi = previous->DeltaLockDeltaPhi;
        DeltaLockDeltaHeightParam = previous->DeltaLockDeltaHeightParam;
        Phi = previous->Phi;
        HeightParam = previous->HeightParam;
        DeltaPhi = previous->DeltaPhi;
        DeltaHeightParam = previous->DeltaHeightParam;
        ZoomInFOV = previous->ZoomInFOV;
        XAxis = previous->XAxis;
        YAxis = previous->YAxis;
        ZAxis = previous->ZAxis;
    } else {
        SetUpParams();
        ZoomInFOV = CameraFOV;
    }
    Update();
}
} // namespace NCameraModes
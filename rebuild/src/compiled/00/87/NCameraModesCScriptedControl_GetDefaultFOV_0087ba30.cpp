// NCameraModes::CScriptedControl::GetDefaultFOV() const -- the script module's FOV when it supplies one, else the FOV of the camera captured when the mode started
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087BA30, 45 bytes, plain `ret` (float in st0).
// Leaf: ?GetDefaultFOV@CScriptedControl
// Name: vtable 0x0125C58C slot 18; FableWin.pdb ?GetDefaultFOV@CScriptedControl@NCameraModes@@UBEMXZ (its float neighbour slot 19
//       0x0087BA60 is GetDefaultRollAngle, the pair GetModuleFOV/GetModuleRollAngle of the module, slots 5/6).
// Callees: the module's virtual GetModuleFOV(float&) (slot 5, +0x14), CCamera::GetHorizontalFOV() const @ 0x00A0BEB0.
// Layout: retail CScriptedControl as the landed ctor 0x0069DFE0: PScriptModule (CCountedPointer, Data first) +0xa4,
//         InitCamera (CCamera) +0xb0.
// Shape: the out-float is a frame local (`push ecx` slot), returned with `fld` on success; the fallback is a member call
//        whose st0 result is returned as is.
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward, Up; };
struct CCamera {                                             // 0x38 bytes
    C3DVector       WorldPosition;                           // +0x00
    CRightHandedSet RHSet;                                   // +0x0c
    bool            HeightLocked;                            // +0x24
    bool            ViewVecZLocked;                          // +0x25
    unsigned long   FOVFlags;                                // +0x28
    float           HorizontalFOV;                           // +0x2c
    float           VerticalFOV;                             // +0x30
    float           Zoom;                                    // +0x34
    float GetHorizontalFOV() const;                          // 0x00A0BEB0
};
namespace NCameraModes {
struct CScriptModuleBase {                                   // vtable 0x01255F74
    virtual ~CScriptModuleBase();                            // slot 0
    virtual void Update() = 0;                               // slot 1
    virtual bool IsFinished() const;                         // slot 2
    virtual bool IsShortenOnOcclusion() const;               // slot 3
    virtual bool IsStretchAroundHero() const;                // slot 4
    virtual bool GetModuleFOV(float& fov) const;             // slot 5 (+0x14)
    virtual bool GetModuleRollAngle(float& angle) const;     // slot 6 (+0x18)
};
struct CBase {                                               // retail sizeof 0xa4
    virtual void v00();
    unsigned char _body[0xa4 - 4];
};
struct CScriptedControl : CBase {                            // vtable 0x0125C58C
    CCountedPointer<CScriptModuleBase> PScriptModule;        // +0xa4 (PDB +0xc8)
    float                              ObservedThingMeshHeightOnInit;   // +0xac (PDB +0xd0)
    CCamera                            InitCamera;           // +0xb0 (PDB +0xd4)
    virtual float GetDefaultFOV() const;
};
float CScriptedControl::GetDefaultFOV() const
{
    float fov;
    if (PScriptModule->GetModuleFOV(fov))
        return fov;
    return InitCamera.GetHorizontalFOV();
}
}
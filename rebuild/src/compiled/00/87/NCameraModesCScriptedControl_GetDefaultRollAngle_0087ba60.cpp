// NCameraModes::CScriptedControl::GetDefaultRollAngle() const -- the script module's roll angle when it supplies one, else no roll
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087BA60, 33 bytes, plain `ret` (float in st0).
// Leaf: ?GetDefaultRollAngle@CScriptedControl
// Name: vtable 0x0125C58C slot 19; FableWin.pdb ?GetDefaultRollAngle@CScriptedControl@NCameraModes@@UBEMXZ.
// Callees: the module's virtual GetModuleRollAngle(float&) (slot 6, +0x18).
// Layout: retail CScriptedControl as the landed ctor 0x0069DFE0: PScriptModule (CCountedPointer, Data first) +0xa4,
//         InitCamera (CCamera) +0xb0.
// Shape: the out-float is the `push ecx` frame slot; the fallback 0.0f is the literal-pool zero (0x0122DEDC).
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
    virtual float GetDefaultRollAngle() const;
};
float CScriptedControl::GetDefaultRollAngle() const
{
    float angle;
    if (PScriptModule->GetModuleRollAngle(angle))
        return angle;
    return 0.0f;
}
}
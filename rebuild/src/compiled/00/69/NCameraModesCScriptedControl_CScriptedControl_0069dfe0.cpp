// NCameraModes::CScriptedControl::CScriptedControl(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def, CCountedPointer<CScriptModuleBase>& module) -- scripted camera mode: base camera mode, take a reference on the script module, mesh-height-on-init 1.0, an unlocked initial camera
// Leaf: ??0CScriptedControl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069DFE0, length 83 bytes, `ret 0Ch`. FableWin.pdb
// ??0CScriptedControl@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@AAV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@@Z.
// Vtable 0x0125C58C = NCameraModes::CScriptedControl (rtti-vtables.tsv); callers 0x00699B4E, 0x00699FA1, 0x0069A2DD, 0x0069E407.
// The catalog's CActionParamCCountedPointer<NUISystem::CComponent> label is propagated.
// Callees: NCameraModes::CBase::CBase(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) @ 0x0084E420.
// Layout: Ego_r.pdb CScriptedControl PScriptModule +0xc8 / ObservedThingMeshHeightOnInit +0xd0 / InitCamera +0xd4 sit 0x24 lower in
// retail (+0xa4 / +0xac / +0xb0; the retail CBase is 0xa4 bytes); CCamera HeightLocked / ViewVecZLocked +0x24 / +0x25.
// Shape: the counted pointer's inline copy constructor (Data, Info, then `if (Info) ++Info->RefCount` against the shared
// zero register), the float initialiser, then the camera member's inline constructor clearing its two lock flags.
class CGameCameraManager;
class CCameraModeDef;
template<class T> class CDefPointer { public: T* Object; };

class CCPPointerInfo { public:
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04
    void*         Data;                                      // +0x08
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class C3DVector { public: float X, Y, Z; C3DVector() { } };
class CRightHandedSet { public: C3DVector Forward, Up; CRightHandedSet() { } };
class CCamera { public:                                             // 0x38 bytes
    C3DVector       WorldPosition;                           // +0x00
    CRightHandedSet RHSet;                                   // +0x0c
    bool            HeightLocked;                            // +0x24
    bool            ViewVecZLocked;                          // +0x25
    unsigned long   FOVFlags;                                // +0x28
    float           HorizontalFOV;                           // +0x2c
    float           VerticalFOV;                             // +0x30
    float           Zoom;                                    // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) { }
};

namespace NCameraModes {

class CScriptModuleBase;

class CBase { public:                                               // retail sizeof 0xa4
    virtual void v00();
    unsigned char _body[0xa4 - 4];
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
};

struct CScriptedControl : CBase {                            // vtable 0x0125C58C
    CCountedPointer<CScriptModuleBase> PScriptModule;        // +0xa4 (PDB +0xc8)
    float                              ObservedThingMeshHeightOnInit;   // +0xac (PDB +0xd0)
    CCamera                            InitCamera;           // +0xb0 (PDB +0xd4)
    virtual void v00();
    CScriptedControl(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                     CCountedPointer<CScriptModuleBase>& module);
};

CScriptedControl::CScriptedControl(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                                   CCountedPointer<CScriptModuleBase>& module)
    : CBase(manager, def)
    , PScriptModule(module)
    , ObservedThingMeshHeightOnInit(1.0f)
{
}

}
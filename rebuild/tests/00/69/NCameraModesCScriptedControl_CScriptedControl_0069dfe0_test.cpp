#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
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
    CCountedPointer() : Data(0), Info(0) { }   // test-only
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

static int g_baseCalls = 0;
static CGameCameraManager* g_manager = 0;
NCameraModes::CBase::CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) { ++g_baseCalls; g_manager = &manager; }
void NCameraModes::CBase::v00() { }
void NCameraModes::CScriptedControl::v00() { }
int main()
{
    CCPPointerInfo info = { 1, 0, 0 };
    CCountedPointer<NCameraModes::CScriptModuleBase> module;
    module.Data = (NCameraModes::CScriptModuleBase*)&info;
    module.Info = &info;
    CDefPointer<const CCameraModeDef> def = { 0 };
    CGameCameraManager* manager = (CGameCameraManager*)&info;
    NCameraModes::CScriptedControl* c = new NCameraModes::CScriptedControl(*manager, def, module);
    if (g_baseCalls != 1 || g_manager != manager || c->PScriptModule.Data != module.Data || c->PScriptModule.Info != &info ||
        info.RefCount != 2 || c->ObservedThingMeshHeightOnInit != 1.0f || c->InitCamera.HeightLocked || c->InitCamera.ViewVecZLocked) {
        std::printf("0069dfe0_TEST FAIL\n");
        return 1;
    }
    CCountedPointer<NCameraModes::CScriptModuleBase> empty;
    NCameraModes::CScriptedControl* d = new NCameraModes::CScriptedControl(*manager, def, empty);
    if (d->PScriptModule.Info != 0 || info.RefCount != 2) {
        std::printf("0069dfe0_TEST FAIL\n");
        return 1;
    }
    std::printf("0069dfe0_TEST PASS\n");
    return 0;
}
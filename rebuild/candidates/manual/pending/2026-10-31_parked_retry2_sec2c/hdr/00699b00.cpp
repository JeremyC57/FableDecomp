// CGameCameraManager::PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase> module) -- warm up a
// Grade: DIFFER(ours v retail) -- 243v238: only the by-value module parameter's release -- retail `mov esi,[esp+90h] ; test ; je ; dec [esi] ; jne`, ours keeps the block in eax and esi and re-reads the count (`dec [eax] ; cmp [esi],0`); member, local-copy, nested and Release()-member forms tried
// scripted camera: build a scripted-control mode for the module (inheriting the current mode's persistent data),
// initialise it, let the display engine preload for the resulting camera, then restore both cameras
// Leaf: PreloadScriptedCamera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00699B00, length 238 bytes (padded to 240), `ret 8`.
// Callees: CCamera::CCamera(const CCamera&) @ 0x0049D380 (x2), operator new @ 0x00BFEA1A,
// NCameraModes::CScriptedControl::CScriptedControl(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&,
// CCountedPointer<CScriptModuleBase>&) @ 0x0069DFE0 (0xE8 bytes; labelled CActionParam...), CCountedPointer::Reset
// @ 0x0069C2F0 (landed body above, auto_inline off), NCameraModes::CBase::TransferPersistantBaseData(CBase*)
// @ 0x0084CF80, NCameraModes::CBase::Init @ 0x0084E110, the engine display's renderer virtual slot 57 (+0xE4, takes
// the camera), operator delete @ 0x00BFE9BC. Globals: the engine 0x013B86A0 (+0x28 -> +0x2C), the default camera mode
// def 0x013BA8D8.
// Retail layout: CGameCameraManager Camera +0x04, OldCamera +0x3C (0x38-byte CCamera), CurrentMode +0x98
// (CCountedPointer<NCameraModes::CBase>; engine/CGameCameraManager.h has it at the PDB +0x9C). Graphics/camera layer.
// Shape: two saved cameras by copy ctor, the landed 0x0069E3D0 counted-pointer mode model, the restores through
// CCamera's implicit assignment (`rep movsd`), the by-value module destroyed at the end; the counted pointer's dtor
// works on a local copy of the block pointer (the module's address escapes to the mode ctor, yet retail keeps the
// block in esi).

#include "engine/CCountedPointer.h"

struct CCamera {
    unsigned char _body[0x38];
    CCamera(const CCamera& other);                  // 0x0049D380
};

struct CCameraModeDef;
template <class T> struct CDefPointer { T* Object; };
extern CDefPointer<const CCameraModeDef> g_CameraModeDef;   // 0x013BA8D8

class CGameCameraManager;
namespace NCameraModes {
struct CScriptModuleBase;
struct CBase {
    virtual ~CBase();
    void TransferPersistantBaseData(CBase* previous);   // 0x0084CF80
    void Init();                                        // 0x0084E110
};
struct CScriptedControl : CBase {
    CScriptedControl(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
                     CCountedPointer<CScriptModuleBase>& module);   // 0x0069DFE0
    unsigned char _body[0xe4];
};
}

struct CRenderer {
    virtual void r000();
    virtual void r001();
    virtual void r002();
    virtual void r003();
    virtual void r004();
    virtual void r005();
    virtual void r006();
    virtual void r007();
    virtual void r008();
    virtual void r009();
    virtual void r010();
    virtual void r011();
    virtual void r012();
    virtual void r013();
    virtual void r014();
    virtual void r015();
    virtual void r016();
    virtual void r017();
    virtual void r018();
    virtual void r019();
    virtual void r020();
    virtual void r021();
    virtual void r022();
    virtual void r023();
    virtual void r024();
    virtual void r025();
    virtual void r026();
    virtual void r027();
    virtual void r028();
    virtual void r029();
    virtual void r030();
    virtual void r031();
    virtual void r032();
    virtual void r033();
    virtual void r034();
    virtual void r035();
    virtual void r036();
    virtual void r037();
    virtual void r038();
    virtual void r039();
    virtual void r040();
    virtual void r041();
    virtual void r042();
    virtual void r043();
    virtual void r044();
    virtual void r045();
    virtual void r046();
    virtual void r047();
    virtual void r048();
    virtual void r049();
    virtual void r050();
    virtual void r051();
    virtual void r052();
    virtual void r053();
    virtual void r054();
    virtual void r055();
    virtual void r056();
    virtual void PreloadForCamera(const CCamera& camera);   // slot 57 (+0xE4)
};
struct CDisplay {
    unsigned char _pad[0x2c];
    CRenderer* Renderer;                            // +0x2C
};
struct CEngine {
    unsigned char _pad[0x28];
    CDisplay* Display;                              // +0x28
};
extern CEngine* g_pEngine;                          // 0x013B86A0

class CGameCameraManager {
public:
    void* __vftable;                                // +0x00
    CCamera Camera;                                 // +0x04
    CCamera OldCamera;                              // +0x3C
    unsigned char _pad74[0x24];
    CCountedPointer<NCameraModes::CBase> CurrentMode;   // +0x98
    void PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase> module);
};

void CGameCameraManager::PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase> module)
{
    CCamera savedCamera(Camera);
    CCamera savedOldCamera(OldCamera);
    CCountedPointer<NCameraModes::CBase> mode;
    mode.Reset(new NCameraModes::CScriptedControl(*this, g_CameraModeDef, module));
    if (CurrentMode.Data)
        mode.Data->TransferPersistantBaseData(CurrentMode.Data);
    mode.Data->Init();
    g_pEngine->Display->Renderer->PreloadForCamera(Camera);
    Camera = savedCamera;
    OldCamera = savedOldCamera;
}

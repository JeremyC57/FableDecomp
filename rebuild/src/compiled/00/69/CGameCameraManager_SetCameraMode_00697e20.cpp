// Native SetCameraMode: counted-pointer by-value ownership and pending reset.
// Actual donor manager/World headers retained under aliases: CurrentMode9C->98,
// PendingA4->A0, SingleCutBC->B0, CameraPreloadF4->E8; World script38->2C.
#define CGameCameraManager CGameCameraManagerDonor
#define CWorld CWorldDonor
#include "engine/CGameCameraManager.h"
#include "engine/CWorld.h"
#undef CWorld
#undef CGameCameraManager
#include "engine/CGameScriptInterface.h"
#include <string.h>
namespace NCameraModes {class CBase { public:void Die();void TransferPersistantBaseData(const CBase&);void Init();};}
struct CModeOwner {unsigned References;void(__fastcall* Destroy)(void*);void* Data;};
template<class T>struct CModePointer {
 T* Data;CModeOwner* Info;
 CModePointer():Data(0),Info(0){}
 CModePointer(const CModePointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->References;}
 void Release(){if(Info){--Info->References;if(!Info->References){Info->Destroy(Info->Data);delete Info;}}}
 ~CModePointer(){Release();}
 void Reset(){Release();Info=0;Data=0;}
 CModePointer& operator=(const CModePointer& p){if(Info!=p.Info){Release();Data=p.Data;Info=p.Info;if(Info)++Info->References;}return *this;}
};
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its display engine (PDisplayEngine +0x28, GetDisplayEngine()) and that engine's Engine3D +0x2C
//   (Get3DEngine()), whose slot 58 is CIEngine::ChangeCameraAngle() (engine/CIEngine.h); was read as a CGameScriptInterface* at +0x2C
//   with a member-function pointer copied out of its vtable; was the stand-in CModeGlobalRoot* gModeGlobalRoot with its CModeWorld stand-in.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
class CGameCameraManager { public:
 unsigned char Prefix[0x98];CModePointer<NCameraModes::CBase> CurrentMode,PendingMode;
 unsigned char BetweenModesAndCut[8];bool SingleCut;unsigned char BetweenCutAndPreload[0x37];bool CameraPreload,FastMode;
 void TellEngineThatCameraHasMoved(bool); //00697860
 void SetCameraMode(CModePointer<NCameraModes::CBase>);
};
#ifdef _MSC_VER
typedef char Mode98[offsetof(CGameCameraManager,CurrentMode)==0x98?1:-1];
typedef char CutB0[offsetof(CGameCameraManager,SingleCut)==0xb0?1:-1];
typedef char PreloadE8[offsetof(CGameCameraManager,CameraPreload)==0xe8?1:-1];
#endif
void CGameCameraManager::SetCameraMode(CModePointer<NCameraModes::CBase> mode){
 if(mode.Data){if(CurrentMode.Data){CurrentMode.Data->Die();mode.Data->TransferPersistantBaseData(*CurrentMode.Data);}mode.Data->Init();CurrentMode=mode;}
 PendingMode.Reset();
 if(SingleCut)CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().ChangeCameraAngle();
 if(CameraPreload)TellEngineThatCameraHasMoved(FastMode);
}
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
namespace NCameraModes {struct CBase {void Die();void TransferPersistantBaseData(const CBase&);void Init();};}
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
struct CModeWorld {unsigned char Prefix[0x2c];CGameScriptInterface* Script;};
struct CModeGlobalRoot {unsigned char Prefix[0x28];CModeWorld* World;};
extern CModeGlobalRoot* gModeGlobalRoot; //013b86a0, actual chain +28,+2C
struct CGameCameraManager {
 unsigned char Prefix[0x98];CModePointer<NCameraModes::CBase> CurrentMode,PendingMode;
 unsigned char BetweenModesAndCut[8];bool SingleCut;unsigned char BetweenCutAndPreload[0x37];bool CameraPreload,FastMode;
 void TellEngineThatCameraHasMoved(bool); //00697860
 void SetCameraMode(CModePointer<NCameraModes::CBase>);
};
typedef bool(CGameScriptInterface::*CModeCutCall)() const;
#ifdef _MSC_VER
typedef char PMF4[sizeof(CModeCutCall)==4?1:-1];
typedef char Mode98[offsetof(CGameCameraManager,CurrentMode)==0x98?1:-1];
typedef char CutB0[offsetof(CGameCameraManager,SingleCut)==0xb0?1:-1];
typedef char PreloadE8[offsetof(CGameCameraManager,CameraPreload)==0xe8?1:-1];
#endif
void CGameCameraManager::SetCameraMode(CModePointer<NCameraModes::CBase> mode){
 if(mode.Data){if(CurrentMode.Data){CurrentMode.Data->Die();mode.Data->TransferPersistantBaseData(*CurrentMode.Data);}mode.Data->Init();CurrentMode=mode;}
 PendingMode.Reset();
 if(SingleCut){CGameScriptInterface* script=gModeGlobalRoot->World->Script;CModeCutCall cut=0;memcpy(&cut,static_cast<const CModeCutCall*>(script->__vftable)+58,sizeof(cut));(script->*cut)();}
 if(CameraPreload)TellEngineThatCameraHasMoved(FastMode);
}
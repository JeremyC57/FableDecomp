// CInputProcessCreatureMovementWatchForControlAngleChange::ProcessInput -- normalize movement, signal camera changes, remember direction
// Retail00691C50 length206; GetControlMovementFromGameActionEvent00687D00.
// Actual donor headers retained; native camera manager prefix differs because
// CamTools vector16->12, AvailableCamPoints list12->4. ControlNoControlMovement+BD.
// Repair: genuine25slot camera interface, constManager slot9; nonconst slot10.
// Native slot9 returns const manager; const_cast models the observed write through
// real nonconst SetNoControlMovement. No fabricated mutable field is introduced.
// Constants0129BA3C=0.0001f,0122DED8=1.0f,0125C0B8=0.95f independently read from PE.
#include <math.h>
#include "rebuild_abi.h"
#define CAInputProcess DonorCAInputProcess
#include "engine/CAInputProcess.h"
#undef CAInputProcess
#define CInputProcessGameBase DonorCInputProcessGameBase
#include "engine/CInputProcessGameBase.h"
#undef CInputProcessGameBase
#define CAIGameCameraBase DonorCAIGameCameraBase
#include "engine/CAIGameCameraBase.h"
#undef CAIGameCameraBase
#define CGameCameraManager DonorCGameCameraManager
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CWorld DonorCWorld
#include "engine/CWorld.h"
#undef CWorld
class CWorldInit;
class CWorld:public DonorCWorld { public:CWorld(const CWorldInit&);};
#include "engine/CProcessedInput.h"
class CInputEvent;
class C2DVector { public:float X,Y;C2DVector(){} C2DVector(float x,float y):X(x),Y(y){} float GetLength()const{return (float)sqrt(X*X+Y*Y);} };
class C3DVector { public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CCamera;
class CEnginePrimitivePrepareInfo;
class CGameCameraManager;
class CThingGameObject;
class CAIGameCameraBase { public:
 CAIGameCameraBase(const CWorld&,const C3DVector&,const C3DVector&);
 virtual void Update()=0; //0
 virtual CCamera GetRenderCamera(float)const=0; //1
 virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&)const; //2
 virtual bool IsPosInViewingFrustrum(const C3DVector&)const; //3
 virtual bool Is2DAreaInViewingFrustrum(const C3DVector&,float)const; //4
 virtual bool IsVolumeInViewingFrustrum(const C3DVector&,float)const; //5
 virtual bool IsLineOfSightToPos(const C3DVector&)const; //6
 virtual float GetControlAngleXY()const; //7
 virtual void SetChangeInControlDirection(); //8
 // VC groups overloads in reverse declaration order: const9, nonconst10.
 virtual bool GetCameraManager(CGameCameraManager**); //10
 virtual bool GetCameraManager(const CGameCameraManager**)const; //9
 virtual const C3DVector PeekCurrentPos()const; //11
 virtual bool IsFreeLookMode()const; //12
 virtual bool IsOverridingMainAngleXY()const; //13
 virtual void ScriptEnd(); //14
 virtual void ScriptStart(); //15
 virtual void ScriptMoveToPosAndLookAtPos(const C3DVector&,const C3DVector&,float); //16
 virtual void ScriptMoveToPosAndLookAtThing(const CThingGameObject&,const C3DVector&,float); //17
 virtual void ScriptMoveToThingAndLookAtPos(const CThingGameObject&,const C3DVector&,const C3DVector&,float,bool); //18
 virtual void ScriptMoveToThingAndLookAtThing(const CThingGameObject&,const CThingGameObject&,const C3DVector&,float,bool); //19
 virtual void ScriptSetRotationSpeed(float); //20
 virtual void SetDebugCameraType(long); //21
 virtual void SetLookaroundVector(const C3DVector&); //22
 virtual void CyclePlayerViewType(); //23
 virtual ~CAIGameCameraBase(); //24 native scalar6971A0
};
// Prefix view uses genuine fields in their native representation, without invented padding.
class CCamera { public:CCamera();float WorldPosition[3];float Up[3];float Forward[3];bool HeightLocked,ViewVecZLocked;unsigned int FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;};
struct NativeCameraTools { CGameCameraManager* CameraManager;void* ScanThingsStart;void* ScanThingsFinish;void* ScanThingsEnd;float CameraMeshOffsetDistance,CameraCreatureOffsetDistance,CameraLandscapeOffsetDistance,CameraThemeOffsetDistance; };
class CGameCameraManager : public CAIGameCameraBase { public:
 CGameCameraManager(const CWorld&);
 void SetNoControlMovement(){ControlNoControlMovement=true;} //PDBfablewin21E4220
 CCamera Camera,OldCamera;
 CWorld* World;
 NativeCameraTools CamTools;
 void* CurrentMode_Data;void* CurrentMode_Info;void* PendingMode_Data;void* PendingMode_Info;
 bool NewAutoMode,ObservedThingInMelee,ObservedThingInBuilding;
 void* AvailableCamPointsHead;
 bool SingleCut,ChangeInControlDirection;
 int InsideHeroCount;
 float ControlAngleXY;
 bool AngleChange,ControlNoControlMovement;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CBaseClassNonCopyable:CBaseClass {protected:CBaseClassNonCopyable();public:virtual ~CBaseClassNonCopyable();};
class CLinkedListPosition { public:CBaseClass* Data;CLinkedListPosition* Next;CLinkedListPosition* Previous;};
class CInputProcessManager;
class CAInputProcess:public CBaseClassNonCopyable { public:
 CAInputProcess();
 CInputProcessManager* PPlayerInterface;
 CLinkedListPosition InputProcessListPos;
 virtual ~CAInputProcess();
 virtual bool AddToInterface(CInputProcessManager&);
 virtual void Init();
 virtual void Cleanup();
 virtual void ProcessInput(const CInputEvent&,CProcessedInput&)=0;
 virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&)const;
 virtual void Update(CProcessedInput&);
 virtual bool ProcessWhilePaused()const;
 virtual bool IsInputEventToBeSentToThisInputProcess(const CInputEvent&)const;
};
enum EGameAction { GAME_ACTION_MOVEMENT=60 };
class CInputProcessGameBaseInit { public:CWorld* World;CPlayer* MyPlayer;CGameDefinitionManager* DefinitionManager;CMainGameComponent* Component;CInputProcessGameBaseInit();};
struct CInputProcessGameBase:CAInputProcess {
 CInputProcessGameBase(const CInputProcessGameBaseInit&);
 CWorld* World;
 CPlayer* MyPlayer;
 CGameDefinitionManager* DefinitionManager;
 CMainGameComponent* Component;
 CGamePlayerInterface* PGamePlayerInterface;
 bool GetControlMovementFromGameActionEvent(EGameAction,const CInputEvent&,C2DVector&)const; //00687D00 enum EGameAction
};
class CInputProcessCreatureMovementWatchForControlAngleChange:public CInputProcessGameBase { public:
 CInputProcessCreatureMovementWatchForControlAngleChange(const CInputProcessGameBaseInit&);
 C2DVector LastJoyDirection;
 virtual void ProcessInput(const CInputEvent&,CProcessedInput&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CInputProcessCreatureMovementWatchForControlAngleChange)==0x30);
FABLE_STATIC_ASSERT(offsetof(CInputProcessCreatureMovementWatchForControlAngleChange,LastJoyDirection)==0x28);
FABLE_STATIC_ASSERT(offsetof(CGameCameraManager,ControlNoControlMovement)==0xbd);
FABLE_STATIC_ASSERT(sizeof(CInputProcessGameBase)==sizeof(DonorCInputProcessGameBase));
FABLE_STATIC_ASSERT(sizeof(CAInputProcess)==sizeof(DonorCAInputProcess));
#endif
void CInputProcessCreatureMovementWatchForControlAngleChange::ProcessInput(const CInputEvent& event,CProcessedInput&) {
 C2DVector movement;
 if(GetControlMovementFromGameActionEvent(GAME_ACTION_MOVEMENT,event,movement)) {
  C2DVector direction=movement;
  float length=movement.GetLength();
  if(length>0.0001f){float scale=1.0f/length;direction.X=movement.X*scale;direction.Y=movement.Y*scale;}
  else {const CGameCameraManager* manager;if(World->PCurrentGameCamera->GetCameraManager(&manager))const_cast<CGameCameraManager*>(manager)->SetNoControlMovement();}
  if(direction.X*LastJoyDirection.X+direction.Y*LastJoyDirection.Y<0.95f)World->PCurrentGameCamera->SetChangeInControlDirection();
  LastJoyDirection=direction;
 }
}
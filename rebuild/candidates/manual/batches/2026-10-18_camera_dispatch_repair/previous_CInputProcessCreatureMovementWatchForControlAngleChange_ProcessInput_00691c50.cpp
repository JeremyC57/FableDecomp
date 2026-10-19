// CInputProcessCreatureMovementWatchForControlAngleChange::ProcessInput -- normalize movement, signal camera changes, remember direction
// Retail00691C50 length206; GetControlMovementFromGameActionEvent00687D00.
// Actual donor headers retained; native camera manager prefix differs because
// CamTools vector16->12, AvailableCamPoints list12->4. ControlNoControlMovement+BD.
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
#include "engine/CWorld.h"
#include "engine/CProcessedInput.h"
struct CInputEvent;
struct C2DVector {float X,Y;C2DVector(){} C2DVector(float x,float y):X(x),Y(y){} float GetLength()const{return (float)sqrt(X*X+Y*Y);} };
struct C3DVector;
struct CCamera;
struct CEnginePrimitivePrepareInfo;
struct CGameCameraManager;
struct CAIGameCameraBase {
    virtual ~CAIGameCameraBase();
    virtual CCamera GetRenderCamera(float)const=0; // slot1
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&)const=0;
    virtual bool IsPosInViewingFrustrum(const C3DVector&)const=0;
    virtual bool Is2DAreaInViewingFrustrum(const C3DVector&,float)const=0;
    virtual bool IsVolumeInViewingFrustrum(const C3DVector&,float)const=0;
    virtual bool IsLineOfSightToPos(const C3DVector&)const=0;
    virtual float GetControlAngleXY()const=0;
    virtual void SetChangeInControlDirection()=0; //slot8
    virtual bool GetCameraManager(CGameCameraManager**)=0; //slot9; const overload slot10
};
// Prefix view uses genuine fields in their native representation, without invented padding.
struct CCamera {float WorldPosition[3];float Up[3];float Forward[3];bool HeightLocked,ViewVecZLocked;unsigned int FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;};
struct NativeCameraTools { CGameCameraManager* CameraManager;void* ScanThingsStart;void* ScanThingsFinish;void* ScanThingsEnd;float CameraMeshOffsetDistance,CameraCreatureOffsetDistance,CameraLandscapeOffsetDistance,CameraThemeOffsetDistance; };
struct CGameCameraManager : CAIGameCameraBase {
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
struct CBaseClass {virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {virtual ~CBaseClassNonCopyable();};
struct CLinkedListPosition {CBaseClass* Data;CLinkedListPosition* Next;CLinkedListPosition* Previous;};
struct CInputProcessManager;
struct CAInputProcess:CBaseClassNonCopyable {
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
struct CInputProcessGameBase:CAInputProcess {
 CWorld* World;
 CPlayer* MyPlayer;
 CGameDefinitionManager* DefinitionManager;
 CMainGameComponent* Component;
 CGamePlayerInterface* PGamePlayerInterface;
 bool GetControlMovementFromGameActionEvent(EGameAction,const CInputEvent&,C2DVector&)const; //00687D00 enum EGameAction
};
struct CInputProcessCreatureMovementWatchForControlAngleChange:CInputProcessGameBase {
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
  else {CGameCameraManager* manager;if(World->PCurrentGameCamera->GetCameraManager(&manager))manager->ControlNoControlMovement=true;}
  if(direction.X*LastJoyDirection.X+direction.Y*LastJoyDirection.Y<0.95f)World->PCurrentGameCamera->SetChangeInControlDirection();
  LastJoyDirection=direction;
 }
}
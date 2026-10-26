// CCreatureActionBase::InitialiseActionMovement() -- divide animation displacement over usable frames or resolve diagnostic name and zero movement.
// Retail00692C80 length301. Actual actionVT1259D24 slots6 GetAnimDelayTime692460/7 GetAnimationIndex692870.
// World+3C is PMainMeshBank (not AnimationManager); A242C0 is CMeshDataBank::PeekAnimation PDBB60570.
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CMeshDataBank CMeshDataBank_PDB
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
#define CBankFile CBankFile_PDB
#include "engine/CBankFile.h"
#undef CBankFile
#define C3DAnimationInfo C3DAnimationInfo_PDB
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
#include "engine/CMeshDataBankEntry.h"
#include "fable_string.h"
class C3DVector { public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector operator*(const float& f)const{return C3DVector(X*f,Y*f,Z*f);}
 C3DVector operator/(float f)const{return *this*(1.0f/f);}
};
struct CRetailString:CCharString {CRetailString(const char*,long);CRetailString(const CRetailString&);}; //native99EBF0/99EC30, actual shared storage and destructor
class CBankFile:public CBankFile_PDB { public:CRetailString GetEntrySymbol(unsigned long)const;}; //009CC1D0 native const/byvalue string ABI
class C3DAnimationInfo:public CMeshDataBankEntry { public:CRetailString GetName()const;};
class CMeshDataBank:public CBankFile { public:const C3DAnimationInfo* PeekAnimation(unsigned long)const;}; //00A242C0
inline CRetailString C3DAnimationInfo::GetName()const{return Bank->GetEntrySymbol(Index);}
class CWorld { public:unsigned char _beforeMainMeshBank[0x3c];CMeshDataBank* PMainMeshBank;};
class CThingCreatureBase { public:
 C3DVector GetAnimMovementVector(long)const; //006626E0
 CWorld* GetWorld()const; //004C7AB0
};
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();void* PData;CThingCreatureBase* GetPItem()const;}; //00A01B50
class CCreatureActionBase { public:
 virtual ~CCreatureActionBase(); //0062ED20 deleting
 virtual void Validate(); //0099A400
 virtual void EditUpdate(float); //00692DB0
 virtual void PerformAction()=0;
 virtual void FinishAction(); //00694430
 virtual bool IsPlayingAnimation()const; //00692850
 virtual long GetAnimDelayTime()const; //00692460
 virtual long GetAnimationIndex()const; //00692870
 void* IntelligentPointer;CBaseIntelligentPointer PCreature;
 long TotalTime,ActionTime,AnimDelayTime,FramesRunning,InterruptionGroupID,InterruptionPriority,ActionID;
 CBaseIntelligentPointer PItemRemoved;long FirstSound,LastSound;unsigned long PlayingSoundID;
 float RunningTimeSeconds,LastRunningTimeSeconds;long StartFrame,PauseFrame,PauseTimer;C3DVector AnimMovementStep;
 void InitialiseActionMovement();
};
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase,AnimMovementStep)==0x54);
void CCreatureActionBase::InitialiseActionMovement(){
 long animation=GetAnimationIndex();long delay=GetAnimDelayTime();long frames=TotalTime-StartFrame-delay;
 C3DVector movement=PCreature.GetPItem()->GetAnimMovementVector(animation);
 if(frames){float inverse=1.0f/(float)frames;AnimMovementStep=movement*inverse;return;}
 long diagnosticAnimation=GetAnimationIndex();
 CMeshDataBank* bank=PCreature.GetPItem()->GetWorld()->PMainMeshBank;
 const C3DAnimationInfo* info=bank->PeekAnimation(diagnosticAnimation);
 CRetailString name=info?info->GetName():CRetailString("UNKNOWN",-1);
 AnimMovementStep.X=AnimMovementStep.Y=AnimMovementStep.Z=0.0f;
}
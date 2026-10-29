// CCreatureAction_CastAssassinRush::Initialise() -- install rush component, target and configure it, then pass optional final action.
// Retail008CD470 length298. Actual donor derived layout included; native tail -0x18.
// Actual physics slot74=7238E0 SetFacingAnglesFromRHSet; slot75 is distinct SetRHSet.
#define CCreatureAction_CastAssassinRush CCreatureAction_CastAssassinRush_PDB
#include "engine/CCreatureAction_CastAssassinRush.h"
#undef CCreatureAction_CastAssassinRush
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CTCPhysicsBase.h"
#include "engine/CClass.h"
#include "fable_string.h"
#include <string.h>
class C3DVector { public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){};};
class CRightHandedSet { public:C3DVector Up,Forward;};
struct CPhysicsMethods:CTCPhysicsBase {void SetFacingAnglesFromRHSet(const CRightHandedSet&);};
union CPhysicsSlot {void* Address;void(CPhysicsMethods::*SetFacingAnglesFromRHSet)(const CRightHandedSet&);};
inline void CPhysicsMethods::SetFacingAnglesFromRHSet(const CRightHandedSet& v){(this->*reinterpret_cast<CPhysicsSlot*>(__vftable)[74].SetFacingAnglesFromRHSet)(v);}
FABLE_STATIC_ASSERT(sizeof(CPhysicsSlot)==4);
class CCreatureActionBase;
struct CCountedActionInfo {long References;void(FABLE_FASTCALL *Destroy)(void*);void* Owner;};
struct CCountedAction {
 CCreatureActionBase* Data;CCountedActionInfo* Info;
 CCountedAction():Data(0),Info(0){}
 CCountedAction(const CCountedAction& p):Data(p.Data),Info(p.Info){if(Info)++Info->References;}
 ~CCountedAction(){if(Info&&!--Info->References){Info->Destroy(Info->Owner);delete Info;}}
};
class CThing;
// CTCAssassinRush generated header absent. PDB native member offsets are donor minus4 (TC base).
class CTCAssassinRush { public:
 unsigned char _beforeDashDistance[0x70];float DashDistance;
 unsigned char _beforeSpellLevel[0x18];long SpellLevel;CCountedAction FinalAction;void* RadialBlurFadeKey;bool UseCollision;bool IsZTargetting;
 void SetPTarget(CThing*,bool); //007A9AB0
 void SetFinalAction(CCountedAction); //008CEAE0; PDB906010; propagated CMeshDataBank label rejected
};
class CTCParamsBase;
enum ETCInterfaceType { };
class CThing:public CClass { public:
 unsigned char _beforePhysics[0x4f];CPhysicsMethods* PPhysics;
 __forceinline CPhysicsMethods* GetPhysics()const{return PPhysics;}
 void RemoveTC(ETCInterfaceType); //004C9840
 CTCAssassinRush* AddTC(const CCharString&,bool,CTCParamsBase*); //004C9D60
};
class CBaseObject;
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();void* PData;CBaseObject* GetPItem() const;}; //00A01C10/00A01B50
class CCombatActionBase { public:
 void* __vftable;void* IntelligentPointer;CBaseIntelligentPointer PCreature;
 unsigned char _beforeTarget[0x98];CBaseIntelligentPointer PTarget;unsigned char _remainingBase[0x64];
 void Initialise(); //00857520
};
class CCreatureAction_CastAssassinRush:public CCombatActionBase { public:
 bool Override;unsigned char Dir[15];float Distance;long Level;CCountedAction FinalAction;bool UseCollision;bool IsZTargetting;
 void CopyDirection(C3DVector& v)const{memcpy(&v.X,Dir,4);memcpy(&v.Y,Dir+4,4);memcpy(&v.Z,Dir+8,4);}
 void Initialise();
};
FABLE_STATIC_ASSERT(offsetof(CThing,PPhysics)==0x60);
FABLE_STATIC_ASSERT(offsetof(CCreatureAction_CastAssassinRush,Override)==0x114);
FABLE_STATIC_ASSERT(offsetof(CTCAssassinRush,UseCollision)==0x9c);
void CCreatureAction_CastAssassinRush::Initialise(){
 CCombatActionBase::Initialise();((CThing*)PCreature.GetPItem())->RemoveTC((ETCInterfaceType)(0xa9));
 CTCAssassinRush* rush;
 {CCharString name("CTCAssassinRush",-1);rush=((CThing*)PCreature.GetPItem())->AddTC(name,true,0);}
 CThing* target=((CThing*)PTarget.GetPItem());rush->SetPTarget(target,IsZTargetting);
 if(Override){
  rush->DashDistance=Distance;rush->UseCollision=UseCollision;
  CPhysicsMethods& physics=*((CThing*)PCreature.GetPItem())->GetPhysics();
  CRightHandedSet orientation;orientation.Up.X=0;orientation.Up.Y=0;orientation.Up.Z=1;CopyDirection(orientation.Forward);physics.SetFacingAnglesFromRHSet(orientation);
 }
 if(((CThing*)PCreature.GetPItem())->Type==1)rush->SpellLevel=Level;
 if(FinalAction.Data)rush->SetFinalAction(FinalAction);
}
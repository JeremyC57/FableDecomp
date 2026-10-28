// CAIStateGroup_MazeTutorialBattleTeleport::OnActivate() -- Register the creature and its valid combat target.
// Leaf: ?OnActivate@CAIStateGroup_MazeTutorialBattleTeleport@@UAEXXZ
// Identity: Own RTTI0128c2d8 slot2 and PDB OnActivate; target+20 tracked pointer, GetPItem00a01b50 twice; original RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool)006edc50, inherited GetWorld reference008fcfc0.
#include "engine/CWorld.h"
#include "engine/CAIBrain.h"
#define CAIStateGroupBase CAIStateGroupBase_HeaderLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIStateGroup_MazeTutorialBattleTeleport CAIStateGroup_MazeTutorialBattleTeleport_HeaderLayout
#include "engine/CAIStateGroup_MazeTutorialBattleTeleport.h"
#undef CAIStateGroup_MazeTutorialBattleTeleport
// Layout: active CThing header is quarantined; retail type flags+108. No creature fields beyond CThing are read. Shared CAIBrain::Creature+24 and CWorld::PThingCombatManager_ptr+76 retained.
class CThing {public:unsigned char Prefix[108];unsigned long TypeFlags;};class CThingCreatureBase:public CThing{};class CThingAICreature:public CThingCreatureBase{};
class CCombatManager {public:void RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool);};
#ifndef _MSC_VER
#define __fastcall
#endif
class CAIBrain;class CThingAICreature;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{ScriptPriorityDescriptiveZero=0};

// Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.
class CBaseClass{public:virtual ~CBaseClass(){}};
class CAIStateGroupBase:public CBaseClass {public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);virtual void OnActivate(){};protected:CWorld& GetWorld()const;};
class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CAIStateGroup_MazeBase:public CAIStateGroupBase {public:CIntelligentPointer<CThing> PTargetThing;long EnemyKnockDownFrame;};
class CAIStateGroup_MazeTutorialBattleTeleport:public CAIStateGroup_MazeBase{public:virtual void OnActivate();};
void CAIStateGroup_MazeTutorialBattleTeleport::OnActivate(){if(PTargetThing.GetPItem()->TypeFlags&8){CThingCreatureBase* target=(CThingCreatureBase*)PTargetThing.GetPItem();CThingCreatureBase* creature=PBrain->Creature;CWorld& world=GetWorld();world.PThingCombatManager_ptr->RegisterCombatant(*creature,*target,true);}}
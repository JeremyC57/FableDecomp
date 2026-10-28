// CAIStateGroup_MazeTutorialBattleUseShield::Initialise(CAIBrain& brain,CThingAICreature& creature,long priority,EScriptAIPriority scriptPriority,const CCharString& name) -- Initialise the state group and remember the current frame.
// Leaf: ?Initialise@CAIStateGroup_MazeTutorialBattleUseShield@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Identity: Own RTTI0128c350 slot1 and exact PDB Initialise; FrameOfLastShield+32. Helper008fcfc0 is original protected GetWorld() const returning reference, not propagated GetPWorld;0049d870 CWorld::GetFrame static fastcall.
#define CAIStateGroupBase CAIStateGroupBase_HeaderLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIStateGroup_MazeTutorialBattleUseShield CAIStateGroup_MazeTutorialBattleUseShield_HeaderLayout
#include "engine/CAIStateGroup_MazeTutorialBattleUseShield.h"
#undef CAIStateGroup_MazeTutorialBattleUseShield
#ifndef _MSC_VER
#define __fastcall
#endif
class CAIBrain;class CThingAICreature;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{ScriptPriorityDescriptiveZero=0};
class CWorld{public:static long __fastcall GetFrame();};
// Layout: shared state-group headers flatten the vptr and base fields. Genuine inheritance is needed for virtual Initialise and inherited GetWorld. PDB and retail agree base20 and MazeBase32; current-frame member+32.
class CBaseClass{public:virtual ~CBaseClass(){}};
class CAIStateGroupBase:public CBaseClass {public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);protected:CWorld& GetWorld()const;};
class CThing;class CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
class CAIStateGroup_MazeBase:public CAIStateGroupBase {public:CIntelligentPointer<CThing> PTargetThing;long EnemyKnockDownFrame;};
class CAIStateGroup_MazeTutorialBattleUseShield:public CAIStateGroup_MazeBase{public:long FrameOfLastShield;virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);};
void CAIStateGroup_MazeTutorialBattleUseShield::Initialise(CAIBrain& brain,CThingAICreature& creature,long priority,EScriptAIPriority scriptPriority,const CCharString& name){CAIStateGroupBase::Initialise(brain,creature,priority,scriptPriority,name);FrameOfLastShield=GetWorld().GetFrame();}
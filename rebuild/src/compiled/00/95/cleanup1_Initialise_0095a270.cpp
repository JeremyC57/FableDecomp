// CAIStateGroup_HoverAttackArea::Initialise(CAIBrain& brain,CThingAICreature& creature,long priority,EScriptAIPriority scriptPriority,const CCharString& name) -- Record the creature position before base initialisation.
// Leaf: ?Initialise@CAIStateGroup_HoverAttackArea@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Identity: Own RTTI0128c754 slot1; actual inheritance from RTTI and PDB StartPos+28; original CThingPhysical::PeekPos 006a4d00 and CAIStateGroup_EnemyCreatureBase::Initialise.
#define CAIStateGroupBase CAIStateGroupBase_HeaderLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIStateGroup_EnemyCreatureBase CAIStateGroup_EnemyCreatureBase_HeaderLayout
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
#undef CAIStateGroup_EnemyCreatureBase
#define CAIStateGroup_HoverAttack CAIStateGroup_HoverAttack_HeaderLayout
#include "engine/CAIStateGroup_HoverAttack.h"
#undef CAIStateGroup_HoverAttack
#define CAIStateGroup_HoverAttackArea CAIStateGroup_HoverAttackArea_HeaderLayout
#include "engine/CAIStateGroup_HoverAttackArea.h"
#undef CAIStateGroup_HoverAttackArea
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};
#pragma pack(pop)
class CThingPhysical{public:const C3DVector& PeekPos()const; /* 0x006A4D00 ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ (Ego_r donor order Kill/PeekPos/PeekLastPos/OnCreate) */};class CThingCreatureBase:public CThingPhysical{};class CThingAICreature:public CThingCreatureBase{};class CAIBrain;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{ScriptPriorityDescriptiveZero=0};
// Layout: shared owner headers flatten inheritance and StartPos. Real base20, FlyingThingBase36 and EnemyCreatureBase28; inherited PeekPos is CThingPhysical::PeekPos 006a4d00 (byte-identical to CThing::GetPos 004c73d0; identity by Ego_r donor order).
class CBaseClass{public:virtual ~CBaseClass(){}};
class CAIStateGroupBase:public CBaseClass {public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);};
class CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
class CAIStateGroup_FlyingThingBase:public CAIStateGroupBase{public:float MinHeightToKeepAboveGround,MinHeightToKeepAboveGroundInFlight,MaxHeightAboveGround,FlightRange;};
class CAIStateGroup_EnemyCreatureBase:public CAIStateGroupBase{public:CIntelligentPointer<CThingCreatureBase> PTargetThing;virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);};
class CAIStateGroup_HoverMovementBase:public CAIStateGroup_EnemyCreatureBase{};class CAIStateGroup_HoverAttack:public CAIStateGroup_HoverMovementBase{};
class CAIStateGroup_HoverAttackArea:public CAIStateGroup_HoverAttack{public:C3DVector StartPos;virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);};
void CAIStateGroup_HoverAttackArea::Initialise(CAIBrain& brain,CThingAICreature& creature,long priority,EScriptAIPriority scriptPriority,const CCharString& name){StartPos=creature.PeekPos();CAIStateGroup_EnemyCreatureBase::Initialise(brain,creature,priority,scriptPriority,name);}
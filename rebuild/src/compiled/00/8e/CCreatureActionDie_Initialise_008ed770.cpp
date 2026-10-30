// CCreatureAction_Die::Initialise() -- initialise the action and suspend AI for an AI creature.
// Leaf: ?Initialise@CCreatureAction_Die@@UAEXXZ
// Identity: proven; own RTTI 01287FC4 slot12 and both PDB Initialise declaration; real base hook00694D10; actual CThingAICreature::SuspendAI(bool,void*)00833A10, PDB exact name and own Suspender-set+1CC/flag+1C8 body, not propagated SetSuspendAnimation labels.
// Layout: shared CBaseObject header included unchanged under data-layout alias; local CBaseClass/CBaseObject preserve original RTTI virtual inheritance and IntelligentPointer+4. No active CThing header; original type discriminator at+0x10 is unnamed in donor layout, FastCastType is descriptive. Generated creature/action headers flatten virtual bases and omit required APIs; local real action/base and tracked-pointer chain preserve PCreature+8. No derived object size or unmodeled members are used.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing:public CBaseObject{public:unsigned char _unmodeled08[8];unsigned char FastCastType;};class CThingCreatureBase:public CThing{};class CThingAICreature:public CThingCreatureBase{public:void SuspendAI(bool,void*);};
class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CCreatureActionBase:public CBaseObject{public:CIntelligentPointer<CThingCreatureBase> PCreature;virtual void Initialise();};class CCombatActionBase:public CCreatureActionBase{public:virtual void Initialise();};
class CCreatureAction_Die:public CCreatureActionBase{public:virtual void Initialise();};
void CCreatureAction_Die::Initialise(){CCreatureActionBase::Initialise();if(PCreature.GetPItem()->FastCastType==1){CThingAICreature* creature=(CThingAICreature*)PCreature.GetPItem();creature->SuspendAI(true,0);}}
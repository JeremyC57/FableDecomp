// CAIStateGroup_MinionSitAtWork::GetPOwnerObject(CThing** out) -- return the creature building when it is alive.
// Leaf: ?GetPOwnerObject@CAIStateGroup_MinionSitAtWork@@MAE_NPAPAVCThing@@@Z
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the work building read through an inline GetCreature() const accessor
//   on the state group (retail loads the creature into eax and takes lea ecx,[eax+170h]; the direct
//   PBrain->Creature->WorkBuilding form gave mov ecx/add ecx)
// Identity: proven; own RTTI 0128B5D4 slot16 and exact protected virtual PDB bool(CThing**) API; PBrain+4->Creature+24->WorkBuilding+0x170; actual inherited CBaseIntelligentPointer::GetPItem00A01B50.
// Layout: shared CAIBrain/CThingAICreature headers included unchanged; typed tracked-pointer projection over named opaque building field. Shared state-group headers flatten CBaseClass polymorphism and omit protected virtual API, so local actual CAIStateGroupBase/UsesSeat/MinionSitOnSeatBase chain models only inherited PBrain+4. Building primary CThing base is+0 from RTTI, no building fields or size used.
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
class CThing;class CThingBuilding;class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;CThingAICreature* GetCreature() const{return PBrain->Creature;}};class CAIStateGroup_UsesSeat:public CAIStateGroupBase{};class CAIStateGroup_MinionSitOnSeatBase:public CAIStateGroup_UsesSeat{};
class CAIStateGroup_MinionSitAtWork:public CAIStateGroup_MinionSitOnSeatBase{protected:virtual bool GetPOwnerObject(CThing**);};
bool CAIStateGroup_MinionSitAtWork::GetPOwnerObject(CThing** out){CThing* owner=(CThing*)((const CIntelligentPointer<CThingBuilding>*)GetCreature()->WorkBuilding)->GetPItem();if(owner){*out=owner;return true;}return false;}
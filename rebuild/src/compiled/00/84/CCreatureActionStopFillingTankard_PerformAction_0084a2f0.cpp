// CCreatureAction_StopFillingTankard::PerformAction() -- set the creature mode for this action.
// Leaf: ?PerformAction@CCreatureAction_StopFillingTankard@@UAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven; own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; RemoveMode argument42.
// Layout: original RTTI CBaseClass/CBaseObject/CCreatureActionBase chain explicit, plus actual CClass/Thing/Physical/GameObject/Creature chain; shared CBaseObject data header included unchanged under alias. GoUnderground immediate base is CCombatActionBase.  shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.
#define FABLE_RETAIL_CCLASS
#include "engine/CTCBase.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CArray.h"

class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{};
class CBaseObject;struct CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CTCCreatureModeManager:public CTCBase{public:enum{InterfaceType=49};bool AddMode(unsigned long);void RemoveMode(unsigned long);};
class CCreatureActionBase:public CBaseObject{public:CIntelligentPointer<CThingCreatureBase> PCreature;};class CCombatActionBase:public CCreatureActionBase{public:virtual void Initialise();virtual void FinishAction();};
class CCreatureAction_StopFillingTankard:public CCreatureActionBase{public:virtual void PerformAction();};
void CCreatureAction_StopFillingTankard::PerformAction(){CTCCreatureModeManager* mode;PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &mode);mode->RemoveMode(42);}
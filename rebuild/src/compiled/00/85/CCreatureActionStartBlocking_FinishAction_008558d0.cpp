// CCreatureAction_StartBlocking::FinishAction() -- update the creature component after the combat-action base hook.
// Grade: RELOCATION_MATCH -- retried 2026-10-30 (sec3): the parked chatgpt_pass8 source grades RELOCATION_MATCH over the true extent (78 bytes); its pass8 hold was model debt (local action/base/CThing models; the generated engine headers are flattened)
// Leaf: ?FinishAction@CCreatureAction_StartBlocking@@UAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven; CTCCombat RTTI vtable0126724C slot21 returns0x30 at00739CE0; PDB SetStartingToBlock(bool) sets StartingToBlock retail0x62 (donor0x6E minus12); original navigator callee label was wrong.  own RTTI class/method slot and both PDB method declaration; real tracked PCreature+8 GetPItem00A01B50; TC bitset+0x20 and map+0x44 LowerBound0040F020; SetStartingToBlock argument0.
// Layout: shared CTCBase header included unchanged; own component storage is CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > over CArray/std::vector. Original Components donor+0x48 becomes retail+0x44; Compare/Dirty follow three-pointer data. Both PDB LowerBound template name sets confirm enum and real component base.  generated action/base headers flatten polymorphic inheritance and tracked-pointer APIs; minimal real base/PCreature chain retained. CThing header lacks usable template GetTC and bitset/map APIs, so named projection models actual bitset9 and sorted map. No false-branch pointer initialization invented; N091 requires its TC to exist.
#include "engine/CTCBase.h"
#include "engine/CArray.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

class CThingCreatureBase:public CThing{};
class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:virtual ~CBaseIntelligentPointer(){}CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
class CTCCreatureModeManager:public CTCBase{public:enum{InterfaceType=49};bool AddMode(unsigned long);void RemoveMode(unsigned long);};class CTCCombat:public CTCBase{public:enum{InterfaceType=48};void SetStartingToBlock(bool);};
class CCreatureActionBase{public:virtual ~CCreatureActionBase(){}unsigned char _unmodeled04[4];CIntelligentPointer<CThingCreatureBase> PCreature;};class CCombatActionBase:public CCreatureActionBase{public:virtual void Initialise();virtual void FinishAction();};
class CCreatureAction_StartBlocking:public CCombatActionBase{public:virtual void FinishAction();};
void CCreatureAction_StartBlocking::FinishAction(){CCombatActionBase::FinishAction();CTCCombat* nav;if(PCreature.GetPItem()->GetTC(TCI_COMBAT, &nav))nav->SetStartingToBlock(false);}
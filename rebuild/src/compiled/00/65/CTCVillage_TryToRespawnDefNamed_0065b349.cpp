// CTCVillage::TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position) -- finds a live village dummy whose dummy-villager component stands for a dead villager of that def and has it create the villager again; null when none does
// Leaf: ?TryToRespawnDefNamed@CTCVillage@@QAEPAVCThingAICreature@@ABVCCharString@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B349, 183 bytes (`ret 8`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb 0x0069CE09 + FableWin.pdb 0x0216A962
// `?TryToRespawnDefNamed@CTCVillage@@QAEPAVCThingAICreature@@ABVCCharString@@ABVC3DVector@@@Z`.
// Callees: CGameDefinitionManager::Get() 0x0044C6B0 (static `__fastcall`); CDefinitionManager::GetDefGlobalIndexFromName(
//          const CCharString&) 0x009AD410; CBaseIntelligentPointer::GetPItem() const 0x00A01B50; IS_THING_ALIVE<CThing*> 0x0040DCE0
//          and IS_THING_DEAD<CThingAICreature*> 0x0065D639 (`__fastcall`); std::bitset<>::test(size_t) const 0x00410DE0 (interface
//          0xD6, the dummy villager); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetTC);
//          CTCDummyVillager::CreateVillager(CTCVillage&, const C3DVector&, bool) 0x0072F750.
// NF196 (2026-11-20, q4522): whole-row port onto the shared engine/CTCVillage.h + engine/CTCDummyVillager.h in the shape of the
//   landed same-TU sibling 0x0065B142 CTCVillage::EnableVillagerDefTypes; the row-local CBaseObject / CThing / CThingAICreature /
//   CBaseIntelligentPointer / CIntelligentPointer / CTCDummyVillager / CTCVillage / CDefinitionManager models and the same-TU stand-in
//   bodies (IS_THING_ALIVE, IS_THING_DEAD, CreateVillager, with their sinks) are gone. Bindings: FableWin.pdb
//   ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z at 0x0040DCE0 (was ?IS_THING_ALIVE@@YI_NABQAVCBaseObject@@@Z); the PDB
//   ??$IS_THING_DEAD@PAVCThingAICreature@@@@YI_NABQAVCThingAICreature@@@Z at 0x0065D639 (the dummy's Creature is
//   CIntelligentPointer<CThingAICreature>; FableWin.pdb 0x021794DA sits in the tc_village COMDAT run beside
//   FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard> = retail 0x006610C7; was ?IS_THING_DEAD@@YI_NABQAVCThing@@@Z) and
//   the header's ?CreateVillager@CTCDummyVillager@@QAEPAVCThingAICreature@@AAVCTCVillage@@ABVC3DVector@@_N@Z at 0x0072F750 (as 0x0065B142;
//   was the pointer-form stand-in).
#include "fable_compiler.h"
// Pick-any (NF196): the TU's optimize pragmas sit after the shared includes so the header inline COMDATs keep their owner rows'
//   bodies (cmpbody equal: _List_iterator_base 0x0041C780 / 0x0041C7C0 / 0x0041C7A0, IS_THING_ALIVE<CThing*> 0x0040DCE0); the
//   row's IS_THING_DEAD<T> is written in IS_THING_ALIVE's nested-if shape, whose CThingAICreature* instance is byte-identical to the
//   landed owner 0x0065D639 (21 bytes; the `thing==0||IsDead()` spelling gives 22). Bitset test declaration-only (owner 0x00410DE0).
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TRYTORESPAWNDEFNAMED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCVillage.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CDefIndex.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return true; }
 return false;
}
template<class T> bool FABLE_FASTCALL IS_THING_DEAD(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return false; }
 return true;
}
CThingAICreature* CTCVillage::TryToRespawnDefNamed(const CCharString& def_name,const C3DVector& position) {
 CDefIndex def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName(def_name);
 for(std::list<CIntelligentPointer<CThing> >::iterator i=VillageDummies.begin();i!=VillageDummies.end();++i) {
  CThing* dummy=i->Get();
  if(IS_THING_ALIVE(dummy) && dummy->HasTC(TCI_DUMMY_VILLAGER)) {
   CTCDummyVillager* dummy_villager=static_cast<CTCDummyVillager*>(&dummy->GetTC(TCI_DUMMY_VILLAGER));
   if(IS_THING_DEAD(dummy_villager->GetCreature()) && dummy_villager->GetDefType()==def_index)
    return dummy_villager->CreateVillager(*this,position,true);
  }
 }
 return 0;
}

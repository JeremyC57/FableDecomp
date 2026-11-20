// CTCVillage::EnableGuards(bool enable) -- enable or disable every live guard dummy (disabling kills its live guard, enabling re-creates a missing one at the village owner), then clear the village's crimes and decriminalise every live villager's known crimes
// Leaf: ?EnableGuards@CTCVillage@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C45A, length 351 bytes (`leave`, `ret 4`): frame-pointer size TU
//   (Ego_r.pdb 0x0069DFE7 + FableWin.pdb 0x0216A4EB `?EnableGuards@CTCVillage@@QAEX_N@Z`), as the landed CTCVillage siblings.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE<CThing*> 0x0040DCE0, IS_THING_ALIVE<CThingAICreature*>
//   0x004A88F0 and IS_THING_ALIVE<CIntelligentPointer<CThingAICreature> > 0x004AB8F0 (all `__fastcall`); std::bitset<>::test
//   0x00410DE0 (interfaces 0xD6 and 0x4F); CVectorMap LowerBound 0x0040F020 (inlined GetTC); CTCDummyVillager::SetRespawnable
//   0x0072F0E0; CThing::Kill(bool) 0x004C9B80; CThing::GetPos 0x004C73D0; CTCDummyVillager::CreateVillager 0x0072F750;
//   CTCVillage::ClearCrimes 0x0065C2D0; CTCCreatureOpinionOfHero::DecriminaliseAllKnownCrimes 0x00684050 (interface 0x4F).
// NF196 (2026-11-20, q4522): whole-row port onto the shared engine/CTCVillage.h / CTCDummyVillager.h / CTCCreatureOpinionOfHero.h in
//   the shape of the landed same-TU sibling 0x0065B142 CTCVillage::EnableVillagerDefTypes; the row-local CBaseObject / CThing /
//   CThingAICreature / CBaseIntelligentPointer / CIntelligentPointer / CTCDummyVillager / CTCCreatureOpinionOfHero / CTCVillage models
//   and the same-TU stand-in bodies (IS_THING_ALIVE x2, CreateVillager, LowerBound) are gone. Bindings now: FableWin.pdb
//   ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z at 0x0040DCE0 (was ?IS_THING_ALIVE@@YI_NABQAVCBaseObject@@@Z),
//   ??$IS_THING_ALIVE@PAVCThingAICreature@@@@YI_NABQAVCThingAICreature@@@Z at 0x004A88F0 and ?SetRespawnable@CTCDummyVillager@@QAEX_N@Z
//   at 0x0072F0E0 (both as 0x0065B142; were ?IS_THING_ALIVE@@YI_NABQAVCThingAICreature@@@Z / ?SetEnabled@CTCDummyVillager@@QAEX_N@Z),
//   ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingAICreature@@@@@@YI_N.. at 0x004AB8F0 (declared instance, as 0x00658EC3), the header's
//   CreateVillager(CTCVillage&, ...) at 0x0072F750. The guard flag is the inline CTCDummyVillager::IsGuard() (opt-in).
#include "fable_compiler.h"
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ENABLEGUARDS
#define FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
#define FABLE_RETAIL_CTCDUMMYVILLAGER_ISGUARD_INLINE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DECRIMINALISEALLKNOWNCRIMES
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return true; }
 return false;
}
template<> bool FABLE_FASTCALL IS_THING_ALIVE<CIntelligentPointer<CThingAICreature> >(const CIntelligentPointer<CThingAICreature>& thing);   // 0x004AB8F0
void CTCVillage::EnableGuards(bool enable) {
 for(std::list<CIntelligentPointer<CThing> >::iterator i=VillageDummies.begin();i!=VillageDummies.end();++i) {
  CThing* dummy=i->Get();
  if(IS_THING_ALIVE(dummy) && dummy->HasTC(TCI_DUMMY_VILLAGER)) {
   CTCDummyVillager* dummy_villager=static_cast<CTCDummyVillager*>(&dummy->GetTC(TCI_DUMMY_VILLAGER));
   if(dummy_villager->IsGuard()) {
    dummy_villager->SetRespawnable(enable);
    if(!enable) {
     if(IS_THING_ALIVE(dummy_villager->GetCreature()))dummy_villager->GetCreature()->Kill(false);
    } else if(!IS_THING_ALIVE(dummy_villager->GetCreature())) {
     dummy_villager->CreateVillager(*this,ParentThing->GetPos(),false);
    }
   }
  }
 }
 ClearCrimes();
 for(std::list<CIntelligentPointer<CThingAICreature> >::iterator p=VillagePeople.begin();p!=VillagePeople.end();++p) {
  if(IS_THING_ALIVE(*p)) {
   CThingAICreature* person=p->Get();
   if(person->HasTC(TCI_CREATURE_OPINION_OF_HERO))
    static_cast<CTCCreatureOpinionOfHero*>(&person->GetTC(TCI_CREATURE_OPINION_OF_HERO))->DecriminaliseAllKnownCrimes();
  }
 }
}

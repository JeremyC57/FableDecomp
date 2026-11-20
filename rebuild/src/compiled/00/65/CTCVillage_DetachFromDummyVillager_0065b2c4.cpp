#include "fable_compiler.h"
// CTCVillage::DetachFromDummyVillager(CThing* villager) -- for every live village dummy whose dummy-villager component holds this villager, evict it
// Leaf: ?DetachFromDummyVillager@CTCVillage@@QAEXPAVCThing@@@Z
// Retail: Fable.exe @ 0x0065B2C4, 133 bytes (`ret 4`); size TU (unaligned entry): `optimize("s",on)`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2); IS_THING_ALIVE<CThing*>(CThing* const&) 0x0040DCE0 (__fastcall);
//          std::bitset<>::test(size_t) const 0x00410DE0 (interface 214); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
//          (inlined GetTC); CTCDummyVillager::Evict 0x0065F01D.
// NF196 (2026-11-20, q4522): whole-row port onto the shared engine/CTCVillage.h + engine/CTCDummyVillager.h in the shape of the
//   landed sibling 0x0065B142 CTCVillage::EnableVillagerDefTypes (same TU): the row-local CBaseObject / CThing / CBaseIntelligentPointer /
//   CIntelligentPointer / CTCDummyVillager / CTCVillage models are gone; IS_THING_ALIVE is the sibling's same-TU template, binding
//   FableWin.pdb ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z at 0x0040DCE0 (was ?IS_THING_ALIVE@@YI_NABQAVCBaseObject@@@Z) and the
//   component call binds the PDB ?Evict@CTCDummyVillager@@QAEXXZ at 0x0065F01D (Ego_r.pdb + FableWin.pdb; the landed self row's Leaf;
//   was the stand-in ?Detach@CTCDummyVillager@@QAEXXZ).
// Pick-any (NF196): the TU's `optimize("s", on)` sits after the shared includes, so the header inline COMDATs this object defines
//   (_List_iterator_base ctor / operator!= / _M_incr, IS_THING_ALIVE<CThing*>) keep the bodies of their owner rows 0x0041C780 /
//   0x0041C7C0 / 0x0041C7A0 / 0x0040DCE0 (cmpbody: bytes and relocations equal); with the pragma at the top, operator!= and _M_incr
//   differ (18 v 22, 7 v 7 bytes). std::bitset<274>::test is declaration-only (FABLE_RETAIL_STLP_BITSET_TEST_DECL, as at HEAD; owner
//   0x00410DE0). The row's own bytes grade the same either way (RELOCATION_MATCH).
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_DETACHFROMDUMMYVILLAGER
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
#pragma optimize("s", on)
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return true; }
 return false;
}
void CTCVillage::DetachFromDummyVillager(CThing* villager) {
 for(std::list<CIntelligentPointer<CThing> >::iterator i=VillageDummies.begin();i!=VillageDummies.end();++i) {
  CThing* dummy=i->Get();
  if(IS_THING_ALIVE(dummy) && dummy->HasTC(TCI_DUMMY_VILLAGER)) {
   CTCDummyVillager* dummy_villager=static_cast<CTCDummyVillager*>(&dummy->GetTC(TCI_DUMMY_VILLAGER));
   if(dummy_villager->GetCreature()==villager)dummy_villager->Evict();
  }
 }
}

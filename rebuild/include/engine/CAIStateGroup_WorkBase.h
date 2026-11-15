#ifndef FABLE_ENGINE_CAISTATEGROUP_WORKBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_WORKBASE_H
#include "engine/CAIStateGroupBase.h"
class CThing;
#ifdef FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP
// Retail opt-in (chunk 2023, row 0x0094C050 CAIStateGroup_SellToBuyer::Update): the body of
// ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (Ego_r.pdb, protected const; donor 0x009702A0 right before
// CAIStateGroup_SellToBuyer::Cleanup 0x00970310, as retail 0x0094BEF0 before Cleanup 0x0094BF50). A member of the class template, so
// the SellToBuyer TU instantiates it: retail expands it at three sites of Update and calls the COMDAT 0x0094BEF0 at the fourth
// (0x0094C687). Body = retail 0x0094BEF0: the creature's shop keeper interface (0x90), its shop, the shop's keeper-available flag.
// Needs FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE, FABLE_RETAIL_CTCSHOPKEEPER_SHOP, FABLE_RETAIL_CTCSHOP_AVAILABLE and the retail CThing.
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCShop.h"
#endif
// Complete original Ego_r0x535ab, same native20byte base, no own members.
template<class Base> class CAIStateGroup_WorkBase:public Base {
protected:
 bool IsInSchoolHours()const;
#ifdef FABLE_RETAIL_CAISTATEGROUP_WORKBASE_ISINWORKHOURS
 // agentC29 (rows 0x00945060 / 0x00944940 CAIStateGroup_WorkHammerAnvil / _WorkSawingWood::Update): Ego_r.pdb
 // ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ, expanded in those retail rows (the creature's TCI_VILLAGE_MEMBER
 // presence bit +0x24 bit 3, the lookup, CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsWorkTime 0x00658B20; 0x009450A2..0x00945114).
 // Needs CThing.h (GETTC), CThingAICreature.h, CTCVillageMember.h and CTCVillage.h (FABLE_RETAIL_CTCVILLAGE_ISWORKTIME) before this header.
 bool IsInWorkHours()const
 {
  CTCVillageMember* member;
  if(this->GetCreature().GetTC(&member)) {
   CTCVillage* village=member->GetPVillage();
   if(village&&village->IsWorkTime())
    return true;
  }
  return false;
 }
#else
 bool IsInWorkHours()const;
#endif
#ifdef FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP
 bool StayInShop()const
 {
  CTCShopKeeper* shop_keeper;
  if(this->GetCreature().GetTC(&shop_keeper)) {
   CTCShop* shop=shop_keeper->GetShopTC();
   if(shop&&shop->IsShopKeeperAvailable())
    return true;
  }
  return false;
 }
#else
 bool StayInShop()const;
#endif
 bool HasSeller();
 void AcquireSeller();
 bool BuildingHasVolume(const CThing*)const;
};
#endif

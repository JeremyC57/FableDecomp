#include "fable_compiler.h"
// CTCVillage::IsReportedOrUnreportedCrimeKnown() -- true when the village has recorded crimes, or when any live villager's opinion-of-hero component (interface 79) holds a crime deed that makes it run for a guard
// Leaf: ?IsReportedOrUnreportedCrimeKnown@CTCVillage@@QAE_NXZ
// Retail: Fable.exe @ 0x0065BFFA, 144 bytes; size TU (unaligned entry): `optimize("s",on)`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; IS_THING_ALIVE<CThing*>(CThing* const&) 0x0040DCE0 (__fastcall);
//          std::bitset<>::test(size_t) const 0x00410DE0 (interface 79); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0
//          (inlined GetTC); CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard>(const Filter&,
//          long*, bool) const 0x006610C7.
// NF196 (2026-11-20, q4522): whole-row port onto the shared engine/CTCVillage.h (VillagePeople list<CIntelligentPointer<CThingAICreature> >,
//   PlayersCrimes set<long>) and engine/CTCCreatureOpinionOfHero.h in the shape of the landed same-TU sibling 0x0065B142; the row-local
//   CBaseObject / CThing / CBaseIntelligentPointer / CIntelligentPointer / CTCCreatureOpinionOfHero / CTCVillage models are gone. The
//   villager is a CThing* taken from the CThingAICreature list (the derived-to-base conversion), so IS_THING_ALIVE binds FableWin.pdb
//   ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z at 0x0040DCE0 (was ?IS_THING_ALIVE@@YI_NABQAVCBaseObject@@@Z) and the deed search
//   binds the PDB member template ??$FindMostRecentDeed@VCOpinionDeedFilter_IsCrimeToRunAndFindAGuard@@@CTCCreatureOpinionOfHero@@QBEPBV...
//   at 0x006610C7 (as landed 0x0092D060; was the non-template stand-in ?FindMostRecentDeed@CTCCreatureOpinionOfHero@@QAEPAV...).
//   COpinionDeedFilter_IsCrimeToRunAndFindAGuard has no shared header: the empty filter class of the landed 0x0092D060.
// Pick-any (NF196): `optimize("s", on)` sits after the shared includes so the header inline COMDATs keep their owners' bodies
//   (see 0x0065B2C4); std::bitset<274>::test is declaration-only (FABLE_RETAIL_STLP_BITSET_TEST_DECL, owner 0x00410DE0).
// Inferred: the component lookup goes through the const CThing::GetTC (`static_cast<const CThing*>(villager)`): retail calls the
//   const CVectorMap::LowerBound 0x004365B0 at 0x0065C050; the non-const GetTC grades RELOCATION_MATCH too but binds the non-const
//   ?LowerBound@..@QAEPAU.. (0x0040F020's symbol) at 0x004365B0 (RELOC_CONFLICT). The villager stays a CThing* for IS_THING_ALIVE<CThing*>.
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ISREPORTEDORUNREPORTEDCRIMEKNOWN
#define FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FINDMOSTRECENTDEED_FILTER
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillage.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#pragma optimize("s", on)
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public: COpinionDeedFilter_IsCrimeToRunAndFindAGuard() {} };
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 if(thing!=0) { if(!thing->IsDead())return true; }
 return false;
}
bool CTCVillage::IsReportedOrUnreportedCrimeKnown() {
 if(!PlayersCrimes.empty())return true;
 for(std::list<CIntelligentPointer<CThingAICreature> >::iterator i=VillagePeople.begin();i!=VillagePeople.end();++i) {
  CThing* villager=i->Get();
  if(IS_THING_ALIVE(villager) && villager->HasTC(TCI_CREATURE_OPINION_OF_HERO)) {
   const CTCCreatureOpinionOfHero* opinion=static_cast<const CTCCreatureOpinionOfHero*>(&static_cast<const CThing*>(villager)->GetTC(TCI_CREATURE_OPINION_OF_HERO));
   if(opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(),0,true))return true;
  }
 }
 return false;
}

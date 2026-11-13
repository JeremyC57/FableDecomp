// CAIStateGroup_LookAtInterestingThings::FindInterestingThing -- choose a nearby game object weighted by interest, or defer the next search
// Grade: DIFFER(508v508) -- entry only: Creature load before filter179 initialization/vector zero stores; all subsequent instructions/operands and17branches exact
// Retail: 00919240,508 bytes; private thiscall.
// Leaf: ?FindInterestingThing@CAIStateGroup_LookAtInterestingThings@@AAEPAVCThing@@XZ
// Flags: none
#define FABLE_RETAIL_FIND_INTERESTING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CWORLD
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_LookAtInterestingThings.h"
#include "engine/CAIBrain.h"
#include "engine/CInterestingToVillagersDef.h"
#include "engine/CTCInterestingToVillagers.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/C2DBoxI.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
CThing* CAIStateGroup_LookAtInterestingThings::FindInterestingThing()
{
 std::vector<CThingGameObject*> objects;
 PeekThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),10),CThingFilter_HasTC(TCI_INTERESTING_TO_VILLAGERS),objects);
 if(objects.empty()) {
  unsigned long random=GFRandom(120,GetPWorldSeed());
  long fps=CMainGameComponent::GetConstantFPS();
  FrameToLook=CWorld::GetFrame()+(random+240)*fps;
  return 0;
 }
 float total_interest=0.0f;
 for(long n=0;n<objects.size();++n) {
  CTCInterestingToVillagers* interesting;
  objects[n]->GetTC(&interesting);
  total_interest+=interesting->PDef->Interest;
 }
 float chosen_interest=GFFloatRandom(total_interest,GetPWorldSeed());
 total_interest=0.0f;
 for(long n=0;n<objects.size();++n) {
  CTCInterestingToVillagers* interesting;
  objects[n]->GetTC(&interesting);
  total_interest+=interesting->PDef->Interest;
  if(chosen_interest<=total_interest)return objects[n];
 }
 return 0;
}

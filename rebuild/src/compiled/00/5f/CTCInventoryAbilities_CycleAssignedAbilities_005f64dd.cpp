// 005f64dd CTCInventoryAbilities::CycleAssignedAbilities
// Leaf: ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z
// Retail: 005f64dd,508 bytes.
// Flags: /O1 /Oy-
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CHILD
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CTCInventoryAbilities.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"

// Complete original helper: donor01ebd809 and native005f4b30..005f4c44.
void CTCInventoryAbilities::GetPrioritisedAbilityDefs(std::vector<CDefPointer<const CHeroAbilityDef> >& abilities)const
{
 const CThingCreatureBase& creature=static_cast<const CThingCreatureBase&>(GetParentThing());
 if(!creature.IsChild()) {
  long max_size=-1;
  for(long i=0;i<ReceivedAbilityOrder.size();++i) {
   abilities.push_back(GetAbilityPDef(ReceivedAbilityOrder[i]));
   if(ReceivedAbilityOrder[i]!=HERO_ABILITY_NULL)max_size=i;
  }
  std::vector<CDefPointer<const CHeroAbilityDef> > temp_abilities;
  for(long i=0;i<=max_size;++i)temp_abilities.push_back(abilities[i]);
  for(long i=0;i<temp_abilities.size()%GetNumAssignableSpecialMoveButtons();++i)
   temp_abilities.push_back(CDefPointer<const CHeroAbilityDef>());
  abilities=temp_abilities;
 }
}
void CTCInventoryAbilities::CycleAssignedAbilities(bool forward)
{
 std::vector<CDefPointer<const CHeroAbilityDef> > abilities;
 GetPrioritisedAbilityDefs(abilities);
 for(long button_index=0;button_index<GetNumAssignableSpecialMoveButtons();++button_index)
  SetAbilityToButton(HERO_ABILITY_NULL,button_index,false);
 if(abilities.empty())return;
 std::vector<CDefPointer<const CHeroAbilityDef> >::const_iterator it=abilities.begin();
 long original_level=std::max(CurrentAssignedAbilityLevel,-GetNumAssignableSpecialMoveButtons());
 bool valid_set=false;
 do {
  if(forward)CurrentAssignedAbilityLevel+=GetNumAssignableSpecialMoveButtons();
  else CurrentAssignedAbilityLevel-=GetNumAssignableSpecialMoveButtons();
  if(CurrentAssignedAbilityLevel>=(long)abilities.size())CurrentAssignedAbilityLevel=std::min(original_level,0L);
  else if(CurrentAssignedAbilityLevel<0) {
   CurrentAssignedAbilityLevel=abilities.size()-GetNumAssignableSpecialMoveButtons();
   if(CurrentAssignedAbilityLevel<0)CurrentAssignedAbilityLevel=0;
  }
  CurrentAssignedAbilityLevel+=CurrentAssignedAbilityLevel%GetNumAssignableSpecialMoveButtons();
  it=abilities.begin();
  if(CurrentAssignedAbilityLevel>0)it+=CurrentAssignedAbilityLevel;
  long c1=0;
  if(c1<GetNumAssignableSpecialMoveButtons()) {
   std::vector<CDefPointer<const CHeroAbilityDef> >::const_iterator ability_it=it;
   do {
   if(ability_it!=abilities.end()) {
    CDefPointer<const CHeroAbilityDef> pdef=*ability_it;
    if(!pdef.IsNull()){valid_set=true;break;}
   }
   ++c1;++ability_it;
  }while(c1<GetNumAssignableSpecialMoveButtons());
  }
 }while(!valid_set&&CurrentAssignedAbilityLevel!=original_level);
 if(valid_set) {
  for(long button_index=0;it!=abilities.end();++it) {
   CDefPointer<const CHeroAbilityDef> pdef=*it;
   if(!pdef.IsNull())SetAbilityToButton(pdef->Ability,button_index,false);
   else SetAbilityToButton(HERO_ABILITY_NULL,button_index,false);
   if(++button_index==GetNumAssignableSpecialMoveButtons())break;
  }
  if(PSoundManager) {
   CSoundInit init;init.GainFaderBits=0x20;
   CASoundBank& bank=GetParentThing().GetSampleBank();
   long sample=bank.GetSoundIndexFromSymbol("SND_MENU_04");
   PSoundManager->Get2DEmitter().PlaySample(bank,sample,true,init,false);
  }
 }
}
// CTCInventory::OpenInventoryShortcut(long) -- Open a menu and select its requested entry.
// Leaf: ?OpenInventoryShortcut@CTCInventory@@QAEXJ@Z
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_UI_ORDINARY_INLINE
// Flags: /O1 /Oy-
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CMENUENTRY
#define FABLE_STLPORT_UI_ACTIONS
#define FABLE_RETAIL_UI_ACTIONS
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGBASEDEF
#include <string.h>
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CThing.h"
#include "engine/NUISystem_CMenuEntry.h"
void CTCInventory::OpenInventoryShortcut(long screen)
{
 DeleteMenuComponents();
 if(!ParentThing.HasTC(TCI_HERO_ABILITIES) && (screen==5 || screen==8))return;
 if(!ParentThing.HasTC(TCI_INVENTORY_STATS) && screen==6)return;
 if(!PTop_Level_List)return;
 switch(screen) {
 case 0: case 1: case 2: OpenPCInventoryMenu();break;
 case 4: OpenPCMapMenu();break;
 case 5: case 8: OpenPCSkillsMenu();break;
 case 3: case 6: case 7: OpenPCStatusMenu();break;
 }
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 switch(screen) {
 case 0: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_WEAPONS"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 case 1: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_CLOTHING"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 case 2: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_ITEMS"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 case 3: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_QUESTS"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
  NUISystem::CComponent* pentry_current_quests=pentry_sub->FindChild(CCharString("CURRENT_QUESTS_ENTRY"));
  if(pentry_current_quests) {
   NUISystem::CMenuEntry* pentry_current_quests_real=static_cast<NUISystem::CMenuEntry*>(pentry_current_quests);
   pentry_current_quests_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
  }
 }
 break;
 }
 case 5: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_EXPRESSIONS"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 case 6: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_PERSONALITY"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 case 7: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_LOG_BOOK"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 case 8: {
 NUISystem::CComponent* pentry_sub=0;
 for(unsigned long count=0;count<Components.size();++count) {
  pentry_sub=Components[count]->FindChild(CCharString("PC_MENU_ENTRY_MAGIC"));
  if(pentry_sub)break;
 }
 if(pentry_sub) {
  NUISystem::CMenuEntry* pentry_sub_real=static_cast<NUISystem::CMenuEntry*>(pentry_sub);
  pentry_sub_real->OnLeftUnclicked();
 for(unsigned long count=0;count<10;++count)
  for(unsigned long sub_count=0;sub_count<Components.size();++sub_count)
   Components[sub_count]->Update(1.0f);
 }
 break;
 }
 }
}

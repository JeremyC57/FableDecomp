// CTCInventory::OpenInventoryShortcut(long) -- Open a menu and select its requested entry.
// Leaf: ?OpenInventoryShortcut@CTCInventory@@QAEXJ@Z
// Flags: /O1 /Oy-
// Retail: Fable.exe 0x0058E99F, RELOCATION_MATCH (chunk 849 rebase of the held Codex pass 12 source).
// Callees: std::bitset<274>::test 0x00410DE0 (FABLE_RETAIL_STLP_BITSET_TEST_DECL), CTCInventory::DeleteMenuComponents 0x00587A41,
//   OpenPCInventoryMenu 0x0058807D, OpenPCMapMenu 0x00589A30, OpenPCSkillsMenu 0x00587BFF, OpenPCStatusMenu 0x0058A763 (jump table
//   0x0058EF31: cases 3/6/7 -> 0x0058A763, 5/8 -> 0x00587BFF), NUISystem::CComponent / CMenuEntry members.
// Shape: Codex body; only `ParentThing->HasTC` for its reference spelling (CTCBase::ParentThing is a pointer in our retail branch).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CMenuEntry.h"
void CTCInventory::OpenInventoryShortcut(long screen)
{
 DeleteMenuComponents();
 if(!ParentThing->HasTC(TCI_HERO_ABILITIES) && (screen==5 || screen==8))return;
 if(!ParentThing->HasTC(TCI_INVENTORY_STATS) && screen==6)return;
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
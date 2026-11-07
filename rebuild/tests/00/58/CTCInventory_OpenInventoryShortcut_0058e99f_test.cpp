// Standalone behavior fixture: production method body, instrumented external dependencies.
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
class CCharString {public:const char* Text;CCharString(const char* p):Text(p){}};
namespace NUISystem {
class CComponent {public:
 const char* Name;std::vector<CComponent*> Children;unsigned int Updates,Finds,Clicks;
 CComponent(const char* n):Name(n),Updates(0),Finds(0),Clicks(0){}
 virtual ~CComponent(){}
 virtual void Update(float delta){if(delta!=1.0f)abort();++Updates;}
 virtual CComponent* FindChild(CCharString name){++Finds;for(unsigned int i=0;i<Children.size();++i)if(strcmp(Children[i]->Name,name.Text)==0)return Children[i];return 0;}
};
class CMenuEntry:public CComponent {public:CMenuEntry(const char* n):CComponent(n){}virtual void OnLeftUnclicked(){++Clicks;}};
}
enum ETCInterfaceType {TCI_HERO_ABILITIES=111,TCI_INVENTORY_STATS=20};
class CThing {public:bool Abilities,Stats;bool HasTC(ETCInterfaceType t)const{return t==TCI_HERO_ABILITIES?Abilities:Stats;}};
class CTCInventory {public:
 CThing ParentThing;std::vector<NUISystem::CComponent*> Components;NUISystem::CComponent* PTop_Level_List;unsigned int Deleted;int Menu;
 CTCInventory():PTop_Level_List(0),Deleted(0),Menu(-1){}
 void DeleteMenuComponents(){++Deleted;}
 void OpenPCInventoryMenu(){Menu=0;}void OpenPCMapMenu(){Menu=1;}void OpenPCSkillsMenu(){Menu=2;}void OpenPCStatusMenu(){Menu=3;}
 void OpenInventoryShortcut(long);
};
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

static void Check(bool condition){if(!condition)abort();}
int main(){
 const char* names[9]={"PC_MENU_ENTRY_WEAPONS","PC_MENU_ENTRY_CLOTHING","PC_MENU_ENTRY_ITEMS","PC_MENU_ENTRY_QUESTS","unused","PC_MENU_ENTRY_EXPRESSIONS","PC_MENU_ENTRY_PERSONALITY","PC_MENU_ENTRY_LOG_BOOK","PC_MENU_ENTRY_MAGIC"};
 const int menus[9]={0,0,0,3,1,2,3,3,2};
 unsigned int scenarios=0;
 for(int screen=-1;screen<=10;++screen)
 for(int flags=0;flags<8;++flags)
 for(int placement=0;placement<4;++placement)
 for(int quest_child=0;quest_child<2;++quest_child){
  CTCInventory item;item.ParentThing.Abilities=(flags&1)!=0;item.ParentThing.Stats=(flags&2)!=0;
  NUISystem::CComponent first("first"),second("second"),third("third");
  NUISystem::CMenuEntry entry(screen>=0&&screen<9?names[screen]:"unused"),current("CURRENT_QUESTS_ENTRY");
  item.Components.push_back(&first);item.Components.push_back(&second);item.Components.push_back(&third);
  if(placement<3)item.Components[placement]->Children.push_back(&entry);
  if(quest_child)entry.Children.push_back(&current);
  if(flags&4)item.PTop_Level_List=&first;
  item.OpenInventoryShortcut(screen);
  const bool gated=!(flags&4)||(!(flags&1)&&(screen==5||screen==8))||(!(flags&2)&&screen==6);
  const bool selectable=screen>=0&&screen<9&&screen!=4;
  const bool selected=!gated&&selectable&&placement<3;
  const bool extra=selected&&screen==3&&quest_child;
  Check(item.Deleted==1);
  Check(item.Menu==(gated||screen<0||screen>8?-1:menus[screen]));
  const unsigned int updates=gated?0:10+(selected?10:0)+(extra?10:0);
  Check(first.Updates==updates&&second.Updates==updates&&third.Updates==updates);
  Check(entry.Clicks==(selected?1U:0U));Check(current.Clicks==(extra?1U:0U));
  Check(entry.Finds==((selected&&screen==3)?1U:0U));
  unsigned int expected_find=!gated&&selectable?(placement<3?placement+1:3):0;
  Check(first.Finds+second.Finds+third.Finds==expected_find);
  ++scenarios;
 }
 CTCInventory empty;empty.ParentThing.Abilities=true;empty.ParentThing.Stats=true;NUISystem::CComponent root("root");empty.PTop_Level_List=&root;empty.OpenInventoryShortcut(3);Check(empty.Menu==3&&empty.Deleted==1);
 Check(scenarios==768);
 puts("0058e99f_TEST PASS");return 0;
}
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#include "engine/EUIInventoryCategory.h"
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#include "engine/CDefPointer.h"
template<class T> CDefPointer<T>::CDefPointer(T* object) : Object(const_cast<CDefPointeeBase*>(static_cast<const CDefPointeeBase*>(object))) { }
struct CCharString {const char*Text;CCharString(const char*s,long=-1):Text(s){}};
struct CWideString {const char*Text;CWideString():Text(""){}CWideString(const char*s):Text(s){}};
static long ComponentsLive=0,DefsLive=0,DefsMade=0,DefsDeleted=0;
template<class T>class CCountedPointer;
namespace NUISystem {
class CComponent {public:const char*Kind;const char*Name;CComponent(const char*k):Kind(k),Name(""){++ComponentsLive;}virtual~CComponent(){--ComponentsLive;}virtual void SetName(const CCharString&s){Name=s.Text;}};
}
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#include "engine/CCountedPointer.h"
// Controlled external factory/deleter contracts; actual final wrapper lifetime bodies execute.
template<class T> CCountedPointer<T>::CCountedPointer(T* data) : Data(data) {
 if(Data) Info=new CCPPointerInfo(1,DeleteData,(void*)Data);else Info=0;
}
template<class T> void FABLE_FASTCALL CCountedPointer<T>::DeleteData(void* data) { delete static_cast<T*>(data); }

namespace NUISystem {
class CList:public CComponent {public:std::vector<CCountedPointer<CComponent> >Children;long Offsets;CList():CComponent("UI_SUB_LIST"),Offsets(0){}void AddNonScrollingChild(CCountedPointer<CComponent>&p){Children.push_back(p);}void InitialiseOffsets(unsigned long n){Offsets=(long)n;}};
class CManager {public:static CManager*GetInstance(){static CManager m;return &m;}CComponent*CreateComponent(CCharString s,bool b){assert(!b);if(strcmp(s.Text,"UI_SUB_LIST")==0)return new CList;assert(strcmp(s.Text,"UI_BACK_FOR_LIST")==0||strcmp(s.Text,"UI_SELECT_FOR_LIST")==0||strcmp(s.Text,"UI_ITEM_TITLE")==0);return new CComponent(s.Text);}};
}
namespace {CCharString TITLE_NAME("ITEMS TITLE");}
namespace NGameText {class CDataBank {public:const CWideString GetTextBySymbol(const CCharString&s)const{return CWideString(s.Text);}};}
class CMainGameComponent {public:static CMainGameComponent*Get(){static CMainGameComponent g;return &g;}const NGameText::CDataBank&GetTextBank()const{static NGameText::CDataBank b;return b;}};
struct ItemState {bool Book,Expression,Hair,Tattoo;long Category;};
static ItemState Items[32];
static long BookQueries,ExpressionQueries,HairQueries,TattooQueries,CategoryQueries;
static unsigned long SeenOther=0,SeenUnknownNegative=0,SeenUnknownPositive=0;
class CHairCardDef:public CDefPointeeBase {public:CHairCardDef(){++DefsLive;++DefsMade;}~CHairCardDef(){assert(IVRefCount==0);--DefsLive;++DefsDeleted;}};
class CTattooDef:public CDefPointeeBase {public:CTattooDef(){++DefsLive;++DefsMade;}~CTattooDef(){assert(IVRefCount==0);--DefsLive;++DefsDeleted;}};
class CTCInventoryItem {public:static bool IsBook(long i){++BookQueries;return Items[i].Book;}static bool IsExpression(long i){++ExpressionQueries;return Items[i].Expression;}static CDefPointer<const CHairCardDef>GetHairCardDef(long i){++HairQueries;return CDefPointer<const CHairCardDef>(Items[i].Hair ? new CHairCardDef : 0);}static CDefPointer<const CTattooDef>GetTattooCardDef(long i){++TattooQueries;return CDefPointer<const CTattooDef>(Items[i].Tattoo ? new CTattooDef : 0);}static EUIInventoryCategory GetUIInventoryCategory(long i){++CategoryQueries;long c=Items[i].Category;if(c==4)++SeenOther;if(c<0)++SeenUnknownNegative;if(c>4)++SeenUnknownPositive;return(EUIInventoryCategory)c;}};
typedef std::vector<std::pair<long,long> >CItemVector;
static CItemVector Input,Accepted[5];static int CategoryCalls;static const char*Captions[5];static const char*Templates[5];static const char*Descriptions[5];static bool Usable[5];
struct FixtureMap {long Clears;FixtureMap():Clears(0){}void clear(){++Clears;}};
class CTCInventory {public:FixtureMap ItemsMenuEntries,AssignedIcons;void GetAllItems(CItemVector&out)const{for(unsigned long i=0;i<Input.size();++i)out.push_back(Input[i]);}void ConstructItemCategoryList(CWideString&title,NUISystem::CList*list,CItemVector&items,NUISystem::CComponent*parent,const CCharString&picture,const CCharString&description,bool usable){assert(CategoryCalls<5&&list&&parent&&list->Offsets==5);Accepted[CategoryCalls].clear();for(unsigned long i=0;i<items.size();++i)Accepted[CategoryCalls].push_back(items[i]);Captions[CategoryCalls]=title.Text;Templates[CategoryCalls]=picture.Text;Descriptions[CategoryCalls]=description.Text;Usable[CategoryCalls]=usable;++CategoryCalls;}NUISystem::CComponent*GetItemsListUIComponent(NUISystem::CComponent*);};
using namespace NUISystem;
using NGameText::CDataBank;

#include "engine/GFDebugPDynamicCast.h"

CComponent* CTCInventory::GetItemsListUIComponent(CComponent* pparent_list)
{
    CManager* pmanager = CManager::GetInstance();
    ItemsMenuEntries.clear();
    AssignedIcons.clear();
    CItemVector all_items;
    CItemVector trophies;
    CItemVector potions;
    CItemVector gifts;
    CItemVector produce;
    CItemVector other;
    GetAllItems(all_items);
    for (std::pair<long, long>* it = all_items.begin(); it != all_items.end(); ) {
        if (CTCInventoryItem::IsBook(it->first))
            it = all_items.erase(it);
        else
            ++it;
    }
    for (unsigned long count = 0; count < all_items.size(); ++count) {
        if (!CTCInventoryItem::IsExpression(all_items[count].first)) {
        CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
        CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
        if (!phair_card_def.IsNull() || !ptattoo_def.IsNull())
            continue;
        switch (CTCInventoryItem::GetUIInventoryCategory(all_items[count].first)) {
        case UI_INVENTORY_CATEGORY_TROPHIES:  trophies.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_POTIONS:  potions.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_GIFTS:  gifts.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_PRODUCE:  produce.push_back(all_items[count]); break;
        case UI_INVENTORY_CATEGORY_OTHER:  other.push_back(all_items[count]); continue;
        default: other.push_back(all_items[count]); continue;
        }
        }
    }
    CComponent* pcomponent = pmanager->CreateComponent("UI_SUB_LIST", false);
    CList* plist = GFDebugPDynamicCast(pcomponent,(CList*)0);
    CCountedPointer<CComponent> pbackbutton(pmanager->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> pselectbutton(pmanager->CreateComponent("UI_SELECT_FOR_LIST", false));
    plist->AddNonScrollingChild(pbackbutton);
    plist->AddNonScrollingChild(pselectbutton);
    CCountedPointer<CComponent> ptitle2(pmanager->CreateComponent("UI_ITEM_TITLE", false));
    ptitle2->SetName(TITLE_NAME);
    plist->AddNonScrollingChild(ptitle2);
    plist->InitialiseOffsets(5);
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CWideString menuparam;
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_TROPHIES");
    ConstructItemCategoryList(menuparam, plist, trophies, pparent_list, "UI_RING_PIC_TROPHIES", "TEXT_GUI_MENUGUIDE_ITEMS_TROPHIES", false);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_POTIONS");
    ConstructItemCategoryList(menuparam, plist, potions, pparent_list, "UI_RING_PIC_POTIONS", "TEXT_GUI_MENUGUIDE_ITEMS_MAGICPOTIONS", false);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_GIFTS");
    ConstructItemCategoryList(menuparam, plist, gifts, pparent_list, "UI_RING_PIC_GIFTS", "TEXT_GUI_MENUGUIDE_ITEMS_GIFTS", true);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_PRODUCE");
    ConstructItemCategoryList(menuparam, plist, produce, pparent_list, "UI_RING_PIC_PRODUCE", "TEXT_GUI_MENUGUIDE_ITEMS_PRODUCE", false);
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_OTHER");
    ConstructItemCategoryList(menuparam, plist, other, pparent_list, "UI_RING_PIC_OTHERS", "TEXT_GUI_MENUGUIDE_ITEMS_OTHERS", false);
    return pcomponent;
}

static unsigned State=23;static unsigned Next(){State=1664525u*State+1013904223u;return State;}
int main(){
 const char*captions[5]={"TEXT_GUI_MENU_TROPHIES","TEXT_GUI_MENU_POTIONS","TEXT_GUI_MENU_GIFTS","TEXT_GUI_MENU_PRODUCE","TEXT_GUI_MENU_OTHER"};
 const char*pictures[5]={"UI_RING_PIC_TROPHIES","UI_RING_PIC_POTIONS","UI_RING_PIC_GIFTS","UI_RING_PIC_PRODUCE","UI_RING_PIC_OTHERS"};
 const char*descriptions[5]={"TEXT_GUI_MENUGUIDE_ITEMS_TROPHIES","TEXT_GUI_MENUGUIDE_ITEMS_MAGICPOTIONS","TEXT_GUI_MENUGUIDE_ITEMS_GIFTS","TEXT_GUI_MENUGUIDE_ITEMS_PRODUCE","TEXT_GUI_MENUGUIDE_ITEMS_OTHERS"};
 for(int c=0;c<4096;++c){Input.clear();for(int i=0;i<32;++i){unsigned f=Next();Items[i].Book=(f&1)!=0;Items[i].Expression=(f&2)!=0;Items[i].Hair=(f&4)!=0;Items[i].Tattoo=(f&8)!=0;Items[i].Category=(long)((f>>12)%12)-3;}
  int n=c%17==0?0:(int)(Next()%48);for(int k=0;k<n;++k){long i=(long)((Next()>>8)%32),q=(long)((Next()>>10)%100)-3;Input.push_back(std::make_pair(i,q));}
  CItemVector expected[5];long expression=0,cards=0,category=0,defs=0;
  for(int k=0;k<n;++k){long i=Input[k].first;if(Items[i].Book)continue;++expression;if(Items[i].Expression)continue;++cards;defs+=Items[i].Hair+Items[i].Tattoo;if(Items[i].Hair||Items[i].Tattoo)continue;++category;long cat=Items[i].Category;if(cat<0||cat>=4)cat=4;expected[cat].push_back(Input[k]);}
  CategoryCalls=0;BookQueries=ExpressionQueries=HairQueries=TattooQueries=CategoryQueries=0;DefsMade=DefsDeleted=0;
  CTCInventory inventory;CComponent parent("parent");CComponent*result=inventory.GetItemsListUIComponent(&parent);CList*list=static_cast<CList*>(result);
  assert(inventory.ItemsMenuEntries.Clears==1&&inventory.AssignedIcons.Clears==1&&CategoryCalls==5&&list->Offsets==5&&list->Children.size()==3);
  assert(strcmp(list->Children[0]->Kind,"UI_BACK_FOR_LIST")==0&&strcmp(list->Children[1]->Kind,"UI_SELECT_FOR_LIST")==0&&strcmp(list->Children[2]->Kind,"UI_ITEM_TITLE")==0&&strcmp(list->Children[2]->Name,"ITEMS TITLE")==0);
  for(int i=0;i<3;++i)assert(list->Children[i].Info && list->Children[i].Info->RefCount==1);
  for(int i=0;i<5;++i){assert(strcmp(Captions[i],captions[i])==0&&strcmp(Templates[i],pictures[i])==0&&strcmp(Descriptions[i],descriptions[i])==0&&Usable[i]==(i==2));assert(Accepted[i].size()==expected[i].size());for(unsigned long k=0;k<expected[i].size();++k)assert(Accepted[i][k].first==expected[i][k].first&&Accepted[i][k].second==expected[i][k].second);}
  assert(BookQueries==n&&ExpressionQueries==expression&&HairQueries==cards&&TattooQueries==cards&&CategoryQueries==category&&DefsLive==0&&DefsMade==defs&&DefsDeleted==defs);
  assert(ComponentsLive==5);delete list;assert(ComponentsLive==1);
 }
 assert(SeenOther>100 && SeenUnknownNegative>100 && SeenUnknownPositive>100);
 assert(SeenOther==441 && SeenUnknownNegative==1496 && SeenUnknownPositive==1854);
 assert(ComponentsLive==0&&DefsLive==0);puts("00590235_TEST PASS");return 0;
}

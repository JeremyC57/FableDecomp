// Self-contained fixture uses the production body and actual shared class types.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <math.h>
#include <wchar.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define private public
#define protected public
// 00590235 CTCInventory::GetItemsListUIComponent
// Inferred: OTHER/default continue exits reproduce native cleanup; later debug donor uses break.
// Leaf: ?GetItemsListUIComponent@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: 0x00590235,1393 bytes,thiscall ret4.
// Flags: none
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_IMPLICIT_GENERATED_SPECIALS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_CONSOLEMENUS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEFINITION_ORIGINAL_USER_CONSTRUCTORS_PASS22
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
// Test instantiates the shared inline library implementation: FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
// Test instantiates the shared inline library implementation: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
// Test instantiates the shared inline library implementation: FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CHairCardDef.h"
#include "engine/CTattooDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CCountedPointer.h"
using namespace NUISystem;
using NGameText::CDataBank;
typedef std::vector<std::pair<long,long> > CItemVector;
namespace {extern CCharString TITLE_NAME;}

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

#include "engine/CThing.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CDefString.h"
#include "engine/C2DBoxF.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NUISystem_CDeletion.h"
namespace NUISystem {
// No shared CListState header exists; original Ego_r fields: one list<pair<long,long> > Structure.
class CListState {public:std::list<std::pair<long,long> > Structure;};
}


static long ComponentsLive=0,DefsLive=0,DefsMade=0,DefsDeleted=0;
static long StringLive=0;
static const char* Chars(const CCharString&x){return reinterpret_cast<const char*>(x.PStringData);}
static const wchar_t* Wide(const CWideString&x){return reinterpret_cast<const wchar_t*>(x.PStringData);}
static char* CopyChars(const char*p){size_t n=strlen(p)+1;char*out=(char*)malloc(n);assert(out);memcpy(out,p,n);return out;}
static wchar_t* CopyWide(const wchar_t*p){size_t n=(wcslen(p)+1)*sizeof(wchar_t);wchar_t*out=(wchar_t*)malloc(n);assert(out);memcpy(out,p,n);return out;}
CCharString::CCharString():PStringData(reinterpret_cast<CCharStringData*>(CopyChars(""))){++StringLive;}
CCharString::CCharString(const char*p,long):PStringData(reinterpret_cast<CCharStringData*>(CopyChars(p))){++StringLive;}
CCharString::CCharString(const CCharString&p):PStringData(reinterpret_cast<CCharStringData*>(CopyChars(Chars(p)))){++StringLive;}
CCharString::~CCharString(){--StringLive;free(PStringData);}
const CCharString&CCharString::operator=(const CCharString&p){if(this!=&p){free(PStringData);PStringData=reinterpret_cast<CCharStringData*>(CopyChars(Chars(p)));}return *this;}
CWideString::CWideString():PStringData(reinterpret_cast<CWideStringData*>(CopyWide(L""))){++StringLive;}
CWideString::CWideString(const wchar_t*p):PStringData(reinterpret_cast<CWideStringData*>(CopyWide(p))){++StringLive;}
CWideString::CWideString(const CWideString&p):PStringData(reinterpret_cast<CWideStringData*>(CopyWide(Wide(p)))){++StringLive;}
CWideString::~CWideString(){--StringLive;free(PStringData);}
const CWideString&CWideString::operator=(const CWideString&p){if(this!=&p){free(PStringData);PStringData=reinterpret_cast<CWideStringData*>(CopyWide(Wide(p)));}return *this;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){} CInitBaseClass::~CInitBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){} CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CTCBase::CTCBase(CThing&t):ParentThing(&t){} CTCBase::~CTCBase(){}
namespace NInventory {CTCInventoryBase::CTCInventoryBase(CThing&t):CTCBase(t){} CTCInventoryBase::~CTCInventoryBase(){}}
namespace NHeroInformationScreens {CBase::CBase(){} CBase::~CBase(){}}
CTCInventory::CTCInventory(CThing&t):NInventory::CTCInventoryBase(t){} CTCInventory::~CTCInventory(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){} CResource::~CResource(){}
CDefPointeeBase::CDefPointeeBase(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}
namespace NUISystem {
CUIInterface::~CUIInterface(){} CObserver::CObserver(){} CObserver::~CObserver(){}
CComponent::CComponent(long){} CComponent::~CComponent(){}
CChangingStateComponent::CChangingStateComponent(long x):CComponent(x){} CChangingStateComponent::~CChangingStateComponent(){}
CList::CList(long x):CChangingStateComponent(x){} CList::~CList(){}
CManager::CManager(){} CManager::~CManager(){}
}
CGame::CGame():CurrentGameComponent(0),Quit(false){} CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){} CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){} void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){} CMainGameComponent::~CMainGameComponent(){}
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
CMainGameComponentInit::CMainGameComponentInit(){} CMainGameComponentInit::~CMainGameComponentInit(){}
class FixtureHair:public CHairCardDef{public:FixtureHair(){++DefsLive;++DefsMade;}~FixtureHair(){assert(IVRefCount==0);--DefsLive;++DefsDeleted;}void Transfer(CPersistContext&)  { abort(); }
void Copy(const CDefClassBase*)  { abort(); }
unsigned long GetSizeofClass()  { abort(); }};
class FixtureTattoo:public CTattooDef{public:FixtureTattoo(){++DefsLive;++DefsMade;}~FixtureTattoo(){assert(IVRefCount==0);--DefsLive;++DefsDeleted;}void Transfer(CPersistContext&)  { abort(); }
void Copy(const CDefClassBase*)  { abort(); }
unsigned long GetSizeofClass()  { abort(); }};

void CIVCountedPointeeBase::Delete()const{delete this;}
template<class T>CDefPointer<T>::CDefPointer(T*object):Object(const_cast<CDefPointeeBase*>(static_cast<const CDefPointeeBase*>(object))){}
template<class T>CCountedPointer<T>::CCountedPointer(T*data):Data(data){Info=Data?new CCPPointerInfo(1,DeleteData,Data):0;}
template<class T>void FABLE_FASTCALL CCountedPointer<T>::DeleteData(void*data){delete static_cast<T*>(data);}
struct ItemState {bool Book,Expression,Hair,Tattoo;long Category;};
static ItemState Items[32];
static long BookQueries,ExpressionQueries,HairQueries,TattooQueries,CategoryQueries;
static unsigned long SeenOther=0,SeenUnknownNegative=0,SeenUnknownPositive=0;
bool CTCInventoryItem::IsBook(long i){++BookQueries;return Items[i].Book;}
bool CTCInventoryItem::IsExpression(long i){++ExpressionQueries;return Items[i].Expression;}
CDefPointer<const CHairCardDef>CTCInventoryItem::GetHairCardDef(long i){++HairQueries;return CDefPointer<const CHairCardDef>(Items[i].Hair?new FixtureHair:0);}
CDefPointer<const CTattooDef>CTCInventoryItem::GetTattooCardDef(long i){++TattooQueries;return CDefPointer<const CTattooDef>(Items[i].Tattoo?new FixtureTattoo:0);}
EUIInventoryCategory CTCInventoryItem::GetUIInventoryCategory(long i){++CategoryQueries;long c=Items[i].Category;if(c==4)++SeenOther;if(c<0)++SeenUnknownNegative;if(c>4)++SeenUnknownPositive;return(EUIInventoryCategory)c;}
static CItemVector Input,Accepted[5];static int CategoryCalls;static const char*Captions[5];static const char*Templates[5];static const char*Descriptions[5];static bool Usable[5];
static char CaptionStorage[5][128],TemplateStorage[5][128],DescriptionStorage[5][128];
namespace {CCharString TITLE_NAME("ITEMS TITLE");}
namespace NUISystem {
class FixtureComponent:public CComponent{public:const char*Kind;FixtureComponent(const char*k):CComponent(0),Kind(k){++ComponentsLive;}~FixtureComponent(){--ComponentsLive;}};
class FixtureList:public CList{public:std::vector<CCountedPointer<CComponent> >Children;long Offsets;
 FixtureList():CList(0),Offsets(0){++ComponentsLive;}~FixtureList(){--ComponentsLive;}
 void AddNonScrollingChild(CCountedPointer<CComponent>&p){Children.push_back(p);NonScrollingChildren.push_back(p.Data);}
 void InitialiseOffsets(unsigned long n){Offsets=(long)n;}
};
void CComponent::SetName(const CCharString&s){Name=s;}
static CManager Manager;
CManager* FABLE_FASTCALL CManager::GetInstance(){return &Manager;}
CComponent*CManager::CreateComponent(const CCharString&s,bool b){assert(!b);if(strcmp(Chars(s),"UI_SUB_LIST")==0)return new FixtureList;const char*k=strcmp(Chars(s),"UI_BACK_FOR_LIST")==0?"UI_BACK_FOR_LIST":strcmp(Chars(s),"UI_SELECT_FOR_LIST")==0?"UI_SELECT_FOR_LIST":"UI_ITEM_TITLE";assert(strcmp(Chars(s),k)==0);return new FixtureComponent(k);}
}
const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString&s)const{wchar_t out[128];int n=0;for(;Chars(s)[n];++n){assert(n<127);out[n]=(unsigned char)Chars(s)[n];}out[n]=0;return CWideString(out);}
void NInventory::CTCInventoryBase::GetAllItems(CItemVector&out)const{for(unsigned long i=0;i<Input.size();++i)out.push_back(Input[i]);}
void CTCInventory::ConstructItemCategoryList(CWideString&title,CList*list,CItemVector&items,CComponent*parent,const CCharString&picture,const CCharString&description,bool usable){assert(CategoryCalls<5&&list&&parent&&static_cast<NUISystem::FixtureList*>(list)->Offsets==5);int idx=CategoryCalls;Accepted[idx].clear();for(unsigned long i=0;i<items.size();++i)Accepted[idx].push_back(items[i]);int n=0;for(;Wide(title)[n];++n){assert(n<127);CaptionStorage[idx][n]=(char)Wide(title)[n];}CaptionStorage[idx][n]=0;strcpy(TemplateStorage[idx],Chars(picture));strcpy(DescriptionStorage[idx],Chars(description));Captions[idx]=CaptionStorage[idx];Templates[idx]=TemplateStorage[idx];Descriptions[idx]=DescriptionStorage[idx];Usable[idx]=usable;++CategoryCalls;}

static void SeedMap(std::map<long,NUISystem::CComponent*>&m){
 typedef std::pair<const long,NUISystem::CComponent*>Value;
 typedef std::_Rb_tree_node<Value> Node;
 assert(m.size()==0);Node*n=(Node*)malloc(sizeof(Node));assert(n);new(&n->_M_value_field)Value(7,(NUISystem::CComponent*)0);
 n->_M_parent=m._M_t._M_header._M_data;n->_M_left=n->_M_right=0;
 m._M_t._M_header._M_data->_M_parent=n;m._M_t._M_header._M_data->_M_left=n;m._M_t._M_header._M_data->_M_right=n;m._M_t._M_node_count=1;
 assert(m.size()==1);
}

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
namespace NInventory { void CTCInventoryBase::OnCreate() { abort(); } }
namespace NInventory { void CTCInventoryBase::InitialActivate() { abort(); } }
namespace NInventory { void CTCInventoryBase::OnInitialActivate() { abort(); } }
namespace NInventory { void CTCInventoryBase::GlobalUpdate(bool) { abort(); } }
namespace NInventory { void CTCInventoryBase::OnKill() { abort(); } }
namespace NInventory { void CTCInventoryBase::Copy(const CTCBase&) { abort(); } }
namespace NInventory { bool CTCInventoryBase::IsComponentSerialisable() const { abort(); } }
namespace NInventory { void CTCInventoryBase::UpdateMenuScrollButtonsCategory() { abort(); } }
namespace NInventory { void CTCInventoryBase::UpdateMenuScrollButtonsItem() { abort(); } }
namespace NInventory { void CTCInventoryBase::OpenInventory() { abort(); } }
namespace NInventory { void CTCInventoryBase::CloseInventory() { abort(); } }
namespace NInventory { void CTCInventoryBase::AddCloseInventoryEvent() { abort(); } }
namespace NInventory { void CTCInventoryBase::OnItemSelectedInHand(CThing&,bool&) { abort(); } }
namespace NInventory { void CTCInventoryBase::OnItemDeselected(long,bool&) { abort(); } }
namespace NInventory { void CTCInventoryBase::OnItemHighlighted(long) { abort(); } }
namespace NInventory { void CTCInventoryBase::OnChangeCategory() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessInventoryInputUp() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessInventoryInputDown() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessInventoryInputLeft() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessInventoryInputRight() { abort(); } }
namespace NInventory { float CTCInventoryBase::GetInitialCategoryRepeatDelay() const { abort(); } }
namespace NInventory { float CTCInventoryBase::GetContinuedCategoryRepeatDelay() const { abort(); } }
namespace NInventory { float CTCInventoryBase::GetInitialItemRepeatDelay() const { abort(); } }
namespace NInventory { float CTCInventoryBase::GetContinuedItemRepeatDelay() const { abort(); } }
namespace NInventory { void CTCInventoryBase::SetInventorySelectionState(NInventory::CTCInventoryBase::EInventorySelectionState) { abort(); } }
namespace NInventory { CThing * CTCInventoryBase::GetItemOfTypeInInventory(long) { abort(); } }
namespace NInventory { bool CTCInventoryBase::GetInventoryShoppingValidity() { abort(); } }
namespace NInventory { bool CTCInventoryBase::GetInventoryGuildValidity() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonAPressed() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonBPressed() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonXPressed() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonYPressed() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonAHeld() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonBHeld() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonXHeld() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonYHeld() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonAReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonBReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonXReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonYReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessDPadUpReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessDPadDownReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessDPadLeftReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessDPadRightReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonWhitePressed() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessButtonWhiteReleased() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessTradeAction() { abort(); } }
namespace NInventory { void CTCInventoryBase::ProcessAugmentAction() { abort(); } }
namespace NInventory { bool CTCInventoryBase::ProcessQuestActivateAction() { abort(); } }
namespace NInventory { void CTCInventoryBase::IncrementQuestCardItemSelection() { abort(); } }
namespace NInventory { void CTCInventoryBase::DecrementQuestCardItemSelection() { abort(); } }
namespace NInventory { void CTCInventoryBase::ReturnAllConfiscatedItems() { abort(); } }
namespace NInventory { void CTCInventoryBase::RemoveAllConfiscatedItems() { abort(); } }
namespace NInventory { bool CTCInventoryBase::GetIsEnabled() const { abort(); } }
namespace NInventory { CWideString CTCInventoryBase::GetCategoryNameFromAssociation(CThing *) const { abort(); } }
namespace NInventory { NUISystem::CComponent * CTCInventoryBase::ConstructScreen(NUISystem::CComponent *) { abort(); } }
namespace NInventory { void CTCInventoryBase::GetItemDescription(CWideString&,long,bool) const { abort(); } }
namespace NInventory { void CTCInventoryBase::RemoveItemFromInventory(long,long) { abort(); } }
namespace NInventory { bool CTCInventoryBase::AddItemToInventory(CThing&,bool,bool,long,bool) { abort(); } }
namespace NInventory { bool CTCInventoryBase::AddItemToInventory(long,long,bool,bool,long,bool) { abort(); } }
namespace NInventory { std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >& CTCInventoryBase::GetComponents() { abort(); } }
namespace NInventory { void CTCInventoryBase::OnSerialise(CPersistContext&,bool) { abort(); } }
namespace NInventory { void CTCInventoryBase::OnItemReturnedToInventory(CThing&) { abort(); } }
namespace NInventory { void CTCInventoryBase::OnItemAdded(long,bool,bool) { abort(); } }
namespace NInventory { void CTCInventoryBase::ConfiscateItems(const std::vector<NInventory::CItem,std::allocator<NInventory::CItem> >&) { abort(); } }
namespace NHeroInformationScreens { bool CBase::IsInventoryScreen() const { abort(); } }
ETCInterfaceType CTCInventory::GetInterfaceType() const { abort(); }
CCharString CTCInventory::GetClassTypeName() const { abort(); }
void CTCInventory::OnKill() { abort(); }
void CTCInventory::Copy(const CTCBase & p0) { abort(); }
void CTCInventory::OnCreate() { abort(); }
void CTCInventory::InitialActivate() { abort(); }
void CTCInventory::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) { abort(); }
void CTCInventory::OnItemSelected(long p0, bool & p1) { abort(); }
void CTCInventory::OnItemSelectedInHand(CThing & p0, bool & p1) { abort(); }
void CTCInventory::OnItemHighlighted(long p0) { abort(); }
void CTCInventory::OpenInventory() { abort(); }
void CTCInventory::CloseInventory() { abort(); }
void CTCInventory::GlobalUpdate(bool p0) { abort(); }
bool CTCInventory::IsComponentSerialisable() const { abort(); }
void CTCInventory::OnSerialise(CPersistContext & p0, bool p1) { abort(); }
void CTCInventory::OnChangeCategory() { abort(); }
void CTCInventory::GetItemDescription(CWideString & p0, long p1, bool p2) const { abort(); }
const std::vector<long,std::allocator<long> > & CTCInventory::GetInitialItems() const { abort(); }
CWideString CTCInventory::GetScreenName() const { abort(); }
EPlayerMode CTCInventory::GetPlayerMode() const { abort(); }
NHeroInformationScreens::EScreenType CTCInventory::GetScreenType() const { abort(); }
void CTCInventory::ProcessButtonAReleased() { abort(); }
void CTCInventory::ProcessButtonBReleased() { abort(); }
void CTCInventory::ProcessButtonXReleased() { abort(); }
void CTCInventory::ProcessButtonYReleased() { abort(); }
void CTCInventory::ProcessDPadUpReleased() { abort(); }
void CTCInventory::ProcessDPadDownReleased() { abort(); }
void CTCInventory::ProcessDPadLeftReleased() { abort(); }
void CTCInventory::ProcessDPadRightReleased() { abort(); }
void CTCInventory::ProcessInventoryInputLeft() { abort(); }
void CTCInventory::ProcessInventoryInputRight() { abort(); }
void CTCInventory::AddCloseInventoryEvent() { abort(); }
NUISystem::CComponent * CTCInventory::ConstructScreen(NUISystem::CComponent * p0) { abort(); }
NUISystem::CComponent * CTCInventory::ConstructScreenPC(NUISystem::CComponent * p0) { abort(); }
void CTCInventory::RemoveItemFromInventory(long p0, long p1) { abort(); }
void CTCInventory::OnItemAdded(long p0, bool p1, bool p2) { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
namespace NUISystem { void CUIInterface::Update(float) { abort(); } }
namespace NUISystem { void CUIInterface::Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *) { abort(); } }
namespace NUISystem { bool CObserver::IsObservingEvent(EEvent) { abort(); } }
namespace NUISystem { void CObserver::ObserveEvent(EEvent) { abort(); } }
namespace NUISystem { void CObserver::IgnoreEvent(EEvent) { abort(); } }
namespace NUISystem { void CObserver::IgnoreAllEvents() { abort(); } }
namespace NUISystem { void CObserver::ObserveAllEvents() { abort(); } }
namespace NUISystem { void CObserver::SetPreventObserving(bool) { abort(); } }
namespace NUISystem { void CComponent::Update(float) { abort(); } }
namespace NUISystem { void CComponent::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindChild(CCharString) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindChildForward(CCharString) { abort(); } }
namespace NUISystem { void CComponent::FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { abort(); } }
namespace NUISystem { void CComponent::FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { abort(); } }
namespace NUISystem { void CComponent::SetPAcceptButton(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindChildNonRecursive(CCharString) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindFirstChildOfType(NUISystem::EType) { abort(); } }
namespace NUISystem { bool CComponent::HasChild(NUISystem::CComponent *) const { abort(); } }
namespace NUISystem { void CComponent::SetZoom(const C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetZoom() const { abort(); } }
namespace NUISystem { void CComponent::SetColour(const CRGBColour &) { abort(); } }
namespace NUISystem { CRGBColour CComponent::GetColour() const { abort(); } }
namespace NUISystem { void CComponent::SetPosition(const C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetPosition() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetParentPosition() const { abort(); } }
namespace NUISystem { void CComponent::SetParentPosition(C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetParentZoom() const { abort(); } }
namespace NUISystem { void CComponent::SetParentZoom(C2DVector &) { abort(); } }
namespace NUISystem { CRGBColour CComponent::GetParentColour() const { abort(); } }
namespace NUISystem { void CComponent::SetParentColour(CRGBColour &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetSize() const { abort(); } }
namespace NUISystem { C2DBoxF CComponent::GetBoundingBox() const { abort(); } }
namespace NUISystem { const C2DVector & CComponent::GetRenderPosition() const { abort(); } }
namespace NUISystem { const C2DVector & CComponent::GetRenderZoom() const { abort(); } }
namespace NUISystem { const CRGBColour & CComponent::GetRenderColour() const { abort(); } }
namespace NUISystem { void CComponent::SetRenderPosition(C2DVector &) { abort(); } }
namespace NUISystem { void CComponent::SetRenderZoom(C2DVector &) { abort(); } }
namespace NUISystem { void CComponent::SetRenderColour(CRGBColour &) { abort(); } }
namespace NUISystem { void CComponent::ChangePosition(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangePositionDelta(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangeTargetPositionDelta(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::UpdatePosition(float) { abort(); } }
namespace NUISystem { void CComponent::ChangeZoom(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangeZoomDelta(const C2DVector &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::UpdateZoom(float) { abort(); } }
namespace NUISystem { void CComponent::ChangeColour(const CRGBColour &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::ChangeColourDelta(const CRGBColour &, float, bool) { abort(); } }
namespace NUISystem { void CComponent::UpdateColour(float) { abort(); } }
namespace NUISystem { void CComponent::ResetState() { abort(); } }
namespace NUISystem { void CComponent::ForceState(unsigned long, bool) { abort(); } }
namespace NUISystem { void CComponent::Initialise() { abort(); } }
namespace NUISystem { bool CComponent::HasState(unsigned long) { abort(); } }
namespace NUISystem { bool CComponent::HasStateInChildren(unsigned long) { abort(); } }
namespace NUISystem { void CComponent::ChangeState(unsigned long) { abort(); } }
namespace NUISystem { void CComponent::ChangeState(unsigned long, float) { abort(); } }
namespace NUISystem { void CComponent::ChangeState(NUISystem::CUIState &) { abort(); } }
namespace NUISystem { bool CComponent::ChangedState() { abort(); } }
namespace NUISystem { NUISystem::CUIState * CComponent::GetCurrentState() { abort(); } }
namespace NUISystem { void CComponent::SetParent(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::GetParent() const { abort(); } }
namespace NUISystem { std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & CComponent::GetChildren() { abort(); } }
namespace NUISystem { float CComponent::GetUpdateTime() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetPosition() const { abort(); } }
namespace NUISystem { CRGBColour CComponent::GetTargetColour() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetZoom() const { abort(); } }
namespace NUISystem { void CComponent::ProcessChangeState() { abort(); } }
namespace NUISystem { void CComponent::AddChild(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CComponent::SetDeletion(NUISystem::CDeletion) { abort(); } }
namespace NUISystem { NUISystem::CDeletion & CComponent::GetDeletion() { abort(); } }
namespace NUISystem { void CComponent::RemoveChild(unsigned long) { abort(); } }
namespace NUISystem { NUISystem::EType CComponent::GetType() const { abort(); } }
namespace NUISystem { void CComponent::AddAction(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddPreAction(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::Die() { abort(); } }
namespace NUISystem { std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & CComponent::GetStates() { abort(); } }
namespace NUISystem { CCharString CComponent::GetName() { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindComponent(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { unsigned long CComponent::GetCurrentStateIndex() { abort(); } }
namespace NUISystem { unsigned long CComponent::GetTargetStateIndex() const { abort(); } }
namespace NUISystem { float CComponent::CanScroll(bool, C2DBoxF &) { abort(); } }
namespace NUISystem { void CComponent::FakeChildDying(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::SetParentViewport(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::ClearHandle() { abort(); } }
namespace NUISystem { void CComponent::MakeUnavailable() { abort(); } }
namespace NUISystem { void CComponent::MakeUnavailableDynamically() { abort(); } }
namespace NUISystem { void CComponent::MakeAvailable() { abort(); } }
namespace NUISystem { void CComponent::MakeAvailableDynamically() { abort(); } }
namespace NUISystem { void CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType) { abort(); } }
namespace NUISystem { bool CComponent::IsAllowedToScroll() const { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::GetParentViewport() { abort(); } }
namespace NUISystem { bool CComponent::IsBastardChild() const { abort(); } }
namespace NUISystem { bool CComponent::IsIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsPositionIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsZoomIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsLayerIndependent() const { abort(); } }
namespace NUISystem { bool CComponent::IsDrawFromViewport() const { abort(); } }
namespace NUISystem { bool CComponent::IsPositionIsCenter() const { abort(); } }
namespace NUISystem { long CComponent::GetDefIndex() const { abort(); } }
namespace NUISystem { CDefPointer<NUISystem::CUIDef const > CComponent::LoadAndReturnDef() const { abort(); } }
namespace NUISystem { void CComponent::OnObserverRemove() { abort(); } }
namespace NUISystem { C2DVector CComponent::CanMove(const C2DVector &) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetDynamicSize() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetRenderPosition() const { abort(); } }
namespace NUISystem { bool CComponent::IsPCStyle() const { abort(); } }
namespace NUISystem { void CComponent::SetRelativeParentPosition(C2DVector &) { abort(); } }
namespace NUISystem { void CComponent::SetRelativeParentZoom(C2DVector &) { abort(); } }
namespace NUISystem { bool CComponent::IsUsingRelativeZoom() const { abort(); } }
namespace NUISystem { bool CComponent::IsUsingRelativePosition() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetRelativeRenderPosition() { abort(); } }
namespace NUISystem { C2DVector CComponent::GetRelativeRenderZoom() { abort(); } }
namespace NUISystem { void CComponent::SetRelativeRenderPosition(C2DVector) { abort(); } }
namespace NUISystem { C2DVector CComponent::SetRelativeRenderZoom(C2DVector) { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetRelativeRenderPosition() const { abort(); } }
namespace NUISystem { C2DVector CComponent::GetTargetRelativeRenderZoom() const { abort(); } }
namespace NUISystem { void CComponent::AddPositionChild(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::SetPositionParent(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CComponent::GetPositionParent() { abort(); } }
namespace NUISystem { void CComponent::PositionChildDying(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CComponent::PositionParentDying(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { long CComponent::GetRenderLayer() const { abort(); } }
namespace NUISystem { void CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { abort(); } }
namespace NUISystem { bool CComponent::IsClickable() const { abort(); } }
namespace NUISystem { void CComponent::RemoveFromHoveredList() { abort(); } }
namespace NUISystem { void CComponent::ProcessEvent(EEvent) { abort(); } }
namespace NUISystem { void CChangingStateComponent::Initialise() { abort(); } }
namespace NUISystem { void CChangingStateComponent::Update(float) { abort(); } }
namespace NUISystem { void CChangingStateComponent::ForceState(unsigned long, bool) { abort(); } }
namespace NUISystem { void CChangingStateComponent::ChangeState(unsigned long, float) { abort(); } }
namespace NUISystem { void CChangingStateComponent::ChangeState(unsigned long) { abort(); } }
namespace NUISystem { NUISystem::CUIState * CChangingStateComponent::GetCurrentState() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::HasState(unsigned long) { abort(); } }
namespace NUISystem { bool CChangingStateComponent::ChangedState() { abort(); } }
namespace NUISystem { unsigned long CChangingStateComponent::GetCurrentStateIndex() { abort(); } }
namespace NUISystem { unsigned long CChangingStateComponent::GetTargetStateIndex() const { abort(); } }
namespace NUISystem { float CChangingStateComponent::GetUpdateTime() const { abort(); } }
namespace NUISystem { void CChangingStateComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { bool CChangingStateComponent::IsPositionIndependent() const { abort(); } }
namespace NUISystem { bool CChangingStateComponent::IsZoomIndependent() const { abort(); } }
namespace NUISystem { void CChangingStateComponent::ProcessChangeState() { abort(); } }
namespace NUISystem { NUISystem::CUIState * CChangingStateComponent::GetState(unsigned long) { abort(); } }
namespace NUISystem { const NUISystem::CUIState * CChangingStateComponent::GetState(unsigned long) const { abort(); } }
namespace NUISystem { void CChangingStateComponent::UpdateStateChange() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::ChangedStateLastUpdate() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::InternalChanged() { abort(); } }
namespace NUISystem { bool CChangingStateComponent::AllChildrenChangedState() { abort(); } }
namespace NUISystem { void CChangingStateComponent::OnActivate() { abort(); } }
namespace NUISystem { void CChangingStateComponent::OnDeactivate() { abort(); } }
namespace NUISystem { void CList::Initialise() { abort(); } }
namespace NUISystem { void CList::Update(float) { abort(); } }
namespace NUISystem { void CList::ChangeState(unsigned long) { abort(); } }
namespace NUISystem { void CList::ProcessEvent(NUISystem::EEvent) { abort(); } }
namespace NUISystem { long CList::GetSelectedChild() { abort(); } }
namespace NUISystem { void CList::RemoveChild(unsigned long) { abort(); } }
namespace NUISystem { void CList::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CList::AddScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CList::Die() { abort(); } }
namespace NUISystem { void CList::AddScrollingChild(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CList::AddNonScrollingChild(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CList::AddNonScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { C2DVector CList::GetPositionOffset() { abort(); } }
namespace NUISystem { float CList::GetAlphaOffset() { abort(); } }
namespace NUISystem { void CList::SetLastInputTimeToBeNow() { abort(); } }
namespace NUISystem { void CList::SetNextScrollingChildAlpha(float) { abort(); } }
namespace NUISystem { void CList::SetNextScrollingChildPosition(C2DVector &) { abort(); } }
namespace NUISystem { void CList::AddScrollingChildAndSetOffsets(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CList::AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<NUISystem::CComponent> &) { abort(); } }
namespace NUISystem { void CList::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CList::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CList::SetActionOnScrollUp(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CList::SetActionOnScrollDown(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CList::InitialiseOffsets(unsigned long) { abort(); } }
namespace NUISystem { void CList::SetDefaultChild(long) { abort(); } }
namespace NUISystem { void CList::SetDefaultChild(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { NUISystem::CListState CList::SaveState() { abort(); } }
namespace NUISystem { void CList::RestoreState(NUISystem::CListState &) { abort(); } }
namespace NUISystem { void CList::SetSelectedChild(long) { abort(); } }
namespace NUISystem { bool CList::AllScrollingChildrenDeleting() { abort(); } }
namespace NUISystem { std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & CList::GetScrollingChildren() { abort(); } }
namespace NUISystem { std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & CList::GetNonScrollingChildren() { abort(); } }
namespace NUISystem { void CList::ScrollUp(bool,bool) { abort(); } }
namespace NUISystem { void CList::ScrollUp(unsigned long,unsigned long,bool,bool,bool) { abort(); } }
namespace NUISystem { void CList::ScrollDown(bool,bool) { abort(); } }
namespace NUISystem { void CList::ScrollDown(unsigned long,unsigned long,bool,bool,bool) { abort(); } }
namespace NUISystem { bool CList::AreThereAnyDeletions() { abort(); } }
namespace NUISystem { bool CList::IsWrapping() const { abort(); } }
namespace NUISystem { bool CList::IsScrolling() const { abort(); } }
namespace NUISystem { bool CList::IsInverted() const { abort(); } }
namespace NUISystem { void CList::ScrollUntilChild(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CList::ScrollUntilChildContaining(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { C2DVector CList::CanMove(const C2DVector &) { abort(); } }
namespace NUISystem { void CList::ChangePosition(const C2DVector &,float,bool) { abort(); } }
namespace NUISystem { NUISystem::CComponent * CList::GetScrollBar() { abort(); } }
namespace NUISystem { NUISystem::CComponent * CList::GetScrollBarOutside() { abort(); } }
namespace NUISystem { void CList::SetInitialPositionDummy(NUISystem::CComponent *) { abort(); } }
namespace NUISystem { void CList::ScrollToFirstChild() { abort(); } }
namespace NUISystem { void CList::ScrollToLastChild() { abort(); } }
namespace NUISystem { void CList::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { abort(); } }
namespace NUISystem { void CList::SetNonScrollingChildrenStates() { abort(); } }
namespace NUISystem { void CList::SetRecomputeOffsets(bool) { abort(); } }
namespace NUISystem { void CList::ScrollUpProcess() { abort(); } }
namespace NUISystem { void CList::ScrollDownProcess() { abort(); } }
namespace NUISystem { void CList::OnActivate() { abort(); } }
namespace NUISystem { void CList::DoRecomputeOffsets() { abort(); } }
namespace NUISystem { void CList::ComputeChildrenVisibility() { abort(); } }
namespace NUISystem { void CList::ComputeScrollBarPosition() { abort(); } }
namespace NUISystem { void CList::ComputeScrollBarZoom() { abort(); } }
namespace NUISystem { bool CManager::GetMouseDisabled() { abort(); } }
namespace NUISystem { void CManager::SetKeyPressed(unsigned short) { abort(); } }
namespace NUISystem { unsigned short CManager::GetKeyPressed() { abort(); } }
namespace NUISystem { void CManager::Action(CCountedPointer<NUISystem::CAction> &) { abort(); } }
namespace NUISystem { void CManager::SetWeaponsInventory(CTCInventoryWeapons *) { abort(); } }
namespace NUISystem { void CManager::SetMapInventory(CTCInventoryMap *) { abort(); } }
namespace NUISystem { void CManager::SetQuestInventory(CTCInventoryQuests *) { abort(); } }
namespace NUISystem { void CManager::SetTCHeroExperienceInventory(CTCHeroExperience *) { abort(); } }
namespace NUISystem { void CManager::SetAbilitiesInventory(CTCInventoryAbilities *) { abort(); } }
namespace NUISystem { void CManager::SetExperienceInventory(CTCInventoryExperience *) { abort(); } }
namespace NUISystem { void CManager::SetInventory(CTCInventory *) { abort(); } }
namespace NUISystem { void CManager::SetInventoryClothing(CTCInventoryClothing *) { abort(); } }
namespace NUISystem { void CManager::SetInventoryTrade(CTCInventoryTrade *) { abort(); } }
namespace NUISystem { void CManager::AddThingActionParam(long) { abort(); } }
namespace NUISystem { void CManager::RemoveThingActionParam(long) { abort(); } }
namespace NUISystem { void CManager::SetPlayerThing(CThing *) { abort(); } }
namespace NUISystem { void CManager::AddUIActionParam(NUISystem::CActionParam *) { abort(); } }
namespace NUISystem { void CManager::RemoveUIActionParam(NUISystem::CActionParam *) { abort(); } }
namespace NUISystem { void CManager::ClearActionParams() { abort(); } }
namespace NUISystem { void CManager::SetNumberAugmentations(unsigned long) { abort(); } }
namespace NUISystem { unsigned long CManager::GetNumberAugmentations() const { abort(); } }
namespace NUISystem { void CManager::SetChange(NUISystem::CManager::EChange) { abort(); } }
namespace NUISystem { NUISystem::CManager::EChange CManager::GetChange() { abort(); } }
namespace NUISystem { void CManager::SetMetaLayer(long) { abort(); } }
namespace NUISystem { long CManager::GetMetaLayer() { abort(); } }
namespace NUISystem { EEngine2DLayer CManager::GetLayer(long) { abort(); } }
namespace NUISystem { void CManager::ClearBastardChild() { abort(); } }
namespace NUISystem { void CManager::SetRightStickForce(C2DVector &) { abort(); } }
namespace NUISystem { C2DVector & CManager::GetRightStickForce() { abort(); } }
namespace NUISystem { void CManager::PlaySoundW(CCharString &,long) { abort(); } }
namespace NUISystem { unsigned long CManager::PlaySample(CCharString &,long) { abort(); } }
namespace NUISystem { CDefPointer<NUISystem::CUIMiscThingsDef> CManager::GetMiscThingsDef() { abort(); } }
namespace NUISystem { CDefPointer<NUISystem::CUIIconsDef> CManager::GetIconsDef() { abort(); } }
namespace NUISystem { void CManager::AddActionsListenIgnoreOnList(NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::EEvent) { abort(); } }
namespace NUISystem { void CObservable::SetEvent(NUISystem::EEvent) { abort(); } }
namespace NUISystem { void CObservable::SetOwnedEvent(NUISystem::CObserver *,NUISystem::EEvent) { abort(); } }
namespace NUISystem { void CObservable::AddObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::AddExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::RemoveExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::RemoveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::ClearObservers() { abort(); } }
namespace NUISystem { NUISystem::CObserver * CObservable::GetExclusiveObserver() { abort(); } }
namespace NUISystem { bool CObservable::IsExclusivelyObserved() { abort(); } }
namespace NUISystem { void CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver *) { abort(); } }
namespace NUISystem { void CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver *) { abort(); } }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
static unsigned State=23;static unsigned Next(){State=1664525u*State+1013904223u;return State;}
int main(){
 const char*captions[5]={"TEXT_GUI_MENU_TROPHIES","TEXT_GUI_MENU_POTIONS","TEXT_GUI_MENU_GIFTS","TEXT_GUI_MENU_PRODUCE","TEXT_GUI_MENU_OTHER"};
 const char*pictures[5]={"UI_RING_PIC_TROPHIES","UI_RING_PIC_POTIONS","UI_RING_PIC_GIFTS","UI_RING_PIC_PRODUCE","UI_RING_PIC_OTHERS"};
 const char*descriptions[5]={"TEXT_GUI_MENUGUIDE_ITEMS_TROPHIES","TEXT_GUI_MENUGUIDE_ITEMS_MAGICPOTIONS","TEXT_GUI_MENUGUIDE_ITEMS_GIFTS","TEXT_GUI_MENUGUIDE_ITEMS_PRODUCE","TEXT_GUI_MENUGUIDE_ITEMS_OTHERS"};
 CThing owner;CGame game;CMainGameComponentInit componentInit;CMainGameComponent component(game,componentInit);CMainGameComponent::PMainGameComponent=&component;NGameText::CDataBank bank;NGameText::CDataBank*pb=&bank;memcpy(component.PTextBank,&pb,sizeof pb);
 const long initialStrings=StringLive;
 for(int c=0;c<4096;++c){assert(StringLive==initialStrings);Input.clear();for(int i=0;i<32;++i){unsigned f=Next();Items[i].Book=(f&1)!=0;Items[i].Expression=(f&2)!=0;Items[i].Hair=(f&4)!=0;Items[i].Tattoo=(f&8)!=0;Items[i].Category=(long)((f>>12)%12)-3;}
  int n=c%17==0?0:(int)(Next()%48);for(int k=0;k<n;++k){long i=(long)((Next()>>8)%32),q=(long)((Next()>>10)%100)-3;Input.push_back(std::make_pair(i,q));}
  CItemVector expected[5];long expression=0,cards=0,category=0,defs=0;
  for(int k=0;k<n;++k){long i=Input[k].first;if(Items[i].Book)continue;++expression;if(Items[i].Expression)continue;++cards;defs+=Items[i].Hair+Items[i].Tattoo;if(Items[i].Hair||Items[i].Tattoo)continue;++category;long cat=Items[i].Category;if(cat<0||cat>=4)cat=4;expected[cat].push_back(Input[k]);}
  CategoryCalls=0;BookQueries=ExpressionQueries=HairQueries=TattooQueries=CategoryQueries=0;DefsMade=DefsDeleted=0;
  CTCInventory inventory(owner);NUISystem::FixtureComponent parent("parent");SeedMap(inventory.ItemsMenuEntries);SeedMap(inventory.AssignedIcons);CComponent*result=inventory.GetItemsListUIComponent(&parent);NUISystem::FixtureList*list=static_cast<NUISystem::FixtureList*>(result);
  assert(inventory.ItemsMenuEntries.size()==0&&inventory.AssignedIcons.size()==0&&CategoryCalls==5&&list->Offsets==5&&list->Children.size()==3);
  assert(strcmp(static_cast<NUISystem::FixtureComponent*>(list->Children[0].Data)->Kind,"UI_BACK_FOR_LIST")==0&&strcmp(static_cast<NUISystem::FixtureComponent*>(list->Children[1].Data)->Kind,"UI_SELECT_FOR_LIST")==0&&strcmp(static_cast<NUISystem::FixtureComponent*>(list->Children[2].Data)->Kind,"UI_ITEM_TITLE")==0&&strcmp(Chars(list->Children[2]->Name),"ITEMS TITLE")==0);
  for(int i=0;i<3;++i)assert(list->Children[i].Info && list->Children[i].Info->RefCount==1);
  for(int i=0;i<5;++i){assert(strcmp(Captions[i],captions[i])==0&&strcmp(Templates[i],pictures[i])==0&&strcmp(Descriptions[i],descriptions[i])==0&&Usable[i]==(i==2));assert(Accepted[i].size()==expected[i].size());for(unsigned long k=0;k<expected[i].size();++k)assert(Accepted[i][k].first==expected[i][k].first&&Accepted[i][k].second==expected[i][k].second);}
  assert(BookQueries==n&&ExpressionQueries==expression&&HairQueries==cards&&TattooQueries==cards&&CategoryQueries==category&&DefsLive==0&&DefsMade==defs&&DefsDeleted==defs);
  assert(ComponentsLive==5);delete list;assert(ComponentsLive==1);
 }
 assert(SeenOther>100 && SeenUnknownNegative>100 && SeenUnknownPositive>100);
 assert(SeenOther==441 && SeenUnknownNegative==1496 && SeenUnknownPositive==1854);
 assert(ComponentsLive==0&&DefsLive==0&&StringLive==initialStrings);puts("00590235_TEST PASS");return 0;
}
#include <cassert>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>

typedef std::string CWideString;
namespace NUISystem {
class CComponent {
    std::map<std::string, CComponent*> children;
public:
    virtual ~CComponent() { }
    CComponent* FindChild(const char* name) { return children[name]; }
    void SetChild(const char* name,CComponent* child) { children[name]=child; }
};
class CText : public CComponent {
    CWideString value;
public:
    CText() { }
    void SetValue(const CWideString& text, bool update, bool propagate)
    { assert(update && propagate); value=text; }
    virtual const CWideString& GetValue() { return value; }
};
class CViewport : public CComponent {
public:
    std::vector<CComponent*> children;
    void AddFakeChild(CComponent* child) {assert(child);children.push_back(child);}
};
class CManager {
public:
    static CManager* GetInstance() { static CManager manager; return &manager; }
};
}
using namespace NUISystem;

template<class T> class CDefPointer {
    T* data;
public:
    CDefPointer(T* ptr=0) : data(ptr) { }
    bool IsNull() const { return !data; }
    T* operator->() const { return data; }
};
class CHairCardDef {
public:
    long HairObject;
    CHairCardDef() : HairObject(11) { }
};

class CTattooDef {
public:
    bool invalid;
    bool IsInvalidCustomTattoo() const { return invalid; }
};

class CAppearanceModifierDef {
public:
    long CoversBodyAreaFlags;
};

class CDefClassBase {
public:
    bool found;
    CAppearanceModifierDef def;
    bool GetSubDef(CDefPointer<const CAppearanceModifierDef>* result) const
    {
        *result = found ? &def : 0;
        return found;
    }
};

class CDefinitionManager {
public:
    CDefClassBase def;
    bool found;
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const
    {
        assert(index==11);
        return found ? &def : 0;
    }
};

class CTCInventoryItem {
public:
    static CHairCardDef hair_def;
    static CTattooDef tattoo_def;
    static bool has_hair, has_tattoo, book, expression;
    static long category;
    static bool IsBook(long index) { assert(index==99||index==100); return index==100||book; }
    static bool IsExpression(long index) { assert(index==99); return expression; }
    static long GetUIInventoryCategory(long index) { assert(index==99); return category; }
    static CDefPointer<const CHairCardDef> GetHairCardDef(long index)
    { assert(index==99); return has_hair ? &hair_def : 0; }
    static CDefPointer<const CTattooDef> GetTattooCardDef(long index)
    { assert(index==99); return has_tattoo ? &tattoo_def : 0; }
};
CHairCardDef CTCInventoryItem::hair_def;
CTattooDef CTCInventoryItem::tattoo_def;
bool CTCInventoryItem::has_hair, CTCInventoryItem::has_tattoo, CTCInventoryItem::book, CTCInventoryItem::expression;
long CTCInventoryItem::category;

class CTCInventoryStats {
public:
    static CWideString GetXString() { return " x"; }
};
CWideString GFIntToWideString(long number)
{
    char text[24];
    std::sprintf(text,"%ld",number);
    return text;
}

class CTCInventory {
public:
    typedef std::vector<std::pair<long,long> > Cards;
    CDefClassBase def;
    CDefinitionManager manager;
    Cards buckets[3];
    void GetAllItems(Cards& items) { items.push_back(std::make_pair(100L,1L));items.push_back(std::make_pair(99L,7L));items.push_back(std::make_pair(100L,4L));items.push_back(std::make_pair(99L,3L)); }
    const CDefinitionManager& GetDefinitionManager() const { return manager; }
    void ConstructStyleEntry(CComponent* entry, const Cards& cards,
                               const char* title, CComponent* parent)
    {
        assert(entry && parent);
        std::string name(title);
        unsigned i=0;
        if (name=="UI_FACIAL_HAIR_TITLE") i=1;
        else if (name=="UI_TATTOOS_TITLE") i=2;
        else assert(name=="UI_HAIRCUTS_TITLE");
        buckets[i]=cards;
    }
    void GetStyleCardsListUIComponent(long tot_num_haircut_cards, long tot_num_facial_hair_cards,
                                      long tot_num_tattoo_cards, CComponent* pparent_list);
};


template<class F,class T>T GFDebugPDynamicCast(F p,T) {return static_cast<T>(p);}
enum EUIInventoryCategory { UI_INVENTORY_CATEGORY_TROPHIES, UI_INVENTORY_CATEGORY_POTIONS,UI_INVENTORY_CATEGORY_GIFTS,UI_INVENTORY_CATEGORY_PRODUCE,UI_INVENTORY_CATEGORY_OTHER };
void CTCInventory::GetStyleCardsListUIComponent(long tot_num_haircut_cards, long tot_num_facial_hair_cards, long tot_num_tattoo_cards, CComponent* pparent_list)
{
    CManager* pmanager = CManager::GetInstance();
    std::vector<std::pair<long, long> > all_items;
    std::vector<std::pair<long, long> > haircut;
    std::vector<std::pair<long, long> > facial_hair;
    std::vector<std::pair<long, long> > tattoo;
    GetAllItems(all_items);
    for (std::vector<std::pair<long, long> >::iterator it = all_items.begin(); it != all_items.end(); ) {
        if (CTCInventoryItem::IsBook(it->first))
            it = all_items.erase(it);
        else
            ++it;
    }
    for (unsigned long count = 0; count < all_items.size(); ++count) {
        if (CTCInventoryItem::IsExpression(all_items[count].first))
            continue;
        switch (CTCInventoryItem::GetUIInventoryCategory(all_items[count].first)) {
        case UI_INVENTORY_CATEGORY_TROPHIES:
        case UI_INVENTORY_CATEGORY_POTIONS:
        case UI_INVENTORY_CATEGORY_GIFTS:
        case UI_INVENTORY_CATEGORY_PRODUCE:
            break;
        case UI_INVENTORY_CATEGORY_OTHER: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull() || !ptattoo_def.IsNull()) {
                if (!phair_card_def.IsNull()) {
                    CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                    CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                    if (!db.IsNull())
                        db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        if ((pappearance_modifier_def->CoversBodyAreaFlags & 0x80) || (pappearance_modifier_def->CoversBodyAreaFlags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (pappearance_modifier_def->CoversBodyAreaFlags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                } else if (!ptattoo_def.IsNull())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        default: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull() || !ptattoo_def.IsNull()) {
                if (!phair_card_def.IsNull()) {
                    CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                    CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                    if (!db.IsNull())
                        db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        if ((pappearance_modifier_def->CoversBodyAreaFlags & 0x80) || (pappearance_modifier_def->CoversBodyAreaFlags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (pappearance_modifier_def->CoversBodyAreaFlags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                } else if (!ptattoo_def.IsNull())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        }
    }
    CComponent* pmenu_entry = 0;
    CComponent* pmenu_entry_title = 0;
    CText* pmenu_entry_title_real = 0;
    CWideString title_value;
    CComponent* pmenu_entry_subtitle = 0;
    CText* pmenu_entry_subtitle_real = 0;
    CComponent* pviewport = pparent_list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    CViewport* pviewport_real = GFDebugPDynamicCast(pviewport,(CViewport*)0);
    pmenu_entry = pparent_list->FindChild("UI_MENU_ENTRY_HAIRCUTS");
    ConstructStyleEntry(pmenu_entry, haircut, "UI_HAIRCUTS_TITLE", pparent_list);
    pviewport_real->AddFakeChild(pmenu_entry->FindChild("UI_HAIRCUTS_DESC"));
    pmenu_entry_title = pmenu_entry->FindChild("UI_HAIRCUTS_BUTTON");
    pmenu_entry_title = pmenu_entry_title->FindChild("UI_TEXT_HAIRCUTS");
    pmenu_entry_title_real = GFDebugPDynamicCast(pmenu_entry_title,(CText*)0);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_haircut_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("UI_HAIRCUTS_SUB_TITLE");
    pmenu_entry_subtitle_real = GFDebugPDynamicCast(pmenu_entry_subtitle,(CText*)0);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("UI_MENU_ENTRY_FACIAL_HAIR");
    ConstructStyleEntry(pmenu_entry, facial_hair, "UI_FACIAL_HAIR_TITLE", pparent_list);
    pviewport_real->AddFakeChild(pmenu_entry->FindChild("UI_FACIAL_HAIR_DESC"));
    pmenu_entry_title = pmenu_entry->FindChild("UI_FACIAL_HAIR_BUTTON");
    pmenu_entry_title = pmenu_entry_title->FindChild("UI_TEXT_FACIAL_HAIR");
    pmenu_entry_title_real = GFDebugPDynamicCast(pmenu_entry_title,(CText*)0);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_facial_hair_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("UI_FACIAL_HAIR_SUB_TITLE");
    pmenu_entry_subtitle_real = GFDebugPDynamicCast(pmenu_entry_subtitle,(CText*)0);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("UI_MENU_ENTRY_TATTOOS");
    ConstructStyleEntry(pmenu_entry, tattoo, "UI_TATTOOS_TITLE", pparent_list);
    pviewport_real->AddFakeChild(pmenu_entry->FindChild("UI_TATTOOS_DESC"));
    pmenu_entry_title = pmenu_entry->FindChild("UI_TATTOOS_BUTTON");
    pmenu_entry_title = pmenu_entry_title->FindChild("UI_TEXT_TATTOOS");
    pmenu_entry_title_real = GFDebugPDynamicCast(pmenu_entry_title,(CText*)0);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_tattoo_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("UI_TATTOOS_SUB_TITLE");
    pmenu_entry_subtitle_real = GFDebugPDynamicCast(pmenu_entry_subtitle,(CText*)0);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

}
namespace HelperTest {
struct Object {mutable int refs;Object():refs(0){}};
struct CAppearanceModifierDef:Object{static const char* StatGetTypeName(){return "CAppearanceModifierDef";}};
template<class T>class CDefPointer {
public:
 T* p;
 explicit CDefPointer(T* value=0):p(value){if(p)++p->refs;}
 CDefPointer(const CDefPointer& other):p(other.p){if(p)++p->refs;}
 ~CDefPointer(){if(p)--p->refs;}
 CDefPointer& operator=(const CDefPointer& other){
  if(p!=other.p){if(p)--p->refs;p=other.p;if(p)++p->refs;}return *this;
 }
};
struct CSubDefInfo {long DefIndex;};
struct DefinitionManager {
 CAppearanceModifierDef* current;int calls;
 CDefPointer<const CAppearanceModifierDef> GetPDefFromGlobalIndex(long index){assert(index==77);++calls;return CDefPointer<const CAppearanceModifierDef>(current);}
};
struct CDefClassBase {
 DefinitionManager* PDefManager;CSubDefInfo info;bool found;
 const CSubDefInfo* GetPSubDefInfo(const char* name)const{assert(std::string(name)=="CAppearanceModifierDef");return found?&info:0;}
 template<class U> bool GetSubDef(CDefPointer<const U>* out)const;
};
template <>
bool CDefClassBase::GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CAppearanceModifierDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CAppearanceModifierDef> def(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = def;
        return true;
    }
    return false;
}

void run(){
 for(int found=0;found<2;++found)for(int incoming=0;incoming<3;++incoming)for(int previous=0;previous<3;++previous){
  CAppearanceModifierDef a,b;CAppearanceModifierDef* options[]={0,&a,&b};
  DefinitionManager mgr={options[incoming],0}; CDefClassBase owner={&mgr,{77},found!=0};
  {CDefPointer<const CAppearanceModifierDef> out(options[previous]);assert(owner.GetSubDef(&out)==(found!=0));
   const CAppearanceModifierDef* wanted=options[found?incoming:previous];
   assert(out.p==wanted && a.refs==(&a==wanted) && b.refs==(&b==wanted));assert(mgr.calls==found);}
  assert(!a.refs&&!b.refs);
 }
}
}

int main()
{
    HelperTest::run();
    const char* const entry[]={"UI_MENU_ENTRY_HAIRCUTS","UI_MENU_ENTRY_FACIAL_HAIR","UI_MENU_ENTRY_TATTOOS"};
    const char* const buttons[]={"UI_HAIRCUTS_BUTTON","UI_FACIAL_HAIR_BUTTON","UI_TATTOOS_BUTTON"};
    const char* const labels[]={"UI_TEXT_HAIRCUTS","UI_TEXT_FACIAL_HAIR","UI_TEXT_TATTOOS"};
    const char* const subs[]={"UI_HAIRCUTS_SUB_TITLE","UI_FACIAL_HAIR_SUB_TITLE","UI_TATTOOS_SUB_TITLE"};
    const char* const words[]={"Haircuts","Facial hair","Tattoos"};
    const char* const totals[]={"Haircuts x2","Facial hair x3","Tattoos x5"};
    const long body_masks[]={0,0x10,0x80,0x100,0x190};

    const char* desc[]={"UI_HAIRCUTS_DESC","UI_FACIAL_HAIR_DESC","UI_TATTOOS_DESC"};
    unsigned tested=0;
    for (long category=-1; category<7; ++category) {
        for (unsigned flags=0; flags<128; ++flags) {
            for (unsigned body=0; body<5; ++body) {
                CTCInventory inventory;
                CViewport frame;CComponent parent,menu[3],button[3],detail[3];
                CText text[3], subtitle[3];
                parent.SetChild("UI_SCROLLING_VIEWPORT_TEXT_AREA",&frame);
                const unsigned subtitle_mask=(flags>>2)&7;
                for (unsigned i=0; i<3; ++i) {
                    parent.SetChild(entry[i],&menu[i]);
                    menu[i].SetChild(buttons[i],&button[i]);menu[i].SetChild(desc[i],&detail[i]);
                    button[i].SetChild(labels[i],&text[i]);
                    text[i].SetValue(words[i],true,true);
                    if (subtitle_mask & (1U<<i)) menu[i].SetChild(subs[i],&subtitle[i]);
                }
                CTCInventoryItem::category=category;
                CTCInventoryItem::book=(flags&1)!=0;
                CTCInventoryItem::expression=(flags&2)!=0;
                CTCInventoryItem::has_hair=(flags&4)!=0;
                CTCInventoryItem::has_tattoo=(flags&8)!=0;
                CTCInventoryItem::tattoo_def.invalid=(flags&16)!=0;
                inventory.manager.found=(flags&32)!=0;
                inventory.manager.def.found=(flags&64)!=0;
                inventory.manager.def.def.CoversBodyAreaFlags=body_masks[body];
                inventory.GetStyleCardsListUIComponent(2,3,5,&parent);
                const bool skip=(flags&3) || (category>=0 && category<=3);
                unsigned wanted=3;
                if (!skip && (flags&4) && (flags&96)==96 && body)
                    wanted=body==1 ? 0 : 1;
                if (!skip && !(flags&4) && (flags&8))
                    wanted=2;
                for (unsigned i=0; i<3; ++i) {
                    assert(inventory.buckets[i].size()==(i==wanted ? 2U : 0U));
                    assert(frame.children.size()==3 && frame.children[i]==&detail[i]);
                    if (i==wanted) { assert(inventory.buckets[i][0]==std::make_pair(99L,7L));assert(inventory.buckets[i][1]==std::make_pair(99L,3L)); }
                    assert(text[i].GetValue()==totals[i]);
                    assert(subtitle[i].GetValue()==((subtitle_mask & (1U<<i)) ? totals[i] : ""));
                }
                ++tested;
            }
        }
    }
    assert(tested==5120);
    std::puts("005850fd_TEST PASS");
}
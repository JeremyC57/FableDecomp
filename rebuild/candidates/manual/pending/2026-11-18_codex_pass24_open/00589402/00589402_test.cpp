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
    CWideString GetValue() const { return value; }
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
    static bool IsBook(long index) { assert(index==99); return book; }
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
    void GetAllItems(Cards& items) { items.push_back(std::make_pair(99L,1L)); }
    const CDefinitionManager& GetDefinitionManager() const { return manager; }
    void ConstructStyleEntryPC(CComponent* entry, const Cards& cards, CComponent* view,
                               const char* title, CComponent* parent)
    {
        assert(entry && view && parent);
        std::string name(title);
        unsigned i=0;
        if (name=="PC_FACIAL_HAIR_TITLE") i=1;
        else if (name=="PC_TATTOOS_TITLE") i=2;
        else assert(name=="PC_HAIRCUTS_TITLE");
        buckets[i]=cards;
    }
    void GetStyleCardsListUIComponent(long tot_num_haircut_cards, long tot_num_facial_hair_cards,
                                      long tot_num_tattoo_cards, CComponent* pparent_list);
};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CTCInventory::GetStyleCardsListUIComponent(long tot_num_haircut_cards, long tot_num_facial_hair_cards, long tot_num_tattoo_cards, CComponent* pparent_list)
{
    CManager* pmanager = CManager::GetInstance();
    CComponent* pviewport = pparent_list->FindChild("PC_UI_FRAME");
    CComponent* pmenu_entry;
    CComponent* pmenu_entry_title;
    CComponent* pmenu_entry_subtitle;
    NUISystem::CText* pmenu_entry_title_real;
    NUISystem::CText* pmenu_entry_subtitle_real;
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
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull()) {
                CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                if (!db.IsNull()) {
                    CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                    db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        long flags = pappearance_modifier_def->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (flags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                }
            } else {
                if (ptattoo_def.IsNull())
                    continue;
                if (!ptattoo_def->IsInvalidCustomTattoo())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        default: {
            CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(all_items[count].first);
            CDefPointer<const CTattooDef> ptattoo_def = CTCInventoryItem::GetTattooCardDef(all_items[count].first);
            if (!phair_card_def.IsNull()) {
                CDefPointer<const CDefClassBase> db = GetDefinitionManager().GetPDefFromGlobalIndex(phair_card_def->HairObject);
                CDefPointer<const CAppearanceModifierDef> pappearance_modifier_def;
                if (!db.IsNull()) {
                    db->GetSubDef(&pappearance_modifier_def);
                    if (!pappearance_modifier_def.IsNull()) {
                        long flags = pappearance_modifier_def->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            facial_hair.push_back(all_items[count]);
                        else if (flags & 0x10)
                            haircut.push_back(all_items[count]);
                    }
                }
            } else {
                if (ptattoo_def.IsNull())
                    continue;
                if (!ptattoo_def->IsInvalidCustomTattoo())
                    tattoo.push_back(all_items[count]);
            }
            break;
        }
        }
    }
    CWideString title_value;
    pmenu_entry = pparent_list->FindChild("PC_MENU_ENTRY_HAIRCUTS");
    ConstructStyleEntryPC(pmenu_entry, haircut, pviewport, "PC_HAIRCUTS_TITLE", pparent_list);
    pmenu_entry_title = pmenu_entry->FindChild("PC_HAIRCUTS_BUTTON")->FindChild("PC_TEXT_HAIRCUTS");
    pmenu_entry_title_real = static_cast<NUISystem::CText*>(pmenu_entry_title);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_haircut_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("PC_HAIRCUTS_SUBTITLE");
    pmenu_entry_subtitle_real = static_cast<NUISystem::CText*>(pmenu_entry_subtitle);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("PC_MENU_ENTRY_FACIAL_HAIR");
    ConstructStyleEntryPC(pmenu_entry, facial_hair, pviewport, "PC_FACIAL_HAIR_TITLE", pparent_list);
    pmenu_entry_title = pmenu_entry->FindChild("PC_FACIAL_HAIR_BUTTON")->FindChild("PC_TEXT_FACIAL_HAIR");
    pmenu_entry_title_real = static_cast<NUISystem::CText*>(pmenu_entry_title);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_facial_hair_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("PC_FACIAL_HAIR_SUBTITLE");
    pmenu_entry_subtitle_real = static_cast<NUISystem::CText*>(pmenu_entry_subtitle);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

    pmenu_entry = pparent_list->FindChild("PC_MENU_ENTRY_TATTOOS");
    ConstructStyleEntryPC(pmenu_entry, tattoo, pviewport, "PC_TATTOOS_TITLE", pparent_list);
    pmenu_entry_title = pmenu_entry->FindChild("PC_TATTOOS_BUTTON")->FindChild("PC_TEXT_TATTOOS");
    pmenu_entry_title_real = static_cast<NUISystem::CText*>(pmenu_entry_title);
    title_value = pmenu_entry_title_real->GetValue();
    title_value += CTCInventoryStats::GetXString();
    title_value += GFIntToWideString(tot_num_tattoo_cards);
    pmenu_entry_title_real->SetValue(title_value, true, true);
    pmenu_entry_subtitle = pmenu_entry->FindChild("PC_TATTOOS_SUBTITLE");
    pmenu_entry_subtitle_real = static_cast<NUISystem::CText*>(pmenu_entry_subtitle);
    if (pmenu_entry_subtitle_real)
        pmenu_entry_subtitle_real->SetValue(title_value, true, true);

}

int main()
{
    const char* const entry[]={"PC_MENU_ENTRY_HAIRCUTS","PC_MENU_ENTRY_FACIAL_HAIR","PC_MENU_ENTRY_TATTOOS"};
    const char* const buttons[]={"PC_HAIRCUTS_BUTTON","PC_FACIAL_HAIR_BUTTON","PC_TATTOOS_BUTTON"};
    const char* const labels[]={"PC_TEXT_HAIRCUTS","PC_TEXT_FACIAL_HAIR","PC_TEXT_TATTOOS"};
    const char* const subs[]={"PC_HAIRCUTS_SUBTITLE","PC_FACIAL_HAIR_SUBTITLE","PC_TATTOOS_SUBTITLE"};
    const char* const words[]={"Haircuts","Facial hair","Tattoos"};
    const char* const totals[]={"Haircuts x2","Facial hair x3","Tattoos x5"};
    const long body_masks[]={0,0x10,0x80,0x100,0x190};

    unsigned tested=0;
    for (long category=-1; category<7; ++category) {
        for (unsigned flags=0; flags<128; ++flags) {
            for (unsigned body=0; body<5; ++body) {
                CTCInventory inventory;
                CComponent parent, frame, menu[3], button[3];
                CText text[3], subtitle[3];
                parent.SetChild("PC_UI_FRAME",&frame);
                const unsigned subtitle_mask=body ? 5U : 2U;
                for (unsigned i=0; i<3; ++i) {
                    parent.SetChild(entry[i],&menu[i]);
                    menu[i].SetChild(buttons[i],&button[i]);
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
                if (!skip && !(flags&4) && (flags&24)==8)
                    wanted=2;
                for (unsigned i=0; i<3; ++i) {
                    assert(inventory.buckets[i].size()==(i==wanted ? 1U : 0U));
                    assert(text[i].GetValue()==totals[i]);
                    assert(subtitle[i].GetValue()==((subtitle_mask & (1U<<i)) ? totals[i] : ""));
                }
                ++tested;
            }
        }
    }
    assert(tested==5120);
    std::puts("00589402_TEST PASS");
}

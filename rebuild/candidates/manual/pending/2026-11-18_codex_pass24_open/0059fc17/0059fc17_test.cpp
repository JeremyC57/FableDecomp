#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

enum { TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, TCI_APPEARANCE_MORPH, TCI_HERO, APPEARANCE_HAIR };

template<class T> class CDefPointer {
    T* data;
public:
    CDefPointer(T* d=0) : data(d) { }
    T* Get() const { return data; }
    bool IsNull() const { return data==0; }
    T* operator->() const { return data; }
    T& operator*() const { return *data; }
};

class CHairCardDef {
public:
    long HairObject;
    explicit CHairCardDef(long hair) : HairObject(hair) { }
};
class CTattooDef {
public:
    long BankIndex;
    bool RemovesAllTattoos, Permanent;
    std::string CustomFileName;
    long area;
    CTattooDef(long bank,bool removes,bool permanent,const char* file,long area_index)
        : BankIndex(bank),RemovesAllTattoos(removes),Permanent(permanent),CustomFileName(file),area(area_index) { }
    bool IsConflicting(const CTattooDef& card) const { return card.area==area; }
};

class CHeroTitleDef { };
class CThingBaseDef {
    CHeroTitleDef title;
public:
    bool GetSubDef(CDefPointer<const CHeroTitleDef>* output) const { *output=&title; return true; }
};

class CDefinitionManager {
    CThingBaseDef title_def;
public:
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* def) const
    {
        if (index!=9) return false;
        *def=&title_def;
        return true;
    }
};

class CTCHero {
public:
    long HeroTitleObjectDef;
    CTCHero() : HeroTitleObjectDef(0) { }
};

class CTCHeroMorph {
public:
    std::vector<CDefPointer<const CTattooDef> > tattoos;
    void GetTattoos(std::vector<CDefPointer<const CTattooDef> >& dest) { dest=tattoos; }
    bool HasAPermanentTattooCoveringArea() const
    { return !tattoos.empty() && tattoos.front()->Permanent; }
};

class CTCHeroAttachableAppearanceModifiers {
public:
    long worn;
    CTCHeroAttachableAppearanceModifiers() : worn(0) { }
    bool IsWearingAppearanceModifier(long hair, int type)
    { return type==APPEARANCE_HAIR && hair==worn; }
};

class CThing {
public:
    CTCHero hero;
    CTCHeroMorph morph;
    CTCHeroAttachableAppearanceModifiers modifiers;
    bool GetTC(int type, CTCHero** result) { assert(type==TCI_HERO); *result=&hero; return true; }
    bool GetTC(int type, CTCHeroMorph** result) { assert(type==TCI_APPEARANCE_MORPH); *result=&morph; return true; }
    bool GetTC(int type, CTCHeroAttachableAppearanceModifiers** result)
    { assert(type==TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS); *result=&modifiers; return true; }
};

class CTCInventoryItem {
public:
    static CDefPointer<const CHairCardDef> GetHairCardDef(long index)
    {
        static CHairCardDef card(4);
        return index==1 ? &card : 0;
    }
    static CDefPointer<const CTattooDef> GetTattooCardDef(long index)
    {
        static CTattooDef banked(20,false,false,"",1);
        static CTattooDef blank(0,true,false,"",0);
        static CTattooDef custom(-1,false,false,"red",2);
        if (index==2) return &banked;
        if (index==3) return &blank;
        if (index==4) return &custom;
        return 0;
    }
};

class CTCInventoryTrade {
    CDefinitionManager definitions;
public:
    CThing* ParentThing;
    explicit CTCInventoryTrade(CThing& thing) : ParentThing(&thing) { }
    const CDefinitionManager& GetDefinitionManager() const { return definitions; }
    bool AlreadyHaveSpecial(long def_index);
};

// The selected implementation is reproduced verbatim for this standalone fixture.
bool CTCInventoryTrade::AlreadyHaveSpecial(long def_index)
{
    CDefPointer<const CHairCardDef> phair_card_def = CTCInventoryItem::GetHairCardDef(def_index);
    if (!phair_card_def.IsNull()) {
        CTCHeroAttachableAppearanceModifiers* ptc_modifiers;
        if (ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &ptc_modifiers) &&
            ptc_modifiers->IsWearingAppearanceModifier(phair_card_def->HairObject, APPEARANCE_HAIR))
            return true;
    }
    CDefPointer<const CTattooDef> ptattoo_card_def = CTCInventoryItem::GetTattooCardDef(def_index);
    if (!ptattoo_card_def.IsNull()) {
        CTCHeroMorph* ptc_morph;
        ParentThing->GetTC(TCI_APPEARANCE_MORPH, &ptc_morph);
        if (ptc_morph) {
            std::vector<CDefPointer<const CTattooDef> > tattoos;
            ptc_morph->GetTattoos(tattoos);
            if (tattoos.empty()) {
                if (ptattoo_card_def->RemovesAllTattoos)
                    return true;
                return false;
            }
            const CTattooDef* card = ptattoo_card_def.Get();
            if (card->RemovesAllTattoos) {
                if (tattoos.size() == 1 && ptc_morph->HasAPermanentTattooCoveringArea())
                    return true;
                return false;
            }
            unsigned long tattoo_count = tattoos.size();
            for (unsigned long count = 0; count < tattoo_count; ++count) {
                const CTattooDef* tattoo = tattoos[count].Get();
                if (tattoo->BankIndex == card->BankIndex) {
                    if (tattoo->BankIndex != -1 || card->BankIndex != -1)
                        return true;
                    if (tattoo->CustomFileName == card->CustomFileName)
                        return true;
                } else if (tattoo->Permanent && tattoo->IsConflicting(*card))
                    return true;
            }
        }
    }
    CDefPointer<const CThingBaseDef> pbase_def;
    CDefPointer<const CHeroTitleDef> ptitle_def;
    if (GetDefinitionManager().GetDef(def_index, &pbase_def) && pbase_def->GetSubDef(&ptitle_def)) {
        CTCHero* ptc_hero;
        ParentThing->GetTC(TCI_HERO, &ptc_hero);
        if (ptc_hero && ptc_hero->HeroTitleObjectDef == def_index)
            return true;
    }
    return false;
}

int main()
{
    CThing thing;
    CTCInventoryTrade trade(thing);
    assert(!trade.AlreadyHaveSpecial(1));
    thing.modifiers.worn=4;
    assert(trade.AlreadyHaveSpecial(1));
    assert(!trade.AlreadyHaveSpecial(2));
    assert(trade.AlreadyHaveSpecial(3));

    const CTattooDef banked(20,false,false,"",3), blue(-1,false,false,"blue",2),
                     red(-1,false,false,"red",3), permanent(21,false,true,"",1), other(22,false,false,"",3);
    thing.morph.tattoos.push_back(&banked);
    assert(trade.AlreadyHaveSpecial(2) && !trade.AlreadyHaveSpecial(3));
    thing.morph.tattoos[0]=&blue;
    assert(!trade.AlreadyHaveSpecial(4));
    thing.morph.tattoos.push_back(&red);
    assert(trade.AlreadyHaveSpecial(4));
    thing.morph.tattoos.clear();
    thing.morph.tattoos.push_back(&permanent);
    assert(trade.AlreadyHaveSpecial(2) && trade.AlreadyHaveSpecial(3));
    thing.morph.tattoos.push_back(&other);
    assert(!trade.AlreadyHaveSpecial(3));

    assert(!trade.AlreadyHaveSpecial(9));
    thing.hero.HeroTitleObjectDef=9;
    assert(trade.AlreadyHaveSpecial(9));
    assert(!trade.AlreadyHaveSpecial(10));
    std::puts("0059fc17_TEST PASS");
}

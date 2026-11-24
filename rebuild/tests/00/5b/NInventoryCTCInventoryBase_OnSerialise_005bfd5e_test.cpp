#include <stdio.h>
#include <string.h>
#include <vector>

struct CCharString {
    long id;
    CCharString(long value = 0) : id(value) {}
};
struct CWorld {
    bool quest;
    bool IsSerialisingNonPersistentQuestItems() const { return quest; }
};
struct CGameDefinitionManager {
    mutable std::vector<long> queries;
    CCharString GetDefNameFromGlobalIndex(long index) const {
        queries.push_back(index);
        return CCharString(index + 1000);
    }
};
namespace NInventory {
struct CItem {
    long id;
    CItem(long value = 0) : id(value) {}
};
struct CCategory {
    long selected;
    long GetHighlightedItem() const { return selected; }
};
}
class CPersistContext {
public:
    bool saving, valid;
    unsigned step;
    std::vector<NInventory::CItem> inventory, seized;
    std::vector<CCharString> names;
    CPersistContext(bool save) : saving(save), valid(true), step(0) {}
    bool IsSaving() const { return saving; }
    void TransferVector(const char* name, std::vector<NInventory::CItem>& items) {
        const bool first = step == 0 && strcmp(name,"InventoryItems") == 0;
        const bool third = step == 2 && strcmp(name,"ConfiscatedItems2") == 0;
        if (!first && !third) valid = false;
        std::vector<NInventory::CItem>& stored = first ? inventory : seized;
        if (saving) stored = items;
        else {
            if (!items.empty()) valid = false;
            items = stored;
        }
        ++step;
    }
    void TransferVector(const char* name, std::vector<CCharString>& values) {
        if (step != 1 || strcmp(name,"SelectedInventoryItems") != 0) valid = false;
        if (saving) names = values;
        else {
            if (!values.empty()) valid = false;
            values = names;
        }
        ++step;
    }
};
template<class T> class Owned {
    T* data;
    Owned(const Owned&);
    Owned& operator=(const Owned&);
public:
    unsigned resets;
    Owned() : data(0), resets(0) {}
    ~Owned() { delete data; }
    void Reset(T* p) { delete data; data=p; ++resets; }
    T& operator*() const { return *data; }
};
namespace NInventory {
class CTCInventoryBase {
public:
    CWorld world;
    CGameDefinitionManager definitions;
    std::vector<CCategory> Categories;
    std::vector<CItem> ConfiscatedItems;
    Owned<std::vector<CItem> > SerialisedItems;
    Owned<std::vector<CCharString> > SerialisedSelectedItems;
    int requestedQuest;
    CTCInventoryBase() : requestedQuest(-1) {}
    const CWorld& PeekWorld() const { return world; }
    const CGameDefinitionManager& GetDefinitionManager() const { return definitions; }
    void GetAllNonEmptyInventoryItems(std::vector<CItem>& items, bool quest) {
        requestedQuest = quest ? 1 : 0;
        items.push_back(CItem(11));
        if (quest) items.push_back(CItem(12));
    }
    void GetAllSelectedItems(std::vector<long>&) const;
    void OnSerialise(CPersistContext&, bool);
};
}
namespace NInventory {

void CTCInventoryBase::GetAllSelectedItems(std::vector<long>& items) const
{
    items.clear();
    for (unsigned long i = 0; i < Categories.size(); ++i)
        items.push_back(Categories[i].GetHighlightedItem());
}

void CTCInventoryBase::OnSerialise(CPersistContext& context, bool in_game)
{
    if (context.IsSaving()) {
        std::vector<CItem> items;
        GetAllNonEmptyInventoryItems(items, PeekWorld().IsSerialisingNonPersistentQuestItems());
        context.TransferVector("InventoryItems", items);
        std::vector<long> selected_items;
        GetAllSelectedItems(selected_items);
        std::vector<CCharString> selected_item_def_names;
        selected_item_def_names.reserve(selected_items.size());
        for (long c1 = 0; c1 < selected_items.size(); ++c1)
            if (selected_items[c1] > 0)
                selected_item_def_names.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(selected_items[c1]));
        context.TransferVector("SelectedInventoryItems", selected_item_def_names);
        context.TransferVector("ConfiscatedItems2", ConfiscatedItems);
    } else {
        SerialisedItems.Reset(new std::vector<CItem>);
        context.TransferVector("InventoryItems", *SerialisedItems);
        SerialisedSelectedItems.Reset(new std::vector<CCharString>);
        context.TransferVector("SelectedInventoryItems", *SerialisedSelectedItems);
        ConfiscatedItems.clear();
        context.TransferVector("ConfiscatedItems2", ConfiscatedItems);
    }
}
}

int main() {
    using namespace NInventory;
    const long selected[] = { -1, 4, 0, 9, 4 };
    for (int quest=0;quest<2;++quest) for (int populated=0;populated<2;++populated)
    for (int inGame=0;inGame<2;++inGame) {
        CTCInventoryBase inv;
        inv.world.quest=quest!=0;
        if (populated) {
            for(unsigned i=0;i<5;++i) { CCategory c={selected[i]}; inv.Categories.push_back(c); }
            inv.ConfiscatedItems.push_back(CItem(25));
        }
        CPersistContext ctx(true);
        inv.OnSerialise(ctx,inGame!=0);
        if(!ctx.valid || ctx.step!=3 || inv.requestedQuest!=quest ||
           ctx.inventory.size()!=unsigned(1+quest) || ctx.inventory[0].id!=11 ||
           (quest && ctx.inventory[1].id!=12)) return 1;
        if(ctx.names.size()!=unsigned(populated?3:0) || inv.definitions.queries.size()!=ctx.names.size() ||
           ctx.seized.size()!=unsigned(populated) || (populated && ctx.seized[0].id!=25)) return 2;
        for(unsigned j=0;j<ctx.names.size();++j) {
            long expected=j==1?9:4;
            if(inv.definitions.queries[j]!=expected || ctx.names[j].id!=1000+expected) return 3;
        }
        if(inv.SerialisedItems.resets || inv.SerialisedSelectedItems.resets) return 4;
        inv.ConfiscatedItems.push_back(CItem(99));
        ctx.saving=false; ctx.step=0; inv.requestedQuest=-1;
        for(int round=1;round<=2;++round) {
            if(round==2) {ctx.inventory.clear();ctx.names.clear();ctx.seized.clear();ctx.step=0;}
            inv.OnSerialise(ctx,inGame!=0);
            if(!ctx.valid || ctx.step!=3 || inv.requestedQuest!=-1 ||
               inv.SerialisedItems.resets!=unsigned(round) || inv.SerialisedSelectedItems.resets!=unsigned(round) ||
               (*inv.SerialisedItems).size()!=ctx.inventory.size() ||
               (*inv.SerialisedSelectedItems).size()!=ctx.names.size() ||
               inv.ConfiscatedItems.size()!=ctx.seized.size()) return 5;
            for(unsigned k=0;k<ctx.inventory.size();++k) if((*inv.SerialisedItems)[k].id!=ctx.inventory[k].id) return 6;
            for(unsigned n=0;n<ctx.names.size();++n) if((*inv.SerialisedSelectedItems)[n].id!=ctx.names[n].id) return 7;
            if(!ctx.seized.empty() && inv.ConfiscatedItems[0].id!=25) return 8;
        }
    }
    puts("005bfd5e_TEST PASS");
    return 0;
}
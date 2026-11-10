// Behaviour test of CTCInventoryWeapons::FinalConstructWeaponItemEntriesPC (005c47d5): as the console builder, plus: for every item the
// shared wield entry is rebuilt for the item's kind (melee / ranged) from the parent's PC_UI_FRAME, and that frame is appended as a
// parameter to the add-mesh (2000: def, entry, L"PC_MODEL", frame), load-description (2001: def, viewport, entry, frame) and load-name
// (2004: def, entry, title, augmentations, weapons-usable, wield entry, frame) actions.
#include <cstdio>
#include <string>
#include <utility>
#include <vector>
struct CAction { long ActionType; std::vector<std::string> Params; };
struct CEntry { long Def; std::vector<CAction> OnSelected, OnUnselected, OnVisible; std::vector<long> ThingParams; };
struct CList { std::vector<CEntry*> Scrolling; long Default; CList() : Default(-1) { } };
static std::string L2S(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static bool IsMelee(long def) { return def < 10; }
static std::string ConstructWieldEntryPC(bool melee, const std::string& frame, long def)
{
    return std::string(melee ? "melee" : "ranged") + "@" + frame + ":" + L2S(def);
}
static void FinalConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >& items, CList* list, const std::string& parentFrame,
    long wieldedMelee, std::string& wieldEntry, std::vector<std::string>& wieldHistory, unsigned long offset)
{
    std::string frame = parentFrame;
    for (unsigned long i = 0; i < items.size(); ++i) {
        long def = items[i].first;
        if (IsMelee(def))
            wieldEntry = ConstructWieldEntryPC(true, frame, def);
        else
            wieldEntry = ConstructWieldEntryPC(false, frame, def);
        wieldHistory.push_back(wieldEntry);
        CEntry* entry = new CEntry; entry->Def = def;
        if (def == wieldedMelee)
            list->Default = (long)(offset + i);
        std::string uiFrame = parentFrame;
        CAction a;
        a.ActionType = 2000; a.Params.clear(); a.Params.push_back(L2S(def)); a.Params.push_back("entry"); a.Params.push_back("PC_MODEL");
        a.Params.push_back(uiFrame); entry->OnSelected.push_back(a);
        a.ActionType = 2015; a.Params.clear(); a.Params.push_back("entry"); entry->OnUnselected.push_back(a);
        a.ActionType = 2001; a.Params.clear(); a.Params.push_back(L2S(def)); a.Params.push_back("viewport"); a.Params.push_back("entry");
        a.Params.push_back(uiFrame); entry->OnSelected.push_back(a);
        a.ActionType = 2016; a.Params.clear(); a.Params.push_back("entry"); entry->OnUnselected.push_back(a);
        a.ActionType = 2004; a.Params.clear(); a.Params.push_back(L2S(def)); a.Params.push_back("entry"); a.Params.push_back("title");
        a.Params.push_back("augmentations"); a.Params.push_back("1"); a.Params.push_back(wieldEntry); a.Params.push_back(uiFrame);
        entry->OnVisible.push_back(a);
        list->Scrolling.push_back(entry);
        entry->ThingParams.push_back(def);
    }
}
int main()
{
    std::vector<std::pair<long, long> > items;
    items.push_back(std::make_pair(3L, 1L)); items.push_back(std::make_pair(12L, 1L));
    CList list; std::string wield; std::vector<std::string> history;
    FinalConstructWeaponItemEntriesPC(items, &list, "PC_UI_FRAME", 3, wield, history, 2);
    int bad = 0;
    bad |= history.size() != 2 || history[0] != "melee@PC_UI_FRAME:3" || history[1] != "ranged@PC_UI_FRAME:12";
    bad |= wield != "ranged@PC_UI_FRAME:12" || list.Default != 2 || list.Scrolling.size() != 2;
    for (unsigned i = 0; i < 2; ++i) {
        CEntry* e = list.Scrolling[i];
        bad |= e->OnSelected[0].Params.size() != 4 || e->OnSelected[0].Params[3] != "PC_UI_FRAME";
        bad |= e->OnSelected[1].Params.size() != 4 || e->OnVisible[0].Params.size() != 7;
        bad |= e->OnVisible[0].Params[5] != history[i];          // the name action carries the wield entry built for this item
        bad |= e->OnUnselected.size() != 2 || e->ThingParams[0] != items[i].first;
    }
    if (bad) { std::printf("005c47d5_TEST FAIL\n"); return 1; }
    std::printf("005c47d5_TEST PASS\n");
    return 0;
}
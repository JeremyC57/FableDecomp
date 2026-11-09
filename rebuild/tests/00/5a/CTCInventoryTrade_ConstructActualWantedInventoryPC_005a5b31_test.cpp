// Behaviour test for CTCInventoryTrade::ConstructActualWantedInventoryPC @ 0x005A5B31: the PC "wanted" trade screen. Models what the
// function decides: one sub list per menu branch sized by the number of wanted items in it, the per-branch wanted count appended to
// the branch label (" x<n>" only when the count is positive), the clothing / weapons / items totals (each wanted item counts once,
// by its menu type) appended to the three category headers, the "wanted" total (items + clothes + weapons), the subtitle text and
// static name picked by the entry's name (clothing / weapons / items entries, anything else gets an empty subtitle and no name),
// and the closing refresh that runs only for an ordinary shop (not a title, tattoo or barber shop).
#include <cstdio>
#include <map>
#include <string>
#include <vector>

enum EItem_Menu_Type { ITEM_MENU_TYPE_CLOTHES, ITEM_MENU_TYPE_WEAPONS, ITEM_MENU_TYPE_ITEMS, ITEM_MENU_TYPE_NONE };
struct ItemInfo { int branch; EItem_Menu_Type type; };                      // GetMenuBranchFromDefIndexPC's two results

static std::string IntToString(long value)                                  // GFIntToWideString
{
    char buf[32];
    std::sprintf(buf, "%ld", value);
    return buf;
}

struct WantedScreen {
    std::map<int, long> subListSize;                                        // InitialiseOffsets(count) per branch
    std::map<int, long> branchWanted;                                       // NumWantedThings[branch].second
    long clothes, weapons, items;                                           // TotNumClothesWanted / Weapons / Items
};

static WantedScreen Build(const std::vector<long>& wanted, const std::map<long, ItemInfo>& defs)
{
    WantedScreen screen;
    screen.clothes = screen.weapons = screen.items = 0;
    if (wanted.begin() != wanted.end()) {
        for (std::vector<long>::const_iterator it = wanted.begin(); it != wanted.end(); ++it) {
            int branch = defs.find(*it)->second.branch;
            std::map<int, long>::iterator found = screen.subListSize.find(branch);
            if (found == screen.subListSize.end()) screen.subListSize[branch] = 1; else ++found->second;
        }
        for (std::vector<long>::const_iterator it = wanted.begin(); it != wanted.end(); ++it) {
            const ItemInfo& info = defs.find(*it)->second;
            if (screen.branchWanted.find(info.branch) == screen.branchWanted.end()) screen.branchWanted[info.branch] = 1;
            else ++screen.branchWanted[info.branch];
            switch (info.type) {
            case ITEM_MENU_TYPE_CLOTHES: ++screen.clothes; break;
            case ITEM_MENU_TYPE_WEAPONS: ++screen.weapons; break;
            case ITEM_MENU_TYPE_ITEMS:   ++screen.items;   break;
            default: break;
            }
        }
    }
    return screen;
}

static std::string BranchLabel(const std::string& label, long count, const std::string& x)
{
    std::string text = label;
    if (count > 0) text += x + IntToString(count);
    return text;
}

static std::string Subtitle(const std::string& entryName, const WantedScreen& s, const std::string& x, std::string* staticName)
{
    *staticName = "";
    if (entryName == "PC_TRADING_MENU_CLOTHING_ENTRY") { *staticName = "Clothes_Subtitle"; return "TEXT_GUI_MENU_CLOTHING" + x + IntToString(s.clothes); }
    if (entryName == "PC_TRADING_MENU_WEAPONS_ENTRY") { *staticName = "Weapons_Subtitle"; return "TEXT_GUI_MENU_WEAPONS" + x + IntToString(s.weapons); }
    if (entryName == "PC_TRADING_MENU_ITEMS_ENTRY") { *staticName = "Items_Subtitle"; return "TEXT_GUI_MENU_ITEMS" + x + IntToString(s.items); }
    return "";
}

static bool Refreshes(bool titleShop, bool tattooShop, bool barberShop) { return !titleShop && !tattooShop && !barberShop; }

int main()
{
    int ok = 1;
    std::map<long, ItemInfo> defs;
    ItemInfo shirt = { 10, ITEM_MENU_TYPE_CLOTHES }, sword = { 20, ITEM_MENU_TYPE_WEAPONS }, potion = { 30, ITEM_MENU_TYPE_ITEMS };
    ItemInfo hat = { 11, ITEM_MENU_TYPE_CLOTHES };
    defs[1] = shirt; defs[2] = sword; defs[3] = potion; defs[4] = hat;
    std::vector<long> wanted;
    wanted.push_back(1); wanted.push_back(2); wanted.push_back(1); wanted.push_back(3); wanted.push_back(4);

    WantedScreen s = Build(wanted, defs);
    if (s.subListSize.size() != 4 || s.subListSize[10] != 2 || s.subListSize[20] != 1 || s.subListSize[30] != 1 || s.subListSize[11] != 1) ok = 0;
    if (s.branchWanted[10] != 2 || s.branchWanted[11] != 1) ok = 0;
    if (s.clothes != 3 || s.weapons != 1 || s.items != 1) ok = 0;
    if (s.items + s.clothes + s.weapons != 5) ok = 0;                       // "TEXT_GUI_MENU_WANTED x5"

    if (BranchLabel("Shirts", 2, " x") != "Shirts x2") ok = 0;
    if (BranchLabel("Shirts", 0, " x") != "Shirts") ok = 0;

    std::string name;
    if (Subtitle("PC_TRADING_MENU_CLOTHING_ENTRY", s, " x", &name) != "TEXT_GUI_MENU_CLOTHING x3" || name != "Clothes_Subtitle") ok = 0;
    if (Subtitle("PC_TRADING_MENU_WEAPONS_ENTRY", s, " x", &name) != "TEXT_GUI_MENU_WEAPONS x1" || name != "Weapons_Subtitle") ok = 0;
    if (Subtitle("PC_TRADING_MENU_ITEMS_ENTRY", s, " x", &name) != "TEXT_GUI_MENU_ITEMS x1" || name != "Items_Subtitle") ok = 0;
    if (Subtitle("PC_TRADING_MENU_OTHER_ENTRY", s, " x", &name) != "" || name != "") ok = 0;

    std::vector<long> none;
    WantedScreen e = Build(none, defs);                                     // empty list: no sub lists, all totals zero
    if (!e.subListSize.empty() || e.clothes != 0 || e.weapons != 0 || e.items != 0) ok = 0;

    if (!Refreshes(false, false, false)) ok = 0;
    if (Refreshes(true, false, false) || Refreshes(false, true, false) || Refreshes(false, false, true)) ok = 0;

    if (!ok) { std::printf("005a5b31_TEST FAIL\n"); return 1; }
    std::printf("005a5b31_TEST PASS\n");
    return 0;
}
// Behaviour test for CThingPlayerCreature::PreOnKill (0x006AB300): non-main players may die; the main player uses a live
// resurrection item, otherwise the completion UI opens in mode 3; the kill is vetoed in both cases.
#include <cstdio>

struct CThing;
static int g_used, g_activated, g_mode;
struct CTCResurrectionItem { void OnItemUsed(CThing&) { ++g_used; } };
struct CTCQuestCompletionUI { void Activate(int type, const char*, bool, const char*, bool) { ++g_activated; g_mode = type; } };
struct CTCInventoryBase { CThing* Item; CThing* GetResurrectionItemInInventory() { return Item; } };
struct CThing {
    bool Main; bool Dead;
    CTCInventoryBase* Inventory; CTCResurrectionItem* Resurrection; CTCQuestCompletionUI* UI;
    bool HasTC(long t) const { return t == 0x11 ? Inventory != 0 : t == 0xe1 ? Resurrection != 0 : UI != 0; }
    void* GetTC(long t) { return t == 0x11 ? (void*)Inventory : t == 0xe1 ? (void*)Resurrection : (void*)UI; }
    bool PreOnKill()
    {
        if (Main) {
            if (HasTC(0x11)) {
                CThing* item = ((CTCInventoryBase*)GetTC(0x11))->GetResurrectionItemInInventory();
                if (item && !item->Dead) {
                    CTCResurrectionItem* resurrection = 0;
                    if (item->HasTC(0xe1))
                        resurrection = (CTCResurrectionItem*)item->GetTC(0xe1);
                    resurrection->OnItemUsed(*this);
                    return false;
                }
            }
            CTCQuestCompletionUI* ui = 0;
            if (HasTC(0xfc))
                ui = (CTCQuestCompletionUI*)GetTC(0xfc);
            ui->Activate(3, "", true, "", true);
            return false;
        }
        return true;
    }
};

int main()
{
    CTCResurrectionItem res; CTCQuestCompletionUI ui;
    CThing item = { false, false, 0, &res, 0 };
    CTCInventoryBase inv = { &item };
    CThing hero = { true, false, &inv, 0, &ui };
    CThing npc = { false, false, 0, 0, 0 };
    bool ok = npc.PreOnKill() && g_used == 0 && g_activated == 0;
    ok = ok && !hero.PreOnKill() && g_used == 1 && g_activated == 0;
    item.Dead = true;
    ok = ok && !hero.PreOnKill() && g_used == 1 && g_activated == 1 && g_mode == 3;
    if (!ok) { std::printf("006ab300_TEST FAIL\n"); return 1; }
    std::printf("006ab300_TEST PASS\n");
    return 0;
}
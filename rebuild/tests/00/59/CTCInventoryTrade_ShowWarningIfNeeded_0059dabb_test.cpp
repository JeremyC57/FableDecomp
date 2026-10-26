// Behaviour test for CTCInventoryTrade::ShowWarningIfNeeded @ 0x0059DABB (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
static long g_usedTrophy = 0; static bool g_usedFlag = false; static int g_subtitles = 0; static const char* g_lastText = 0;
static unsigned long g_lastColour = 0; static float g_lastTime = 1.0f;
class CTCInventory { public: void UseTrophy(long trophy, bool* fromShop) { g_usedTrophy = trophy; g_usedFlag = *fromShop; *fromShop = false; } };
class CThing { public: CTCInventory* inventory; bool GetInterface(CTCInventory** out) { if (inventory) { *out = inventory; return true; } return false; } };
struct Model {
    CThing* ParentThing; bool WarningNeeded; long TrophyUseNeeded;
    void ShowWarningIfNeeded()
    {
        if (TrophyUseNeeded != -1) {
            bool fromShop = true;
            CTCInventory* inventory;
            if (ParentThing->GetInterface(&inventory)) inventory->UseTrophy(TrophyUseNeeded, &fromShop);
            TrophyUseNeeded = -1;
            WarningNeeded = false;
        } else if (WarningNeeded) {
            WarningNeeded = false;
            ++g_subtitles; g_lastText = "TEXT_GUI_SHOP_WEAPON_HELD_WARNING"; g_lastColour = 0x122; g_lastTime = 0.0f;
        }
    }
};
int main()
{
    bool ok = true;
    CTCInventory inv; CThing thing = { &inv }; CThing bare = { 0 };
    Model m = { &thing, true, 42 };
    m.ShowWarningIfNeeded();            // trophy use wins over the warning and clears both
    ok = ok && g_usedTrophy == 42 && g_usedFlag && m.TrophyUseNeeded == -1 && !m.WarningNeeded && g_subtitles == 0;
    m.WarningNeeded = true; m.ShowWarningIfNeeded();
    ok = ok && g_subtitles == 1 && !m.WarningNeeded && g_lastColour == 0x122 && g_lastTime == 0.0f && strcmp(g_lastText, "TEXT_GUI_SHOP_WEAPON_HELD_WARNING") == 0;
    m.ShowWarningIfNeeded();            // nothing pending
    ok = ok && g_subtitles == 1;
    Model n = { &bare, false, 7 }; g_usedTrophy = 0; n.ShowWarningIfNeeded();   // no inventory component: the trophy is dropped
    ok = ok && g_usedTrophy == 0 && n.TrophyUseNeeded == -1;
    if (!ok) { printf("0059dabb_TEST FAIL\n"); return 1; }
    printf("0059dabb_TEST PASS\n"); return 0;
}
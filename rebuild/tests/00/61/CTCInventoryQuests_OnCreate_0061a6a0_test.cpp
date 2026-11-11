// Behaviour test for 0x0061A6A0 CTCInventoryQuests::OnCreate: the screen registers itself, takes the creature def's quests-screen
// inventory def index, runs the base OnCreate, sizes the primitive handles to 4 and the quest-card items to 6, resets the item
// selection (count 6, index 0), fills items 0..5 with their index and header text, and clears QuestCardCount; the creature def is
// add-ref'd by GetSubDef and released at the end, the thing's base def is add-ref'd and released around GetSubDef.
#include <cstdio>
#include <cstring>
#include <vector>

struct CDef { long RefCount; long QuestsScreenInventory; };
static CDef g_baseDef = { 1, 0 };
static CDef g_creatureDef = { 1, 77 };
static int g_registered, g_baseOnCreate;
static long g_setPDef = -1;
static long g_testQuestIndex = 5, g_testQuestCount = 0;

class CQuestCardItem { public: long ItemIndex; const char* ItemText; };
class CTCInventoryQuests { public:
    std::vector<int> InventoryQuestCardSelectPrimitiveHandles;
    std::vector<CQuestCardItem> QuestCardItemList;
    long QuestCardCount;
    bool GetSubDef(CDef* base, CDef** out) { ++base->RefCount; ++g_creatureDef.RefCount; *out = &g_creatureDef; --base->RefCount; return true; }
    void OnCreate()
    {
        ++g_registered;
        CDef* creature_def = 0;
        GetSubDef(&g_baseDef, &creature_def);
        g_setPDef = creature_def->QuestsScreenInventory;
        ++g_baseOnCreate;
        InventoryQuestCardSelectPrimitiveHandles.resize(4);
        QuestCardItemList.resize(6);
        g_testQuestCount = 6;
        g_testQuestIndex = 0;
        CQuestCardItem item;
        item.ItemText = "";
        long i = 0;
        do {
            item.ItemIndex = i;
            switch (i) {
            case 0: item.ItemText = "Description"; break;
            case 1: item.ItemText = "Picture"; break;
            case 2: item.ItemText = "Trophy"; break;
            case 3: item.ItemText = "Location"; break;
            case 4: item.ItemText = "Rewards"; break;
            case 5: item.ItemText = "Prerequisites"; break;
            }
            QuestCardItemList[i] = item;
        } while (++i < 6);
        QuestCardCount = 0;
        --creature_def->RefCount;
    }
};

#define FAIL { std::printf("0061a6a0_TEST FAIL\n"); return 1; }
int main()
{
    CTCInventoryQuests quests;
    quests.QuestCardCount = 9;
    quests.OnCreate();
    if (g_registered != 1 || g_baseOnCreate != 1 || g_setPDef != 77) FAIL;
    if (quests.InventoryQuestCardSelectPrimitiveHandles.size() != 4 || quests.QuestCardItemList.size() != 6) FAIL;
    if (g_testQuestCount != 6 || g_testQuestIndex != 0 || quests.QuestCardCount != 0) FAIL;
    static const char* names[6] = { "Description", "Picture", "Trophy", "Location", "Rewards", "Prerequisites" };
    for (int i = 0; i < 6; ++i)
        if (quests.QuestCardItemList[i].ItemIndex != i || std::strcmp(quests.QuestCardItemList[i].ItemText, names[i]) != 0) FAIL;
    if (g_baseDef.RefCount != 1 || g_creatureDef.RefCount != 1) FAIL;
    std::printf("0061a6a0_TEST PASS\n");
    return 0;
}
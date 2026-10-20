// Behaviour test for CTCInventoryQuests::vfunc_70 (0x006223C0).
#include <cstdio>

struct CTCQuestCard { long FeatIndex; };
struct CTCHeroCentre { int Removed; void RemoveQuestCard(void*) { ++Removed; } };
struct CThing { CTCQuestCard* QuestCard; CTCHeroCentre* HeroCentre; };
struct CQuestManager {
    bool FeatOk, QuestOk; int Feats, Quests;
    bool ActivateFeatCard(CThing*) { ++Feats; return FeatOk; }
    bool ActivateQuestCard(CThing*, bool) { ++Quests; return QuestOk; }
};
static CQuestManager g_qm;
struct CTCInventoryQuests {
    bool Enabled; long QuestCardCount; CThing* Selected; CThing* Trader; int Rebuilds;
    bool CanTakeSelectedItem() { return Enabled; }
    bool vfunc_70()
    {
        if (CanTakeSelectedItem() && QuestCardCount > 0) {
            CThing* card = Selected;
            if (card) {
                CTCQuestCard* questCard = card->QuestCard;
                if (questCard) {
                    if (questCard->FeatIndex > -1) {
                        if (!g_qm.ActivateFeatCard(card))
                            return false;
                    } else if (!g_qm.ActivateQuestCard(card, false)) {
                        return false;
                    }
                    CThing* trader = Trader;
                    if (trader)
                        trader->HeroCentre->RemoveQuestCard(card);
                    ++Rebuilds;
                    return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    CTCQuestCard quest = { -1 }, feat = { 3 };
    CTCHeroCentre centre = { 0 };
    CThing card = { &quest, 0 }, trader = { 0, &centre };
    CTCInventoryQuests q = { true, 2, &card, &trader, 0 };
    g_qm.FeatOk = true; g_qm.QuestOk = true; g_qm.Feats = 0; g_qm.Quests = 0;
    bool ok = q.vfunc_70() && g_qm.Quests == 1 && centre.Removed == 1 && q.Rebuilds == 1;
    card.QuestCard = &feat;
    ok = ok && q.vfunc_70() && g_qm.Feats == 1;
    g_qm.FeatOk = false;
    ok = ok && !q.vfunc_70() && q.Rebuilds == 2;
    q.QuestCardCount = 0;
    ok = ok && !q.vfunc_70() && g_qm.Feats == 2;
    q.QuestCardCount = 1; q.Trader = 0; g_qm.FeatOk = true;
    ok = ok && q.vfunc_70() && centre.Removed == 2;
    if (!ok) { std::printf("006223c0_TEST FAIL\n"); return 1; }
    std::printf("006223c0_TEST PASS\n");
    return 0;
}

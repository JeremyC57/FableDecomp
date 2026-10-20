// Behaviour test for CQuestManager::PrepareQuestsWhenFinalQuestIsActivated (0x004B0F10): every active card's quest
// that is neither a vignette nor the final quest is queued with reason 2 (in list order); things without a quest
// card are skipped; the world's map-table flag is cleared even when the list is empty.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    bool operator!=(const CCharString& o) const { return std::strcmp(Text, o.Text) != 0; }
};
struct CTCQuestCard {
    CCharString ScriptName; bool Vignette;
    CTCQuestCard(const char* n, bool v) : ScriptName(n), Vignette(v) {}
    bool IsVignette() const { return Vignette; }
};
struct CThing {
    CTCQuestCard* Card;
    bool HasTC(long type) const { return type == 0x6c && Card != 0; }
    CTCQuestCard* GetTC(long) const { return Card; }
};
static CTCQuestCard* GetQuestCard(CThing* thing) { CTCQuestCard* tc = 0; if (thing->HasTC(0x6c)) tc = thing->GetTC(0x6c); return tc; }
struct CBaseIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CCardNode { CCardNode* _M_next; CCardNode* _M_prev; CBaseIntelligentPointer _M_data; };
struct CWorld { bool MapTableShowQuestCardsOnUsed; };
static char g_log[256];
struct CQuestManager {
    CCardNode* ActiveQuestCards;
    CWorld*    World;
    void AddPendingQuestToDeactivate(const CCharString& name, unsigned long reason)
    { char b[64]; std::sprintf(b, "%s:%lu;", name.Text, reason); std::strcat(g_log, b); }
    void PrepareQuestsWhenFinalQuestIsActivated(const CCharString& finalQuestName);
};
void CQuestManager::PrepareQuestsWhenFinalQuestIsActivated(const CCharString& finalQuestName)
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        CTCQuestCard* tc = GetQuestCard(card);
        if (tc && !tc->IsVignette() && tc->ScriptName != finalQuestName)
            AddPendingQuestToDeactivate(tc->ScriptName, 2);
    }
    World->MapTableShowQuestCardsOnUsed = false;
}
int main()
{
    CTCQuestCard a("QA", false), v("QV", true), f("QFinal", false), b("QB", false);
    CThing ta = { &a }, tn = { 0 }, tv = { &v }, tf = { &f }, tb = { &b };
    CThing* things[5] = { &ta, &tn, &tv, &tf, &tb };
    CCardNode head, n[5];
    CCardNode* prev = &head;
    for (int i = 0; i < 5; ++i) { n[i]._M_data.Item = things[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = &head; head._M_prev = prev;
    CWorld world = { true };
    CQuestManager qm; qm.ActiveQuestCards = &head; qm.World = &world;
    char fin[7] = "QFinal";
    qm.PrepareQuestsWhenFinalQuestIsActivated(CCharString(fin));
    int bad = 0;
    if (std::strcmp(g_log, "QA:2;QB:2;") != 0 || world.MapTableShowQuestCardsOnUsed) bad |= 1;
    CCardNode empty; empty._M_next = empty._M_prev = &empty;
    g_log[0] = 0; world.MapTableShowQuestCardsOnUsed = true; qm.ActiveQuestCards = &empty;
    qm.PrepareQuestsWhenFinalQuestIsActivated(CCharString("QA"));
    if (g_log[0] || world.MapTableShowQuestCardsOnUsed) bad |= 2;
    if (bad) { std::printf("004b0f10_TEST FAIL\n"); return 1; }
    std::printf("004b0f10_TEST PASS\n");
    return 0;
}

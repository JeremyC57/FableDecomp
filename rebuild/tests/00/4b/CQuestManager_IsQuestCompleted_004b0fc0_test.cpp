// Behaviour test for CQuestManager::IsQuestCompleted (0x004B0FC0): true only when a LIVING thing on the completed
// list carries a quest card of the named script; dead things, null slots and things without the TC are skipped.
#include <cstdio>
#include <cstring>
// Test doubles for the CQuestManager quest-card lists: a CCharString is its text (compared like the inlined
// operator==), a thing holds an optional quest-card TC and a dead bit, the list is the STLport sentinel ring,
// GetPItem returns the stored thing, Kill/Refresh/free are recording spies (free records only while armed and
// never frees: the nodes live in static storage).
struct CCharString {
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    bool operator==(const CCharString& o) const { return Text == o.Text || std::strcmp(Text, o.Text) == 0; }
};
struct CTCQuestCard { CCharString ScriptName; CTCQuestCard(const char* n) : ScriptName(n) {} };
static int g_kills, g_refreshes, g_frees, g_freeArmed;
struct CThing {
    CTCQuestCard* Card; bool Dead; int Killed;
    CThing(CTCQuestCard* c, bool dead) : Card(c), Dead(dead), Killed(0) {}
    bool HasTC(long type) const { return type == 0x6c && Card != 0; }
    CTCQuestCard* GetTC(long) const { return Card; }
    void Kill(bool immediately) { if (!immediately) { ++Killed; ++g_kills; } }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
struct CCardNode { CCardNode* _M_next; CCardNode* _M_prev; CBaseIntelligentPointer _M_data; };
static void free_spy(void*) { if (g_freeArmed) ++g_frees; }
static void EraseNode(CCardNode* node)
{
    CCardNode* next = node->_M_next;
    CCardNode* prev = node->_M_prev;
    prev->_M_next = next;
    next->_M_prev = prev;
    free_spy(node);
}
struct CPlayerGui { void Refresh(long flag) { if (flag == 0) ++g_refreshes; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
static void Ring(CCardNode* head, CCardNode* nodes, CThing** things, int n)
{
    CCardNode* prev = head;
    for (int i = 0; i < n; ++i) { nodes[i]._M_data.Item = things[i]; prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = head; head->_M_prev = prev;
}
static int Count(CCardNode* head) { int n = 0; for (CCardNode* p = head->_M_next; p != head; p = p->_M_next) ++n; return n; }
struct CQuestManager {
    CCardNode* ActiveQuestCards;
    CCardNode* CompletedQuestCards;
    bool IsCompletedQuestCard(CThing* card) const
    {
        for (CCardNode* node = CompletedQuestCards->_M_next; node != CompletedQuestCards; node = node->_M_next)
            if (node->_M_data.GetPItem() == card)
                return true;
        return false;
    }
    void RemoveActiveQuestCard(const CCharString& scriptName);
    CThing* GetActiveQuestCardFromScriptName(const CCharString& scriptName) const;
    bool IsQuestCompleted(const CCharString& scriptName) const;
};
bool CQuestManager::IsQuestCompleted(const CCharString& scriptName) const
{
    for (CCardNode* node = CompletedQuestCards->_M_next; node != CompletedQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card && !card->Dead) {
            CTCQuestCard* tc = 0;
            if (card->HasTC(0x6c))
                tc = (CTCQuestCard*)card->GetTC(0x6c);
            if (tc && tc->ScriptName == scriptName)
                return true;
        }
    }
    return false;
}
int main()
{
    CTCQuestCard q1("Q1"), q2("Q2"), q3("Q3");
    CThing a(&q1, true), b(0, false), c(&q2, false), d(&q3, false);
    CThing* done[5] = { &a, 0, &b, &c, &d };
    static CCardNode ah, ch, cn[5];
    Ring(&ch, cn, done, 5); Ring(&ah, cn, done, 0);
    CQuestManager qm; qm.ActiveQuestCards = &ah; qm.CompletedQuestCards = &ch;
    char n2[3] = "Q2";
    int bad = 0;
    if (!qm.IsQuestCompleted(CCharString(n2))) bad |= 1;
    if (!qm.IsQuestCompleted(CCharString("Q3"))) bad |= 2;
    if (qm.IsQuestCompleted(CCharString("Q1"))) bad |= 4;     // its thing is dead
    if (qm.IsQuestCompleted(CCharString("Q9"))) bad |= 8;
    if (bad) { std::printf("004b0fc0_TEST FAIL\n"); return 1; }
    std::printf("004b0fc0_TEST PASS\n");
    return 0;
}
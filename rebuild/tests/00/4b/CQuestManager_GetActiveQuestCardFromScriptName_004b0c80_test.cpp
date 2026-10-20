// Behaviour test for CQuestManager::GetActiveQuestCardFromScriptName (0x004B0C80): the first active quest-card
// thing whose card names the script, null when none does (things without the TC are skipped).
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
// the retail helper reads an unset local when the TC is absent; the test thing's GetTC returns null instead
static CTCQuestCard* GetQuestCard(CThing* thing) { CTCQuestCard* tc = 0; if (thing->HasTC(0x6c)) tc = thing->GetTC(0x6c); return tc; }
CThing* CQuestManager::GetActiveQuestCardFromScriptName(const CCharString& scriptName) const
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        CTCQuestCard* tc = GetQuestCard(card);
        if (tc && tc->ScriptName == scriptName)
            return card;
    }
    return 0;
}
int main()
{
    CTCQuestCard q1("Q1"), q2a("Q2"), q2b("Q2");
    CThing a(&q1, false), b(0, false), c(&q2a, false), d(&q2b, false);
    CThing* act[4] = { &a, &b, &c, &d };
    static CCardNode ah, an[4], ch;
    Ring(&ah, an, act, 4); Ring(&ch, an, act, 0);
    CQuestManager qm; qm.ActiveQuestCards = &ah; qm.CompletedQuestCards = &ch;
    char n2[3] = "Q2";
    int bad = 0;
    if (qm.GetActiveQuestCardFromScriptName(CCharString(n2)) != &c) bad |= 1;
    if (qm.GetActiveQuestCardFromScriptName(CCharString("Q1")) != &a) bad |= 2;
    if (qm.GetActiveQuestCardFromScriptName(CCharString("Q9")) != 0) bad |= 4;
    CQuestManager empty; empty.ActiveQuestCards = &ch; empty.CompletedQuestCards = &ch;
    if (empty.GetActiveQuestCardFromScriptName(CCharString("Q1")) != 0) bad |= 8;
    if (bad) { std::printf("004b0c80_TEST FAIL\n"); return 1; }
    std::printf("004b0c80_TEST PASS\n");
    return 0;
}
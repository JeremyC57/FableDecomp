// Behaviour test for CQuestManager::RemoveActiveQuestCard (0x004B0AA0): the first active card of the named script
// is unlinked and freed, killed only when it is not also on the completed list, and the player GUI is refreshed once;
// an unknown script name changes nothing.
#include <cstdio>
#include <cstring>
// Test doubles for the CQuestManager quest-card lists: a CCharString is its text (compared like the inlined
// operator==), a thing holds an optional quest-card TC and a dead bit, the list is the STLport sentinel ring,
// GetPItem returns the stored thing, Kill/Refresh/free are recording spies (free records only while armed and
// never frees: the nodes live in static storage).
class CCharString { public:
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    bool operator==(const CCharString& o) const { return Text == o.Text || std::strcmp(Text, o.Text) == 0; }
};
class CTCQuestCard { public: CCharString ScriptName; CTCQuestCard(const char* n) : ScriptName(n) {} };
static int g_kills, g_refreshes, g_frees, g_freeArmed;
class CThing { public:
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
class CPlayerGui { public: void Refresh(long flag) { if (flag == 0) ++g_refreshes; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
static void Ring(CCardNode* head, CCardNode* nodes, CThing** things, int n)
{
    CCardNode* prev = head;
    for (int i = 0; i < n; ++i) { nodes[i]._M_data.Item = things[i]; prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = head; head->_M_prev = prev;
}
static int Count(CCardNode* head) { int n = 0; for (CCardNode* p = head->_M_next; p != head; p = p->_M_next) ++n; return n; }
class CQuestManager { public:
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
void CQuestManager::RemoveActiveQuestCard(const CCharString& scriptName)
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        CTCQuestCard* tc = 0;
        if (card && card->HasTC(0x6c))
            tc = (CTCQuestCard*)card->GetTC(0x6c);
        if (tc && tc->ScriptName == scriptName) {
            if (!IsCompletedQuestCard(card))
                card->Kill(false);
            EraseNode(node);
            g_pPlayerGui->Refresh(0);
            return;
        }
    }
}
int main()
{
    CTCQuestCard q1("Q1"), q2a("Q2"), q2b("Q2");
    CThing a(&q1, false), b(0, false), c(&q2a, false), d(&q2b, false);
    CThing* act[5] = { &a, &b, 0, &c, &d };
    CThing* done[1] = { &d };
    static CCardNode ah, an[5], ch, cn[1];
    Ring(&ah, an, act, 5); Ring(&ch, cn, done, 1);
    CQuestManager qm; qm.ActiveQuestCards = &ah; qm.CompletedQuestCards = &ch;
    char n2[3] = "Q2";                              // a different buffer: the compare is by text
    int bad = 0;
    g_freeArmed = 1; qm.RemoveActiveQuestCard(CCharString(n2)); g_freeArmed = 0;
    if (Count(&ah) != 4 || ah._M_next->_M_next->_M_next->_M_next != &an[4] || c.Killed != 1 || g_kills != 1 || g_refreshes != 1 || g_frees != 1) bad |= 1;
    g_freeArmed = 1; qm.RemoveActiveQuestCard(CCharString("Q2")); g_freeArmed = 0;
    if (Count(&ah) != 3 || d.Killed != 0 || g_kills != 1 || g_refreshes != 2 || g_frees != 2 || Count(&ch) != 1) bad |= 2;
    g_freeArmed = 1; qm.RemoveActiveQuestCard(CCharString("Q9")); g_freeArmed = 0;
    if (Count(&ah) != 3 || g_refreshes != 2 || g_frees != 2 || a.Killed) bad |= 4;
    if (bad) { std::printf("004b0aa0_TEST FAIL\n"); return 1; }
    std::printf("004b0aa0_TEST PASS\n");
    return 0;
}
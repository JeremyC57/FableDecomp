#include <cstdio>
#include <stddef.h>
#include <string.h>
static int g_stringAssigns = 0, g_literalAssigns = 0;
struct CCharString {
    const char* PStringData;
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++g_stringAssigns; return *this; }
    const CCharString& operator=(const char* text) { PStringData = text; ++g_literalAssigns; return *this; }
};
struct CBaseClass { virtual void v00() { } };
struct CConsoleCommandLine : CBaseClass {
    CCharString CurrentEntry; bool UserEntry; long CursorPosition;
    virtual void v00() { }
    void Reset() { CurrentEntry = ""; UserEntry = false; CursorPosition = 0; }
};
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        iterator() { }
        explicit iterator(node* q) : p(q) { }
        T& operator*() const { return p->value; }
        T* operator->() const { return &p->value; }
        iterator& operator++() { p = p->next; return *this; }
        iterator& operator--() { p = p->prev; return *this; }
        bool operator!=(const iterator& o) const { return p != o.p; }
        bool operator==(const iterator& o) const { return p == o.p; }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
};
typedef CRetailList<CConsoleCommandLine> CHistory;
struct CConsole_Retail {
    virtual void v00() { }
    unsigned char _maps[0x18]; CConsoleCommandLine FrontCommandLine; CConsoleCommandLine BackCommandLine; CCharString HelpBarString; unsigned char _registered[0x0c];
    CHistory CommandLineHistory; CHistory::iterator VisibleHistoryStart; CHistory::iterator VisibleHistoryFinish; long NumberOfHistoryLinesOnscreen; bool HistoryMode; CHistory::iterator HistoryPos; bool MovingIntoHistory; bool GuessMode;
    void ResetCommandLineBuffers() { FrontCommandLine.Reset(); BackCommandLine.Reset(); GuessMode = false; HistoryMode = false; }
    void RevertToUserEntry()
    {
        if (GuessMode) { FrontCommandLine = BackCommandLine; GuessMode = false; }
        else if (HistoryMode) { FrontCommandLine = BackCommandLine; HistoryMode = false; }
        else FrontCommandLine.Reset();
    }
    void PrevInHistory()
    {
        CHistory::iterator start = HistoryPos; bool found = false;
        while (HistoryPos != CommandLineHistory.end() && !found) {
            ++HistoryPos;
            if (HistoryPos != CommandLineHistory.end() && HistoryPos->UserEntry) { FrontCommandLine = *HistoryPos; found = true; }
        }
        if (!found) HistoryPos = start;
    }
    void NextInHistory()
    {
        bool found = false;
        while (HistoryPos != CommandLineHistory.begin() && !found) {
            --HistoryPos;
            if (HistoryPos->UserEntry) { FrontCommandLine = *HistoryPos; found = true; }
        }
        if (!found) RevertToUserEntry();
    }
    void SetHistoryMode()
    {
        for (HistoryPos = CommandLineHistory.begin(); HistoryPos != CommandLineHistory.end(); ++HistoryPos) {
            if (HistoryPos->UserEntry) { FrontCommandLine = *HistoryPos; HistoryMode = true; return; }
        }
    }
};
// a three-entry history (newest first): "new" (user), "msg" (not user), "old" (user)
static CHistory::node g_sentinel, g_n0, g_n1, g_n2;
static void BuildHistory(CConsole_Retail& c)
{
    g_sentinel.next = &g_n0; g_n0.prev = &g_sentinel; g_n0.next = &g_n1; g_n1.prev = &g_n0; g_n1.next = &g_n2; g_n2.prev = &g_n1; g_n2.next = &g_sentinel; g_sentinel.prev = &g_n2;
    g_n0.value.CurrentEntry.PStringData = "new"; g_n0.value.UserEntry = true;  g_n0.value.CursorPosition = 3;
    g_n1.value.CurrentEntry.PStringData = "msg"; g_n1.value.UserEntry = false; g_n1.value.CursorPosition = 0;
    g_n2.value.CurrentEntry.PStringData = "old"; g_n2.value.UserEntry = true;  g_n2.value.CursorPosition = 2;
    memset(&c, 0, sizeof(c)); c.CommandLineHistory.head = &g_sentinel; c.HistoryPos = c.CommandLineHistory.end();
    c.FrontCommandLine.CurrentEntry.PStringData = "front"; c.BackCommandLine.CurrentEntry.PStringData = "back"; c.BackCommandLine.UserEntry = true; c.BackCommandLine.CursorPosition = 4;
}
static bool LayoutOk() { return offsetof(CConsole_Retail, FrontCommandLine) == 0x1c && offsetof(CConsole_Retail, BackCommandLine) == 0x2c && offsetof(CConsole_Retail, CommandLineHistory) == 0x4c
    && offsetof(CConsole_Retail, HistoryMode) == 0x5c && offsetof(CConsole_Retail, HistoryPos) == 0x60 && offsetof(CConsole_Retail, GuessMode) == 0x65 && offsetof(CConsoleCommandLine, UserEntry) == 8 && sizeof(CHistory::node) == 0x18; }

int main()
{
    CConsole_Retail c; BuildHistory(c);
    c.HistoryPos = CHistory::iterator(&g_n0);                      // at "new": previous user entry is "old" (skipping "msg")
    c.PrevInHistory();
    if (!LayoutOk() || c.HistoryPos.p != &g_n2 || g_stringAssigns != 1 || strcmp(c.FrontCommandLine.CurrentEntry.PStringData, "old") != 0 || !c.FrontCommandLine.UserEntry || c.FrontCommandLine.CursorPosition != 2) { std::printf("009ea980_TEST FAIL\n"); return 1; }
    c.PrevInHistory();                                              // nothing older: position restored
    if (c.HistoryPos.p != &g_n2 || g_stringAssigns != 1) { std::printf("009ea980_TEST FAIL\n"); return 1; }
    c.HistoryPos = c.CommandLineHistory.end(); c.PrevInHistory();  // at end(): untouched
    if (c.HistoryPos.p != &g_sentinel || g_stringAssigns != 1) { std::printf("009ea980_TEST FAIL\n"); return 1; }
    std::printf("009ea980_TEST PASS\n"); return 0;
}

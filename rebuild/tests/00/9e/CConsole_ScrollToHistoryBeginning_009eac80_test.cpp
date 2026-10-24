// Behaviour test for 0x009EAC80 CConsole::ScrollToHistoryBeginning: Start = begin(), Finish = begin() advanced by
// min(size, lines onscreen).
#include <cstdio>

struct Node { Node* next; Node* prev; int value; };

struct Iter
{
    Node* p;
    Iter() : p(0) {}
    Iter(Node* q) : p(q) {}
    Iter& operator++() { p = p->next; return *this; }
    Iter& operator--() { p = p->prev; return *this; }
};

template <class _Distance> void Advance(Iter& i, _Distance n)
{
    if (n > 0)
        while (n--) ++i;
    else
        while (n++) --i;
}

struct List
{
    Node* head;
    Iter begin() const { return Iter(head->next); }
    unsigned int size() const { unsigned int n = 0; for (Node* p = head->next; p != head; p = p->next) ++n; return n; }
};

struct Console
{
    List CommandLineHistory;
    Iter VisibleHistoryStart;
    Iter VisibleHistoryFinish;
    int NumberOfHistoryLinesOnscreen;                // retail long is 32-bit: compared unsigned against size()

    void ScrollToHistoryBeginning()
    {
        VisibleHistoryStart = VisibleHistoryFinish = CommandLineHistory.begin();
        if (CommandLineHistory.size() > NumberOfHistoryLinesOnscreen)
            Advance(VisibleHistoryFinish, NumberOfHistoryLinesOnscreen);
        else
            Advance(VisibleHistoryFinish, CommandLineHistory.size());
    }
};

static Node gNodes[13];

static void Build(int count)
{
    Node* head = &gNodes[0];
    Node* prev = head;
    for (int i = 1; i <= count; ++i) { gNodes[i].value = i; prev->next = &gNodes[i]; gNodes[i].prev = prev; prev = &gNodes[i]; }
    prev->next = head; head->prev = prev;
}

int main()
{
    Console c;
    c.CommandLineHistory.head = &gNodes[0];
    c.NumberOfHistoryLinesOnscreen = 9;
    bool ok = true;

    Build(12);                                   // more lines than fit: Finish = 10th element (9 steps from begin)
    c.ScrollToHistoryBeginning();
    ok = ok && c.VisibleHistoryStart.p == &gNodes[1] && c.VisibleHistoryFinish.p == &gNodes[10];

    Build(4);                                    // fewer lines: Finish = end()
    c.ScrollToHistoryBeginning();
    ok = ok && c.VisibleHistoryStart.p == &gNodes[1] && c.VisibleHistoryFinish.p == &gNodes[0];

    Build(0);                                    // empty: both at the sentinel
    c.ScrollToHistoryBeginning();
    ok = ok && c.VisibleHistoryStart.p == &gNodes[0] && c.VisibleHistoryFinish.p == &gNodes[0];

    Build(12);                                   // a negative count compares unsigned (huge): Finish advances by size() to end()
    c.NumberOfHistoryLinesOnscreen = -2;
    c.ScrollToHistoryBeginning();
    ok = ok && c.VisibleHistoryStart.p == &gNodes[1] && c.VisibleHistoryFinish.p == &gNodes[0];

    if (!ok) { std::printf("009eac80_TEST FAIL\n"); return 1; }
    std::printf("009eac80_TEST PASS\n");
    return 0;
}
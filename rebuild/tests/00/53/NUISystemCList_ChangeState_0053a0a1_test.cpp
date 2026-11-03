// Behaviour test for NUISystem::CList::ChangeState (0x0053A0A1): a model of the per-state observer bookkeeping
// (manager observer count, events observed on self) and of the LIST_ACTIVE re-centring of the middle child onto the
// selection and the GPreviousSelected hand-over.
#include <cstdio>
enum { INACTIVE = 0, ACTIVE = 1, SELECTED = 3, UNSELECTED = 4, LIST_ACTIVE = 5, LIST_INACTIVE = 6, LIST_UNSELECTED = 7 };
struct List;
static List* g_previous[4]; static int g_previousCount = 0;
static int g_observers = 0;
struct List {
    long n, sel, middleScrolls; bool recompute, partOfTree; unsigned long baseState, current; int selfEvents; bool observing;
    void Observe() { if (!observing) { observing = true; ++g_observers; } }
    void Ignore() { if (observing) { observing = false; --g_observers; } }
    void ChangeState(unsigned long state)
    {
        switch (state) {
        case LIST_INACTIVE: Ignore(); baseState = current == INACTIVE ? INACTIVE : LIST_INACTIVE; break;
        case LIST_ACTIVE: {
            bool r = recompute;
            if (recompute) recompute = false;
            if (r && n) {
                long middle = (n % 2 == 0) ? n / 2 - 1 : n / 2;
                while (middle != sel) { middle += middle < sel ? 1 : -1; ++middleScrolls; }
            }
            if (partOfTree) {
                if (g_previousCount > 0) g_previous[0]->Ignore();
                for (int i = g_previousCount; i > 0; --i) g_previous[i] = g_previous[i - 1];
                g_previous[0] = this; ++g_previousCount;
            }
            baseState = LIST_ACTIVE;
            if (partOfTree && g_previousCount > 1) g_previous[1]->baseState = LIST_UNSELECTED;
            break; }
        case UNSELECTED: case INACTIVE: Ignore(); baseState = state; break;
        case SELECTED: case ACTIVE: Observe(); selfEvents = 2; baseState = state; break;
        default: baseState = state; break;
        }
    }
};
int main()
{
    List a = { 6, 4, 0, true, true, 0, 0, 0, false };
    List b = { 3, 0, 0, false, true, 0, 0, 0, false };
    a.ChangeState(SELECTED);
    if (g_observers != 1 || a.selfEvents != 2 || a.baseState != SELECTED) { std::printf("0053a0a1_TEST FAIL\n"); return 1; }
    a.ChangeState(LIST_ACTIVE);
    if (a.middleScrolls != 2 || a.recompute || g_previous[0] != &a || a.baseState != LIST_ACTIVE) { std::printf("0053a0a1_TEST FAIL\n"); return 1; }
    b.ChangeState(LIST_ACTIVE);
    if (g_previous[0] != &b || a.observing || a.baseState != LIST_UNSELECTED || g_observers != 0) { std::printf("0053a0a1_TEST FAIL\n"); return 1; }
    a.ChangeState(LIST_INACTIVE);
    if (a.baseState != INACTIVE) { std::printf("0053a0a1_TEST FAIL\n"); return 1; }
    a.current = LIST_ACTIVE;
    a.ChangeState(LIST_INACTIVE);
    if (a.baseState != LIST_INACTIVE) { std::printf("0053a0a1_TEST FAIL\n"); return 1; }
    a.ChangeState(2);
    if (a.baseState != 2) { std::printf("0053a0a1_TEST FAIL\n"); return 1; }
    std::printf("0053a0a1_TEST PASS\n");
    return 0;
}
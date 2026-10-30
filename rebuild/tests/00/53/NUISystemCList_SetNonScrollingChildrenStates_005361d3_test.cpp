// Behaviour test for NUISystem::CList::SetNonScrollingChildrenStates (retail 0x005361D3).
#include <cstdio>
struct CComponent {
    unsigned long states[4]; unsigned long current; bool hasOwn; bool hasChild;
    bool HasState(unsigned long s) { return hasOwn && s == states[0]; }
    bool HasStateInChildren(unsigned long s) { return hasChild && s == states[0]; }
    void ChangeState(unsigned long s) { current = s; }
};
struct CList {
    long SelectedChild; unsigned int nScrolling; CComponent** nonFirst; unsigned int nNon;
    void SetNonScrollingChildrenStates()
    {
        if (nScrolling > 0) {
            for (unsigned int i = 0; i < nNon; ++i)
                if (nonFirst[i]->HasState(SelectedChild + 8) || nonFirst[i]->HasStateInChildren(SelectedChild + 8))
                    nonFirst[i]->ChangeState(SelectedChild + 8);
                else
                    nonFirst[i]->ChangeState(1);
        } else {
            for (unsigned int j = 0; j < nNon; ++j)
                nonFirst[j]->ChangeState(1);
        }
    }
};
int main()
{
    CComponent a = { {10,0,0,0}, 0, true, false }, b = { {10,0,0,0}, 0, false, true }, c = { {11,0,0,0}, 0, true, true };
    CComponent* kids[3] = { &a, &b, &c };
    CList l = { 2, 1, kids, 3 };
    l.SetNonScrollingChildrenStates();
    bool ok = a.current == 10 && b.current == 10 && c.current == 1;
    l.nScrolling = 0; a.current = b.current = c.current = 99;
    l.SetNonScrollingChildrenStates();
    ok = ok && a.current == 1 && b.current == 1 && c.current == 1;
    if (!ok) { std::printf("005361d3_TEST FAIL\n"); return 1; }
    std::printf("005361d3_TEST PASS\n");
    return 0;
}
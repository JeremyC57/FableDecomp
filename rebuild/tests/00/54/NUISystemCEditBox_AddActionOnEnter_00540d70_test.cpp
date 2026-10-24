// Behaviour test for 0x00540d70 NUISystem::CEditBox::AddActionOnEnter
#include <cstdio>
#include <list>
struct CCountedPointer { int* Data; };
struct CEditBox {
    std::list<CCountedPointer> ActionsOnEnter, ActionsOnEscape;
    void AddActionOnEnter(CCountedPointer& action) { ActionsOnEnter.push_back(action); }
};
int main()
{
    int a = 1, b = 2;
    CEditBox e; CCountedPointer pa = { &a }, pb = { &b };
    e.AddActionOnEnter(pa);
    e.AddActionOnEnter(pb);
    int ok = e.ActionsOnEnter.size() == 2 && e.ActionsOnEnter.front().Data == &a && e.ActionsOnEnter.back().Data == &b;
    ok = ok && (e.ActionsOnEnter.size() + e.ActionsOnEscape.size()) == 2;
    if (!ok) { std::printf("00540d70_TEST FAIL\n"); return 1; }
    std::printf("00540d70_TEST PASS\n");
    return 0;
}
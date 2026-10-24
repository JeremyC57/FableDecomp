// Behaviour test for 0x00540da0 NUISystem::CEditBox::AddActionOnEscape
#include <cstdio>
#include <list>
struct CCountedPointer { int* Data; };
struct CEditBox {
    std::list<CCountedPointer> ActionsOnEnter, ActionsOnEscape;
    void AddActionOnEscape(CCountedPointer& action) { ActionsOnEscape.push_back(action); }
};
int main()
{
    int a = 1, b = 2;
    CEditBox e; CCountedPointer pa = { &a }, pb = { &b };
    e.AddActionOnEscape(pa);
    e.AddActionOnEscape(pb);
    int ok = e.ActionsOnEscape.size() == 2 && e.ActionsOnEscape.front().Data == &a && e.ActionsOnEscape.back().Data == &b;
    ok = ok && (e.ActionsOnEnter.size() + e.ActionsOnEscape.size()) == 2;
    if (!ok) { std::printf("00540da0_TEST FAIL\n"); return 1; }
    std::printf("00540da0_TEST PASS\n");
    return 0;
}
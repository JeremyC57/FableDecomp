// Behaviour test for 0x0048573D NUISystem::CList::ClearPreviousSelected(): the static list of previously selected lists
// (CList::GPreviousSelected) is emptied; other state is untouched. Self-contained model (std::list stands in for STLport's).
#include <cstdio>
#include <list>
namespace NUISystem {
class CList {
public:
    long Id;
    static std::list<CList*> GPreviousSelected;
    static void ClearPreviousSelected() { GPreviousSelected.clear(); }
};
std::list<CList*> CList::GPreviousSelected;
}
int main()
{
    NUISystem::CList a, b;
    a.Id = 1; b.Id = 2;
    NUISystem::CList::GPreviousSelected.push_front(&a);
    NUISystem::CList::GPreviousSelected.push_front(&b);
    NUISystem::CList::ClearPreviousSelected();
    int ok = NUISystem::CList::GPreviousSelected.empty() && a.Id == 1 && b.Id == 2;
    NUISystem::CList::ClearPreviousSelected();
    ok = ok && NUISystem::CList::GPreviousSelected.size() == 0;
    if (!ok) { std::printf("0048573d_TEST FAIL\n"); return 1; }
    std::printf("0048573d_TEST PASS\n");
    return 0;
}

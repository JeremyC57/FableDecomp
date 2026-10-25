// Behaviour test for 0x0053BEAC (NUISystem::CList::AddScrollingChildDynamically): child recorded in each list structure (ScrollingChildrenStates gets a copy of the child's GetStates() map).
#include <cstdio>
struct StateMap { long marker; };
struct Comp { StateMap states; StateMap& GetStates() { return states; } };
template <class T> struct Arr { T v[8]; int n; Arr() : n(0) {} void push_back(const T& x) { v[n++] = x; } int size() const { return n; } };
struct List {
    int baseAdds; Arr<int> rows; Arr<long> rowIndices; Arr<Comp*> children; Arr<bool> visible; Arr<StateMap> statesList;
    void AddChildDynamically(Comp* const&) { ++baseAdds; rows.push_back(0); }
    void AddScrolling(Comp* const& child)
    {
        AddChildDynamically(child);
        children.push_back(child);
        visible.push_back(true);
        statesList.push_back(child->GetStates());
        rowIndices.push_back(rows.size() - 1);
    }
};
int main()
{
    List l; l.baseAdds = 0;
    Comp a, b; a.states.marker = 7; b.states.marker = 9;
    l.AddScrolling(&a); l.AddScrolling(&b);
    int ok = l.baseAdds == 2 && l.children.size() == 2 && l.children.v[1] == &b && l.visible.v[0] && l.statesList.v[1].marker == 9 && l.rowIndices.v[0] == 0 && l.rowIndices.v[1] == 1;
    if (!ok) { std::printf("0053beac_TEST FAIL\n"); return 1; }
    std::printf("0053beac_TEST PASS\n");
    return 0;
}
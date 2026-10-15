// Behaviour test for 0x0053BEAC (NUISystem::CList::AddScrollingChildDynamically): child recorded in each list structure.
#include <cstdio>
struct Comp { long key; long GetLayoutKey() { return key; } };
template <class T> struct Arr { T v[8]; int n; Arr() : n(0) {} void push_back(const T& x) { v[n++] = x; } int size() const { return n; } };
struct List {
    int baseAdds; Arr<int> rows; Arr<long> rowIndices; Arr<Comp*> children; Arr<bool> visible; Arr<long> layout;
    void AddChildDynamically(Comp* const&) { ++baseAdds; rows.push_back(0); }
    void AddScrolling(Comp* const& child)
    {
        AddChildDynamically(child);
        children.push_back(child);
        visible.push_back(true);
        layout.push_back(child->GetLayoutKey());
        rowIndices.push_back(rows.size() - 1);
    }
};
int main()
{
    List l; l.baseAdds = 0;
    Comp a = { 7 }, b = { 9 };
    l.AddScrolling(&a); l.AddScrolling(&b);
    int ok = l.baseAdds == 2 && l.children.size() == 2 && l.children.v[1] == &b && l.visible.v[0] && l.layout.v[1] == 9 && l.rowIndices.v[0] == 0 && l.rowIndices.v[1] == 1;
    if (!ok) { std::printf("0053beac_TEST FAIL\n"); return 1; }
    std::printf("0053beac_TEST PASS\n");
    return 0;
}
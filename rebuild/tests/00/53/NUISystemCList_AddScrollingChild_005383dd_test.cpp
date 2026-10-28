// Behaviour test for 0x005383DD NUISystem::CList::AddScrollingChild: CComponent::AddChild(child) first, then the child pointer appended to ScrollingChildren, true to ScrollingChildrenVisibility, and the child's index in Children (size - 1) to ShapeChildren
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
struct CComponent;
struct CP { CComponent* Data; void* Info; CComponent* operator->() const { return Data; } };
template <class T> struct Vec { T items[8]; unsigned n; Vec() : n(0) { } void push_back(const T& x) { items[n++] = x; } unsigned size() const { return n; } };
static int g_step = 0, g_addStep = 0;
struct CComponent {
    Vec<CP> Children; Vec<unsigned int> ShapeChildren;
    void AddChild(CP& c) { g_addStep = ++g_step; Children.push_back(c); }
};
struct CList : CComponent {
    Vec<CComponent*> ScrollingChildren; Vec<bool> ScrollingChildrenVisibility;
    void AddScrollingChild(CP& child)
    {
        CComponent::AddChild(child);
        ScrollingChildren.push_back(child.operator->());
        ScrollingChildrenVisibility.push_back(true);
        ShapeChildren.push_back(Children.size() - 1);
    }
};
int main()
{
    CList l; CComponent c0, c1; CP p0; p0.Data = &c0; p0.Info = 0; CP p1; p1.Data = &c1; p1.Info = 0;
    CP other; other.Data = 0; other.Info = 0; l.Children.push_back(other);          // a non-scrolling child already present
    l.AddScrollingChild(p0);
    CHECK(g_addStep == 1 && l.Children.n == 2 && l.ScrollingChildren.n == 1 && l.ScrollingChildren.items[0] == &c0);
    CHECK(l.ScrollingChildrenVisibility.n == 1 && l.ScrollingChildrenVisibility.items[0] == true);
    CHECK(l.ShapeChildren.n == 1 && l.ShapeChildren.items[0] == 1);
    l.AddScrollingChild(p1);
    CHECK(l.ScrollingChildren.items[1] == &c1 && l.ShapeChildren.items[1] == 2);
    if (fails == 0) printf("005383dd_TEST PASS\n"); else printf("005383dd_TEST FAIL\n");
    return fails;
}
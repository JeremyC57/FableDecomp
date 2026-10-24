// Behaviour test for 0055bb40: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct C2DBoxI { long X0, Y0, X1, Y1; };
template <class T> struct CRetailList {
    T items[8];
    int count;
    struct iterator {
        T* p;
        iterator(T* q) : p(q) { }
        T& operator*() const { return *p; }
        iterator& operator++() { ++p; return *this; }
        bool operator!=(const iterator& o) const { return p != o.p; }
    };
    iterator begin() { return iterator(items); }
    iterator end() { return iterator(items + count); }
};
class CComponent {
public:
    long Priority;
    bool Hoverable;
    bool Inside;
    CComponent(long p, bool h, bool in) : Priority(p), Hoverable(h), Inside(in) { }
    virtual ~CComponent() { }
    virtual long GetHoverPriority() { return Priority; }
    virtual bool CanBeHovered() { return Hoverable; }
    virtual bool IsHovered(C2DBoxI& mouse) { return Inside && mouse.X0 >= 0; }
};
class CHoverable : public CComponent {
public:
    CHoverable(long p, bool h, bool in) : CComponent(p, h, in) { }
    static CRetailList<CHoverable*> GAlreadyHovered;
    static bool GIsAnyAlreadyHovered(C2DBoxI& mouse, CComponent* component)
    {
        for (CRetailList<CHoverable*>::iterator it = GAlreadyHovered.begin(); it != GAlreadyHovered.end(); ++it) {
            CHoverable* hovered = *it;
            if (hovered->CanBeHovered() && component->CanBeHovered()) {
                if (hovered->IsHovered(mouse) && component->GetHoverPriority() >= hovered->GetHoverPriority())
                    return true;
            }
            else {
                if (hovered->IsHovered(mouse) && component->GetHoverPriority() == hovered->GetHoverPriority())
                    return true;
            }
        }
        return false;
    }
};
CRetailList<CHoverable*> CHoverable::GAlreadyHovered;
static void RunTest()
{
    C2DBoxI mouse = { 10, 10, 11, 11 };
    CHoverable low(1, true, true), high(5, true, true), outside(0, true, false), passive(3, false, true);
    CComponent me(3, true, true), mePassive(3, false, true);
    CHoverable::GAlreadyHovered.count = 0;
    CHECK(!CHoverable::GIsAnyAlreadyHovered(mouse, &me));            // empty list
    CHoverable::GAlreadyHovered.items[0] = &high;
    CHoverable::GAlreadyHovered.items[1] = &outside;
    CHoverable::GAlreadyHovered.count = 2;
    CHECK(!CHoverable::GIsAnyAlreadyHovered(mouse, &me));            // high outranks me; outside is not under the mouse
    CHoverable::GAlreadyHovered.items[2] = &low;
    CHoverable::GAlreadyHovered.count = 3;
    CHECK(CHoverable::GIsAnyAlreadyHovered(mouse, &me));             // both hoverable: my 3 >= low's 1
    CHoverable::GAlreadyHovered.items[0] = &passive;
    CHoverable::GAlreadyHovered.count = 1;
    CHECK(CHoverable::GIsAnyAlreadyHovered(mouse, &me));             // passive one: equal priority required (3 == 3)
    CHoverable::GAlreadyHovered.items[0] = &low;
    CHECK(!CHoverable::GIsAnyAlreadyHovered(mouse, &mePassive));     // I am not hoverable: 3 != 1
    CHoverable lowSame(3, true, true);
    CHoverable::GAlreadyHovered.items[0] = &lowSame;
    CHECK(CHoverable::GIsAnyAlreadyHovered(mouse, &mePassive));      // equal priority
    C2DBoxI away = { -1, 0, 0, 0 };
    CHECK(!CHoverable::GIsAnyAlreadyHovered(away, &me));             // nothing under the mouse
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055bb40_TEST FAIL\n");
        return 1;
    }
    std::printf("0055bb40_TEST PASS\n");
    return 0;
}
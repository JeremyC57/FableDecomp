// Behaviour test for 0x00A71B40 __pop_heap: *result = *first, adjust_heap(first, 0, last - first, value, comp); the by-value value's reference released.
#include <cstdio>
class CCharString { public:
    const char* s;
    CCharString() : s(0) {}
    CCharString(const CCharString& o) : s(o.s) {}
    CCharString& operator=(const CCharString& o) { s = o.s; return *this; }
};
struct Block { int refs; Block(int r) : refs(r) {} };
struct CP {
    int* m_pObject; Block* m_pCount;
    CP() : m_pObject(0), m_pCount(0) {}
    void Set(int* o, Block* b) { m_pObject = o; m_pCount = b; }
    CP(const CP& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->refs; }
    CP& operator=(const CP& o)
    {
        if (m_pCount != o.m_pCount) {
            if (m_pCount) { --m_pCount->refs; if (m_pCount->refs == 0) delete m_pCount; }
            m_pObject = o.m_pObject; m_pCount = o.m_pCount; if (m_pCount) ++m_pCount->refs;
        }
        return *this;
    }
    ~CP() { if (m_pCount) { --m_pCount->refs; if (m_pCount->refs == 0) delete m_pCount; } m_pObject = 0; m_pCount = 0; }
};
struct Named { CCharString Name; CP Pointer; Named() {} Named& operator=(const Named& o) { Name = o.Name; Pointer = o.Pointer; return *this; } }; struct Elem { unsigned int first; Named second; };
struct Comp { };
static Elem* g_first = 0; static int g_hole = -1, g_len = -1;
static Elem g_value;
static void adjust_heap(Elem* first, int hole, int len, Elem value, Comp) { g_first = first; g_hole = hole; g_len = len; g_value = value; }
static void pop_heap(Elem* first, Elem* last, Elem* result, Elem value, Comp comp, int*)
{
    *result = *first;
    adjust_heap(first, 0, int(last - first), value, comp);
}
int main()
{
    static const char* names[3] = { "a", "b", "c" };
    static int objs[3];
    Elem a[3];
    for (int i = 0; i < 3; ++i) { a[i].first = 100 + i; a[i].second.Name.s = names[i]; a[i].second.Pointer.Set(&objs[i], new Block(1)); }
    Elem res;
    Comp comp;
    pop_heap(a, a + 3, &res, a[2], comp, 0);
    if (res.first != 100 || res.second.Name.s != names[0] || res.second.Pointer.m_pObject != &objs[0] || g_value.first != 102 || g_value.second.Name.s != names[2] || g_value.second.Pointer.m_pObject != &objs[2] || g_first != a || g_hole != 0 || g_len != 3 || a[2].second.Pointer.m_pCount->refs != 2) { std::printf("00a71b40_TEST FAIL\n"); return 1; }
    std::printf("00a71b40_TEST PASS\n");
    return 0;
}
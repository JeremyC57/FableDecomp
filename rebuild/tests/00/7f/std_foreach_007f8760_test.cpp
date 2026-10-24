// Behaviour test for 007f8760 (std::for_each with SKillThing): every live particle thing in the range is killed (not immediately); empty slots are skipped; the functor comes back by value.
#include <cstdio>
static const void* g_killed[8];
static bool g_immediate[8];
static int g_count = 0;
class CThing {
public:
    int id;
    void Kill(bool immediately) { g_immediate[g_count] = immediately; g_killed[g_count++] = this; }   // stands in for 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    CThing* GetPItem() const { return PItem; }    // stands in for 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
};
typedef CIntelligentPointer<CThing> PThing;
template <class A, class R> struct unary_function {
    typedef A argument_type;
    typedef R result_type;
};
struct SKillThing : unary_function<PThing, void> {
    int marker;
    void operator()(PThing& thing) const
    {
        if (thing.GetPItem())
            thing.GetPItem()->Kill(false);
    }
};
template <class It, class F> F for_each(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
int main()
{
    CThing a, b;
    a.id = 1; b.id = 2;
    PThing list[3];
    list[0].PItem = &a;
    list[2].PItem = &b;
    SKillThing f;
    f.marker = 42;
    SKillThing r = for_each(list, list, f);
    if (g_count != 0 || r.marker != 42) { std::printf("007f8760_TEST FAIL\n"); return 1; }
    r = for_each(list, list + 3, f);
    if (g_count != 2 || g_killed[0] != &a || g_killed[1] != &b || g_immediate[0] || g_immediate[1] || r.marker != 42) { std::printf("007f8760_TEST FAIL\n"); return 1; }
    std::printf("007f8760_TEST PASS\n");
    return 0;
}
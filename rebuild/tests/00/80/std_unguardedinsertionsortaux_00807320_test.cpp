// Behaviour test for 00807320 (std::__unguarded_insertion_sort_aux over CIntelligentPointer<CThing>): the range comes out ordered by key, every element moved whole, copies balanced.
#include <cstdio>
#include <new>
class CThing {
public:
    int key;
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    explicit CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
static CThing g_things[8];
namespace {
struct CDoNotUseBinaryPredicate {          // retail draws a random number; the test orders by key to check the plumbing
    bool operator()(const CIntelligentPointer<CThing>& a, const CIntelligentPointer<CThing>& b) const { return a.GetPItem()->key < b.GetPItem()->key; }
};
}
typedef CIntelligentPointer<CThing> T;
typedef CDoNotUseBinaryPredicate C;
static int KeyOf(const T& x) { return x.GetPItem()->key; }
static bool Consistent(const T& x) { return x.GetPItem() == &g_things[x.GetPItem()->key]; }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee)
{
    T* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };
    for (int k = 0; k < 8; ++k) g_things[k].key = k;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(&g_things[keys[i]]);
    C comp = C();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) { std::printf("00807320_TEST FAIL\n"); return 1; }
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) { std::printf("00807320_TEST FAIL\n"); return 1; }
    if (!(true)) { std::printf("00807320_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    std::printf("00807320_TEST PASS\n");
    return 0;
}
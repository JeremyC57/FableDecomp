// Behaviour test for 0x0076BB70 _STL::__insertion_sort over pair<unsigned long, CParticleUpdateParamDesc<ulong,ulong> >.
#include <cstdio>

struct CBase { virtual ~CBase() { } virtual int Kind() const { return 0; } };
struct CParticleUpdateParamCallback { int id; };
struct CParticleUpdateParamDesc : public CBase {
    CParticleUpdateParamCallback* Callback;
    unsigned long Index;
    virtual int Kind() const { return 1; }
};
struct SKeyedDesc {
    unsigned long first; CParticleUpdateParamDesc second;
    SKeyedDesc() : first(0) { second.Callback = 0; second.Index = 0; }
    SKeyedDesc(const SKeyedDesc& o) : first(o.first), second(o.second) { }
};
struct CParticleUpdateCCompareParamDesc {
    void* m_context;
    bool operator()(const SKeyedDesc& a, const SKeyedDesc& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SKeyedDesc* first, SKeyedDesc* last, SKeyedDesc value, CParticleUpdateCCompareParamDesc comp)
{
    ++g_calls;
    if (value.second.Kind() != 1) return;          // the argument copy carries the descriptor's vtable
    if (comp(value, *first)) {
        for (SKeyedDesc* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SKeyedDesc* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SKeyedDesc* first, SKeyedDesc* last, CParticleUpdateCCompareParamDesc comp)
{
    if (first == last)
        return;
    for (SKeyedDesc* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    CParticleUpdateParamCallback cb[4] = { {0}, {1}, {2}, {3} };
    SKeyedDesc a[4];
    unsigned long keys[4] = { 0x80000002ul, 7, 0x80000001ul, 3 };
    int ids[4] = { 3, 1, 2, 0 };
    for (int k = 0; k < 4; ++k) { a[k].first = keys[k]; a[k].second.Callback = &cb[ids[k]]; a[k].second.Index = 100 + ids[k]; }
    CParticleUpdateCCompareParamDesc comp = { 0 };
    __insertion_sort(a, a + 4, comp);
    unsigned long sorted[4] = { 3, 7, 0x80000001ul, 0x80000002ul };
    for (int k = 0; k < 4; ++k)
        if (a[k].first != sorted[k] || a[k].second.Callback->id != k || a[k].second.Index != (unsigned long)(100 + k)) { std::printf("0076bb70_TEST FAIL\n"); return 1; }
    if (g_calls != 3) { std::printf("0076bb70_TEST FAIL\n"); return 1; }
    std::printf("0076bb70_TEST PASS\n");
    return 0;
}
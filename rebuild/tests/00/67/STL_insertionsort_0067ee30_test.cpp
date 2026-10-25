// Behaviour test for 0x0067EE30 std::__insertion_sort over CDegradableInfo with CDegradableDef::CIsHealth1GTHealth2 (health descending; __linear_insert / copy_backward inlined).
#include <cstdio>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CEngineGraphic { long m_words[4]; };
struct CDegradableInfo {
    float          HealthPercentage;
    CEngineGraphic EngineGraphic;
    long           SmashParticleEmitter;
    bool           BlocksNavigation;
};
struct CIsHealth1GTHealth2 {
    bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
};

static int g_unguarded = 0;
static void __unguarded_linear_insert(CDegradableInfo* last, CDegradableInfo value, CIsHealth1GTHealth2 comp)
{
    ++g_unguarded;
    CDegradableInfo* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
static CDegradableInfo* __copy_backward(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
static CDegradableInfo* copy_backward(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
static void __linear_insert(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo value, CIsHealth1GTHealth2 comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}
static void __insertion_sort(CDegradableInfo* first, CDegradableInfo* last, CIsHealth1GTHealth2 comp)
{
    if (first == last)
        return;
    for (CDegradableInfo* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    float health[6] = { 0.25f, 0.75f, 0.5f, 1.0f, 0.1f, 0.5f };
    CDegradableInfo a[6];
    for (int k = 0; k < 6; ++k) {
        a[k].HealthPercentage = health[k];
        for (int w = 0; w < 4; ++w) a[k].EngineGraphic.m_words[w] = k * 10 + w;
        a[k].SmashParticleEmitter = 100 + k;
        a[k].BlocksNavigation = (k & 1) != 0;
    }
    CIsHealth1GTHealth2 comp;
    __insertion_sort(a, a + 6, comp);
    // descending by health, stable: 1.0(3) 0.75(1) 0.5(2) 0.5(5) 0.25(0) 0.1(4)
    long order[6] = { 3, 1, 2, 5, 0, 4 };
    for (int j = 0; j < 6; ++j) {
        long k = order[j];
        if (a[j].HealthPercentage != health[k] || a[j].SmashParticleEmitter != 100 + k || a[j].EngineGraphic.m_words[3] != k * 10 + 3
            || a[j].BlocksNavigation != ((k & 1) != 0)) { std::printf("0067ee30_TEST FAIL\n"); return 1; }
    }
    // 0.75 and 1.0 go to the front through copy_backward; 0.5, 0.1, 0.5 take the unguarded path
    if (g_unguarded != 3) { std::printf("0067ee30_TEST FAIL\n"); return 1; }
    __insertion_sort(a, a, comp);   // empty range is a no-op
    std::printf("0067ee30_TEST PASS\n");
    return 0;
}
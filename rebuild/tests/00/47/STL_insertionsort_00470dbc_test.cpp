// Behaviour test for 0x00470DBC _STL::__insertion_sort over pair<unsigned long, {long, long}> records.
#include <cstdio>

struct SPayload8 { long m_value0; long m_value1; };
struct SULongKeyedRecord {
    unsigned long first; SPayload8 second;
    SULongKeyedRecord() : first(0) { second.m_value0 = second.m_value1 = 0; }
    SULongKeyedRecord(const SULongKeyedRecord& o) : first(o.first), second(o.second) { }
};
struct SULongKeyLess {
    void* m_context;
    bool operator()(const SULongKeyedRecord& a, const SULongKeyedRecord& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SULongKeyedRecord* first, SULongKeyedRecord* last, SULongKeyedRecord value, SULongKeyLess comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SULongKeyedRecord* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SULongKeyedRecord* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SULongKeyedRecord* first, SULongKeyedRecord* last, SULongKeyLess comp)
{
    if (first == last)
        return;
    for (SULongKeyedRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SULongKeyedRecord a[5];
    unsigned long keys[5] = { 9, 0xFFFFFFF0ul, 2, 9, 1 };
    for (int k = 0; k < 5; ++k) { a[k].first = keys[k]; a[k].second.m_value0 = k; a[k].second.m_value1 = -k; }
    SULongKeyLess comp = { 0 };
    __insertion_sort(a, a + 5, comp);
    unsigned long sorted[5] = { 1, 2, 9, 9, 0xFFFFFFF0ul };
    long origin[5] = { 4, 2, 0, 3, 1 };      // stable among equal keys
    for (int k = 0; k < 5; ++k)
        if (a[k].first != sorted[k] || a[k].second.m_value0 != origin[k] || a[k].second.m_value1 != -origin[k]) { std::printf("00470dbc_TEST FAIL\n"); return 1; }
    if (g_calls != 4) { std::printf("00470dbc_TEST FAIL\n"); return 1; }
    std::printf("00470dbc_TEST PASS\n");
    return 0;
}
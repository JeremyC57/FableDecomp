#include <cstdio>
struct SPayload { long m_words[6]; };
struct SFloatKeyedRecord {
    float    key;
    SPayload payload;
};
struct SFloatKeyLess {
    bool operator()(const SFloatKeyedRecord& a, const SFloatKeyedRecord& b) const { return a.key < b.key; }
};
void __unguarded_linear_insert(SFloatKeyedRecord* last, SFloatKeyedRecord value, SFloatKeyLess comp)
{
    SFloatKeyedRecord* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
int main()
{
    SFloatKeyedRecord r[5];
    float keys[5] = { 1.0f, 2.0f, 4.0f, 8.0f, 3.0f };
    int i;
    for (i = 0; i < 5; ++i) { r[i].key = keys[i]; r[i].payload.m_words[0] = i; r[i].payload.m_words[5] = 10 * i; }
    SFloatKeyedRecord v = r[4];
    SFloatKeyLess less;
    __unguarded_linear_insert(r + 4, v, less);
    float want[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 8.0f };
    long tag[5] = { 0, 1, 4, 2, 3 };
    for (i = 0; i < 5; ++i) if (r[i].key != want[i] || r[i].payload.m_words[0] != tag[i] || r[i].payload.m_words[5] != 10 * tag[i]) { std::printf("004cfd40_TEST FAIL\n"); return 1; }
    std::printf("004cfd40_TEST PASS\n");
    return 0;
}
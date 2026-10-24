#include <cstdio>
struct SFloatKeyedPair {
    float key;
    long  value;
};
struct SFloatKeyPairLess {
    bool operator()(const SFloatKeyedPair& a, const SFloatKeyedPair& b) const { return a.key < b.key; }
};
void __unguarded_linear_insert(SFloatKeyedPair* last, SFloatKeyedPair value, SFloatKeyPairLess comp)
{
    SFloatKeyedPair* next = last;
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
    SFloatKeyedPair r[5] = { { -1.0f, 10 }, { 0.5f, 11 }, { 2.0f, 12 }, { 3.0f, 13 }, { 0.0f, 14 } };
    SFloatKeyPairLess less;
    __unguarded_linear_insert(r + 4, r[4], less);
    float want[5] = { -1.0f, 0.0f, 0.5f, 2.0f, 3.0f };
    long val[5] = { 10, 14, 11, 12, 13 };
    int i;
    for (i = 0; i < 5; ++i) if (r[i].key != want[i] || r[i].value != val[i]) { std::printf("004cfcb0_TEST FAIL\n"); return 1; }
    SFloatKeyedPair v = { 5.0f, 99 };
    __unguarded_linear_insert(r + 4, v, less);
    if (r[4].value != 99 || r[3].value != 12) { std::printf("004cfcb0_TEST FAIL\n"); return 1; }
    std::printf("004cfcb0_TEST PASS\n");
    return 0;
}
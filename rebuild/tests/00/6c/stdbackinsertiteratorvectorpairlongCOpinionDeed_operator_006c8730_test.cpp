// Behaviour test for 0x006C8730 back_insert_iterator<vector<pair<long, COpinionDeed*> > >::operator=: push_back and return *this.
#include <cstdio>
#include <new>
struct Pair { long first; void* second; };
struct Vec {
    Pair* start; Pair* finish; Pair* eos; int overflows;
    void push_back(const Pair& x)
    {
        if (finish != eos) { new (finish) Pair(x); ++finish; }
        else ++overflows;
    }
};
struct BackInserter {
    Vec* container;
    BackInserter& assign(const Pair& v) { container->push_back(v); return *this; }
};
int main()
{
    Pair storage[2];
    Vec v; v.start = v.finish = storage; v.eos = storage + 2; v.overflows = 0;
    BackInserter it; it.container = &v;
    Pair p1 = { 7, &v }, p2 = { 8, 0 }, p3 = { 9, 0 };
    if (&it.assign(p1) != &it) { std::printf("006c8730_TEST FAIL\n"); return 1; }
    it.assign(p2); it.assign(p3);
    if (v.finish != storage + 2 || storage[0].first != 7 || storage[0].second != &v || storage[1].first != 8 || v.overflows != 1) {
        std::printf("006c8730_TEST FAIL\n"); return 1;
    }
    std::printf("006c8730_TEST PASS\n");
    return 0;
}
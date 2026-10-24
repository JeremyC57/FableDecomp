// Behaviour test for 0x00724A90 __find_if(CThing**, CThing**, CIsCreatureIsIntersecting, tag) (predicate modelled).
#include <cstdio>
class CThing { public: int hit; };
static int g_calls = 0;
struct CIsCreatureIsIntersecting {
    CThing* Owner;
    bool operator()(CThing* thing) const { ++g_calls; return thing->hit == Owner->hit; }
};
struct random_access_iterator_tag { };
static CThing** find_if_(CThing** first, CThing** last, CIsCreatureIsIntersecting pred, const random_access_iterator_tag&)
{
    int trip_count = (int)(last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
int main()
{
    CThing t[11];
    CThing* p[11];
    for (int i = 0; i < 11; ++i) { t[i].hit = i; p[i] = &t[i]; }
    CThing owner;
    CIsCreatureIsIntersecting pred;
    pred.Owner = &owner;
    random_access_iterator_tag tag;
    for (int n = 0; n <= 11; ++n)
        for (int k = -1; k < 11; ++k) {
            owner.hit = k;
            g_calls = 0;
            CThing** r = find_if_(p, p + n, pred, tag);
            CThing** want = (k >= 0 && k < n) ? p + k : p + n;
            int calls = (k >= 0 && k < n) ? k + 1 : n;
            if (r != want || g_calls != calls) { std::printf("00724a90_TEST FAIL\n"); return 1; }
        }
    std::printf("00724a90_TEST PASS\n");
    return 0;
}
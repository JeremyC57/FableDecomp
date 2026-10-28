// Behaviour test for 004b8ff0: std::__find over a const CCharString range (four-way unrolled scan, then the 0-3 tail;
// CCharString equality = both strings set, equal length, equal characters).
#include <cstdio>
#include <cstring>

struct S {                                              // CCharString model: PStringData null or a string
    const char* p;
    bool operator==(const S& o) const
    {
        if (p && o.p && std::strlen(p) == std::strlen(o.p))
            return std::strcmp(p, o.p) ? false : true;
        return false;
    }
};

static const S* find_model(const S* first, const S* last, const S& val)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first; ++first;
    case 0:
    default: return last;
    }
}

int main()
{
    S a[7] = { { "alpha" }, { 0 }, { "beta" }, { "gamma" }, { "delta" }, { "beta" }, { "omega" } };
    bool ok = true;
    for (int n = 0; n <= 7; ++n) {
        for (int i = 0; i < n; ++i) {
            if (!a[i].p) continue;
            const S* r = find_model(a, a + n, a[i]);
            int first = i;
            for (int k = 0; k < i; ++k) if (a[k] == a[i]) { first = k; break; }
            ok = ok && r == a + first;
        }
        S missing = { "zeta" };
        ok = ok && find_model(a, a + n, missing) == a + n;
        S null = { 0 };                                  // an empty string never compares equal
        ok = ok && find_model(a, a + n, null) == a + n;
    }
    if (!ok) { std::printf("004b8ff0_TEST FAIL\n"); return 1; }
    std::printf("004b8ff0_TEST PASS\n");
    return 0;
}
// Behaviour test for 00748760 SGI __median over CThing pointers: for every permutation of three
// distinct keys the returned element is the middle one under the comparator.
#include <cstdio>

class CThing { public: float m_distance; };
struct CCompare { long m_reference; bool operator()(CThing* a, CThing* b) const { return a->m_distance > b->m_distance; } };

static CThing* const* Median(CThing* const& a, CThing* const& b, CThing* const& c, CCompare comp)
{
    if (comp(a, b)) {
        if (comp(b, c)) return &b;
        else if (comp(a, c)) return &c;
        else return &a;
    } else if (comp(a, c)) return &a;
    else if (comp(b, c)) return &c;
    else return &b;
}

int main()
{
    CThing t[3]; t[0].m_distance = 1.0f; t[1].m_distance = 2.0f; t[2].m_distance = 3.0f;
    int perm[6][3] = { {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0} };
    CCompare comp; comp.m_reference = 0;
    bool ok = true;
    for (int i = 0; i < 6; ++i) {
        CThing* a = &t[perm[i][0]]; CThing* b = &t[perm[i][1]]; CThing* c = &t[perm[i][2]];
        CThing* const* m = Median(a, b, c, comp);
        if (*m != &t[1]) ok = false;
    }
    if (!ok) { std::printf("00748760_TEST FAIL\n"); return 1; }
    std::printf("00748760_TEST PASS\n");
    return 0;
}
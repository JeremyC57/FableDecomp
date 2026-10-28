extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
template <class It, class T> It find4(It first, It last, const T& val)   // STLport random-access __find
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
    case 1: if (*first == val) return first;
    default: return last;
    }
}
// Behaviour test for 0x00778AB0 CTCFamilyMember::IsRelatedTo: true only when &thing is one of FamilyMembers.
class CThing { public: int id; };
class CTCFamilyMember { public:
    const CThing* const* first; const CThing* const* last;
    bool IsRelatedTo(const CThing& t) const { const CThing* p = &t; return find4(first, last, p) != last; }
};
int main()
{
    CThing a, b, c, d, e, f; const CThing* v[6] = { &a, &b, &c, &d, &e, &f };
    CTCFamilyMember m; m.first = v; m.last = v + 5;
    CHECK(m.IsRelatedTo(a)); CHECK(m.IsRelatedTo(e)); CHECK(m.IsRelatedTo(c));
    CHECK(!m.IsRelatedTo(f));
    m.last = v; CHECK(!m.IsRelatedTo(a));
    if (fails == 0) printf("00778ab0_TEST PASS\n"); else printf("00778ab0_TEST FAIL\n");
    return fails;
}
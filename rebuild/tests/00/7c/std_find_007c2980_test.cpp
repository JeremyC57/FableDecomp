// Behaviour test for 0x007C2980 std::__find over CIntelligentPointer<const CThing> (random-access, 4-way unrolled).
#include <cstdio>

struct CBaseObject { int id; };

struct CBaseIntelligentPointer {
    CBaseObject* PItem;
    CBaseObject* GetPItem() const { return PItem; }
};
struct CThingPointer : CBaseIntelligentPointer { };

inline bool operator==(const CThingPointer& a, const CThingPointer& b) { return a.GetPItem() == b.GetPItem(); }

struct random_access_iterator_tag { };

template <class It, class T>
It find_ra(It first, It last, const T& val, const random_access_iterator_tag&)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
    case 0:
    default:
        return last;
    }
}

int main()
{
    CBaseObject objs[12];
    CThingPointer v[11];
    int i;
    for (i = 0; i < 11; ++i) { objs[i].id = i; v[i].PItem = &objs[i]; }
    random_access_iterator_tag tag;
    for (int n = 0; n <= 11; ++n) {
        for (i = 0; i < 12; ++i) {
            CThingPointer key; key.PItem = &objs[i];
            CThingPointer* r = find_ra(v, v + n, key, tag);
            CThingPointer* expect = (i < n) ? v + i : v + n;
            if (r != expect) { std::printf("007c2980_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("007c2980_TEST PASS\n");
    return 0;
}
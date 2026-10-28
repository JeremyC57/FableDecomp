// Behaviour test for 0x00407330 CUserProfileManager::IsTutorialCategoryGiven: STLport find (4-way unrolled __find) over the given-categories vector; true only when the category is present
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
enum ETutorialCategory { TC_0 = 0, TC_3 = 3, TC_7 = 7, TC_9 = 9 };
template <class It, class T> It find_unrolled(It first, It last, const T& val)   // STLport __find, random access
{
    int trip_count = (int)(last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
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
class CUserProfileManager { public:
    const ETutorialCategory* start; const ETutorialCategory* finish;
    bool IsTutorialCategoryGiven(ETutorialCategory c) const { return find_unrolled(start, finish, c) != finish; }
};
int main()
{
    static const ETutorialCategory v[7] = { TC_0, TC_3, TC_3, TC_9, TC_0, TC_0, TC_7 };
    CUserProfileManager m; m.start = v; m.finish = v + 7;
    CHECK(m.IsTutorialCategoryGiven(TC_7));      // in the switch tail
    CHECK(m.IsTutorialCategoryGiven(TC_9));      // in the unrolled block
    CHECK(m.IsTutorialCategoryGiven(TC_0));
    CHECK(!m.IsTutorialCategoryGiven((ETutorialCategory)5));
    m.finish = v + 6; CHECK(!m.IsTutorialCategoryGiven(TC_7));
    m.finish = v;     CHECK(!m.IsTutorialCategoryGiven(TC_0));
    if (fails == 0) printf("00407330_TEST PASS\n"); else printf("00407330_TEST FAIL\n");
    return fails;
}
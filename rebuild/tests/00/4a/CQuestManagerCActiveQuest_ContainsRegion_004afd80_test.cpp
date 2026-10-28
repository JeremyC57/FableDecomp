// Behaviour test for 0x004AFD80 CQuestManager::CActiveQuest::ContainsRegion: find of the region's address in the quest's region-pointer vector
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
class CRegion { public: int Id; };
template <class It, class T> It find_unrolled(It first, It last, const T& val)
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
struct CActiveQuest {
    const CRegion* const* start; const CRegion* const* finish;
    bool ContainsRegion(const CRegion& r) const { return find_unrolled(start, finish, &r) != finish ? true : false; }
};
int main()
{
    CRegion a = {1}, b = {2}, c = {3}, d = {1};
    const CRegion* v[5] = { &a, &b, &a, &b, &c };
    CActiveQuest q; q.start = v; q.finish = v + 5;
    CHECK(q.ContainsRegion(c));
    CHECK(q.ContainsRegion(a));
    CHECK(!q.ContainsRegion(d));                  // same contents, different region: identity compare
    q.finish = v + 4; CHECK(!q.ContainsRegion(c));
    if (fails == 0) printf("004afd80_TEST PASS\n"); else printf("004afd80_TEST FAIL\n");
    return fails;
}
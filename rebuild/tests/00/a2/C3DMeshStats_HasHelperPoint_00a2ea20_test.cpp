// Behaviour test for C3DMeshStats::HasHelperPoint 0x00A2EA20 (model copy of the source logic).
#include <cstdio>
struct Pair { unsigned int first; float pos[3]; int bone; };
struct Map {
    Pair* Elements; unsigned int Size;
    const Pair* end() const { return Elements + Size; }
    const Pair* LowerBound(const unsigned int& key) const      // sorted ascending, first element >= key
    {
        const Pair* p = Elements;
        while (p != end() && p->first < key) ++p;
        return p;
    }
    const Pair* Find(const unsigned int& key) const
    {
        const Pair* found = LowerBound(key);
        if (found == end() || key < found->first) return end();
        return found;
    }
};
struct Stats {
    Map HelperPoints;
    bool HasHelperPoint(unsigned int id) const
    {
        if (HelperPoints.Find(id) != HelperPoints.end()) return true;
        return false;
    }
};
int main()
{
    Pair p[4] = { {2, {0,0,0}, 0}, {5, {0,0,0}, 1}, {9, {0,0,0}, 2}, {40, {0,0,0}, 3} };
    Stats s; s.HelperPoints.Elements = p; s.HelperPoints.Size = 4;
    unsigned int yes[] = { 2, 5, 9, 40 }, no[] = { 0, 1, 3, 6, 10, 41, 1000 };
    for (int i = 0; i < 4; ++i) if (!s.HasHelperPoint(yes[i])) { std::printf("00a2ea20_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (s.HasHelperPoint(no[i])) { std::printf("00a2ea20_TEST FAIL\n"); return 1; }
    s.HelperPoints.Size = 0;
    if (s.HasHelperPoint(2)) { std::printf("00a2ea20_TEST FAIL\n"); return 1; }
    std::printf("00a2ea20_TEST PASS\n");
    return 0;
}
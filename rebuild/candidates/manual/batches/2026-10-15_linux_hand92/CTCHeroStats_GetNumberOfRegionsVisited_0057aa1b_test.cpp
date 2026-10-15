#include <cstdio>
// Behaviour test for CTCHeroStats::GetNumberOfRegionsVisited (0057aa1b): same logic over a copy of the model.
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    const long& operator[](unsigned long n) const { return first[n]; }
};
struct C { CLongVector RegionTime; long GetNumberOfRegionsVisited() const; };
long C::GetNumberOfRegionsVisited() const
{
    long n = 0;
    for (unsigned long i = 0; i != RegionTime.size(); ++i) {
        if (RegionTime[i] != 0)
            ++n;
    }
    return n;
}
int main()
{
    long data[6] = { 0, 3, -2, 0, 7, 1 };
    C c; c.RegionTime.first = data; c.RegionTime.last = data + 6; c.RegionTime.end_of_storage = data + 6;
    C e; e.RegionTime.first = data; e.RegionTime.last = data; e.RegionTime.end_of_storage = data;
    if (c.GetNumberOfRegionsVisited() != 4 || e.GetNumberOfRegionsVisited() != 0) { std::printf("0057aa1b_TEST FAIL\n"); return 1; }
    std::printf("0057aa1b_TEST PASS\n");
    return 0;
}

// Behaviour test for CRegionMap::GetCount (0x00C465D0): the map's region count is whatever its region list's virtual
// GetCount reports (forwarded through the member list at +4), for an empty and a filled list.
#include <cstdio>
struct RegionList {
    long n;
    virtual ~RegionList() {}
    virtual long GetCount() const { return n; }
};
struct RegionMap {
    void* allocator;
    RegionList m_regions;
    long GetCount() const { return m_regions.GetCount(); }
};
int main()
{
    RegionMap m;
    m.m_regions.n = 0;
    if (m.GetCount() != 0) { std::printf("00c465d0_TEST FAIL 1\n"); return 1; }
    m.m_regions.n = 37;
    if (m.GetCount() != 37) { std::printf("00c465d0_TEST FAIL 2\n"); return 1; }
    std::printf("00c465d0_TEST PASS\n");
    return 0;
}
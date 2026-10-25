// Behaviour test for 00c465b0: CRegionMap::ForEach -- forwards the visitor to the region list's ForEach
#include <cstdio>
class CRegionVisitor { public: int m_seen; };
static const void* g_list = 0; static CRegionVisitor* g_visitor = 0;
struct CRegionList { virtual void ForEach(CRegionVisitor& visitor) const { g_list = this; g_visitor = &visitor; ++visitor.m_seen; } };
struct CRegionMap { void* m_allocator; CRegionList m_regions; void ForEach(CRegionVisitor& visitor) const { m_regions.ForEach(visitor); } };
int main()
{
    CRegionMap m; CRegionVisitor v; v.m_seen = 0; m.ForEach(v);
    int ok = g_list == &m.m_regions && g_visitor == &v && v.m_seen == 1;
    std::printf(ok ? "00c465b0_TEST PASS\n" : "00c465b0_TEST FAIL\n");
    return ok ? 0 : 1;
}

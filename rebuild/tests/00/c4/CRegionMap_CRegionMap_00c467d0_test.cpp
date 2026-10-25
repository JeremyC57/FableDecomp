// Behaviour test for 00c467d0: CRegionMap::CRegionMap -- stores cell size and radius, builds the list over the allocator, walks radius / cellSize
#include <cstdio>
static float g_walkRadius = 0.0f; static const void* g_collectorList = 0; static float g_collectorCell = 0.0f; static const void* g_listAllocator = 0;
struct C2DVectorI { long X, Y; };
class CCellVisitor { public: virtual void Visit(const C2DVectorI& cell) = 0; };
class CNodeAllocator { public: virtual void Free(void*) { } virtual void* Alloc() { return 0; } };
class CRegionList { public: CRegionList(CNodeAllocator* allocator) : m_vptr(0), m_pImpl(0) { g_listAllocator = allocator; } ~CRegionList() { } void* m_vptr; void* m_pImpl; unsigned char m_impl[0x28]; };
class CRegionCollector : public CCellVisitor { public: CRegionCollector(CRegionList* regions, float cellSize) : m_pRegions(regions), m_cellSize(cellSize) { } virtual void Visit(const C2DVectorI&) { } CRegionList* m_pRegions; float m_cellSize; };
class CGridCircle { public: CGridCircle(float radius, CCellVisitor* visitor) { g_walkRadius = radius; CRegionCollector* c = (CRegionCollector*)visitor; g_collectorList = c->m_pRegions; g_collectorCell = c->m_cellSize; } };
class CRegionMap {
public:
    CNodeAllocator m_allocator; CRegionList m_regions; float m_cellSize; float m_radius;
    CRegionMap(float radius, float cellSize) : m_regions(&m_allocator)
    {
        m_cellSize = cellSize;
        m_radius = radius;
        CRegionCollector collector(&m_regions, cellSize);
        CGridCircle(radius / cellSize, &collector);
    }
};
static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    CRegionMap m(10.0f, 4.0f);
    int ok = Near(m.m_cellSize, 4.0f) && Near(m.m_radius, 10.0f) && Near(g_walkRadius, 2.5f) && g_collectorList == &m.m_regions
        && Near(g_collectorCell, 4.0f) && g_listAllocator == &m.m_allocator && sizeof(CRegionMap) == 0x3c;
    std::printf(ok ? "00c467d0_TEST PASS\n" : "00c467d0_TEST FAIL\n");
    return ok ? 0 : 1;
}

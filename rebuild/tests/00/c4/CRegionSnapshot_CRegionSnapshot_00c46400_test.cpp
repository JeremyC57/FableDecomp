// Behaviour test for 00c46400: CRegionSnapshot::CRegionSnapshot -- cell size, count and a copied entry array from the map
#include <cstdio>

struct C2DVectorI { int X, Y; C2DVectorI() : X(0), Y(0) { } };
struct SRegionEntry { int m_visited; C2DVectorI m_cell; };
struct CRegionVisitor {
    virtual void Visit(const SRegionEntry& entry) = 0;
    virtual ~CRegionVisitor() { }
};
struct CRegionCopier : public CRegionVisitor {
    SRegionEntry* m_pDest; int m_index; int m_count;
    CRegionCopier(SRegionEntry* dest, int count) : m_pDest(dest), m_index(0), m_count(count) { }
    virtual void Visit(const SRegionEntry& entry)
    {
        m_pDest[m_index].m_cell = entry.m_cell;
        m_pDest[m_index].m_visited = entry.m_visited;
        ++m_index;
    }
};
struct CRegionMap {
    SRegionEntry m_items[3]; int m_count; float m_cellSize;
    void ForEach(CRegionVisitor& visitor) const { for (int i = 0; i < m_count; ++i) visitor.Visit(m_items[i]); }
    int GetCount() const { return m_count; }
    float GetCellSize() const { return m_cellSize; }
};
struct CRegionSnapshot {
    float m_cellSize; int m_count; SRegionEntry* m_entries;
    CRegionSnapshot(const CRegionMap& map)
        : m_cellSize(map.GetCellSize()), m_count(map.GetCount()), m_entries(0)
    {
        m_entries = new SRegionEntry[m_count];
        CRegionCopier copier(m_entries, m_count);
        map.ForEach(copier);
    }
};

int main()
{
    CRegionMap map;
    for (int i = 0; i < 3; ++i) { map.m_items[i].m_visited = i & 1; map.m_items[i].m_cell.X = i * 2; map.m_items[i].m_cell.Y = -i; }
    map.m_count = 3; map.m_cellSize = 2.5f;
    CRegionSnapshot snap(map);
    int ok = snap.m_count == 3 && snap.m_cellSize > 2.49f && snap.m_cellSize < 2.51f && snap.m_entries != 0;
    ok &= snap.m_entries[1].m_visited == 1 && snap.m_entries[2].m_cell.X == 4 && snap.m_entries[2].m_cell.Y == -2;
    delete[] snap.m_entries;
    if (!ok) { std::printf("00c46400_TEST FAIL\n"); return 1; }
    std::printf("00c46400_TEST PASS\n");
    return 0;
}
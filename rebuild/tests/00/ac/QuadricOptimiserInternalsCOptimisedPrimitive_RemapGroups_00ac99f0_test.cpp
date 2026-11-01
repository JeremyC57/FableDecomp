// behaviour test for 0x00AC99F0 COptimisedPrimitive::RemapGroups
#include <cstdio>
struct CGroupRemap { int add; };
struct COptimisedVertex { int group; void RemapGroups(const CGroupRemap& r) { group += r.add; } };
template <class T> struct CRetailVector {
    T* m_first; T* m_last; T* m_end;
    unsigned int size() const { return (unsigned int)(m_last - m_first); }
    T& operator[](unsigned int n) { return m_first[n]; }
};
struct COptimisedPrimitive {
    CRetailVector<COptimisedVertex> Vertices;
    void RemapGroups(const CGroupRemap& remap) { for (unsigned int i = 0; i < Vertices.size(); ++i) Vertices[i].RemapGroups(remap); }
};
int main()
{
    COptimisedVertex v[3] = { { 1 }, { 2 }, { 3 } };
    COptimisedPrimitive p; p.Vertices.m_first = v; p.Vertices.m_last = v + 3; p.Vertices.m_end = v + 3;
    CGroupRemap r = { 10 };
    p.RemapGroups(r);
    COptimisedPrimitive e; e.Vertices.m_first = e.Vertices.m_last = e.Vertices.m_end = v;
    e.RemapGroups(r);
    if (v[0].group != 11 || v[1].group != 12 || v[2].group != 13)
    { std::printf("00ac99f0_TEST FAIL\n"); return 1; }
    std::printf("00ac99f0_TEST PASS\n");
    return 0;
}
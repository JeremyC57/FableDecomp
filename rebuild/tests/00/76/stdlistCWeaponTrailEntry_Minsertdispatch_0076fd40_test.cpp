// Behaviour test for 0x0076FD40 std::list<CWeaponTrailEntry>::_M_insert_dispatch (range insert before pos)
#include <cstdio>
#include <cstdlib>
#include <new>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CWeaponTrailEntry {
    C3DVector Point0;
    C3DVector Point1;
    float Time;
};
struct __false_type { __false_type() { } };
struct SNodeBase { SNodeBase* m_next; SNodeBase* m_prev; };
struct SNode : SNodeBase { CWeaponTrailEntry m_data; };
struct SIter {
    SNodeBase* m_node;
    SIter(SNodeBase* n) : m_node(n) { }
    SIter(const SIter& o) : m_node(o.m_node) { }
};
struct SConstIter { SNodeBase* m_node; };
struct CTrailList {
    SNodeBase* m_node;
    CTrailList() { m_node = (SNodeBase*)std::malloc(sizeof(SNode)); m_node->m_next = m_node; m_node->m_prev = m_node; }
    SIter end() { return SIter(m_node); }
    SIter begin() { return SIter(m_node->m_next); }
    SIter insert(SIter pos, const CWeaponTrailEntry& x)
    {
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        new (&tmp->m_data) CWeaponTrailEntry(x);
        SNodeBase* n = pos.m_node;
        SNodeBase* p = n->m_prev;
        tmp->m_next = n;
        tmp->m_prev = p;
        p->m_next = tmp;
        n->m_prev = tmp;
        return SIter(tmp);
    }
    void push_back(const CWeaponTrailEntry& x) { insert(end(), x); }
    void InsertDispatch(SIter pos, SConstIter first, SConstIter last, const __false_type&)
    {
        for (; first.m_node != last.m_node; first.m_node = first.m_node->m_next)
            insert(pos, ((SNode*)first.m_node)->m_data);
    }
    int size() const { int n = 0; for (SNodeBase* p = m_node->m_next; p != m_node; p = p->m_next) ++n; return n; }
    const CWeaponTrailEntry& at(int i) const { SNodeBase* p = m_node->m_next; while (i--) p = p->m_next; return ((SNode*)p)->m_data; }
};
static CWeaponTrailEntry MakeEntry(float base)
{
    CWeaponTrailEntry e = { C3DVector(base, base + 1, base + 2), C3DVector(base + 3, base + 4, base + 5), base + 6 };
    return e;
}
static bool Same(const CWeaponTrailEntry& a, const CWeaponTrailEntry& b)
{
    return a.Point0.X == b.Point0.X && a.Point0.Y == b.Point0.Y && a.Point0.Z == b.Point0.Z && a.Point1.X == b.Point1.X
        && a.Point1.Y == b.Point1.Y && a.Point1.Z == b.Point1.Z && a.Time == b.Time;
}
int main()
{
    CTrailList src, dst;
    src.push_back(MakeEntry(1));
    src.push_back(MakeEntry(10));
    src.push_back(MakeEntry(20));
    dst.push_back(MakeEntry(100));
    SConstIter first; first.m_node = src.m_node->m_next;
    SConstIter last; last.m_node = src.m_node;
    dst.InsertDispatch(dst.begin(), first, last, __false_type());   // before the existing element
    if (dst.size() != 4 || !Same(dst.at(0), MakeEntry(1)) || !Same(dst.at(1), MakeEntry(10)) || !Same(dst.at(2), MakeEntry(20))
        || !Same(dst.at(3), MakeEntry(100))) { std::printf("0076fd40_TEST FAIL\n"); return 1; }
    SConstIter e; e.m_node = src.m_node;
    dst.InsertDispatch(dst.end(), e, e, __false_type());             // empty range
    if (dst.size() != 4 || src.size() != 3) { std::printf("0076fd40_TEST FAIL\n"); return 1; }
    std::printf("0076fd40_TEST PASS\n");
    return 0;
}
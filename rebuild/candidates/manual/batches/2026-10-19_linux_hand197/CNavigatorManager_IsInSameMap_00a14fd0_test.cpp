#include <cstdio>
// Behaviour test for 00a14fd0 CNavigatorManager::IsInSameMap: the first map holding either point decides; true only when
// that map holds the first point and the second; half-open integer areas (truncated coordinates).
struct C3DVector { float X, Y, Z; };
struct C2DCoordI {
    long X, Y;
    C2DCoordI(const C3DVector& v) : X((long)v.X), Y((long)v.Y) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    bool Contains(const C2DCoordI& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};
struct CCharString { void* m_pData; };
template <class T> struct CCountedPointer { T* Data; void* Info; };
class CNavQuadTree;
struct CNavQuadTreeMap {                           // 32 bytes
    CCountedPointer<CNavQuadTree> PQuadTree;       // +0x00
    long                          Index;           // +0x08
    C2DBoxI                       MapArea;         // +0x0c
    CCharString                   SectionName;     // +0x1c
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;                           // +0x08
};
template <class T> struct list {                   // STLport list, 4 bytes
    _List_node<T>* _M_node;
    _List_node<T>* begin() const { return _M_node->_M_next; }
    _List_node<T>* end() const { return _M_node; }
};
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    bool IsInSameMap(const C3DVector& a, const C3DVector& b) const;
};

bool CNavigatorManager::IsInSameMap(const C3DVector& a, const C3DVector& b) const
{
    for (_List_node<CNavQuadTreeMap>* it = QuadTrees.begin(); it != QuadTrees.end(); it = it->_M_next) {
        if (it->_M_data.MapArea.Contains(C2DCoordI(a)))
            return it->_M_data.MapArea.Contains(C2DCoordI(b));
        if (it->_M_data.MapArea.Contains(C2DCoordI(b)))
            return false;
    }
    return false;
}

typedef _List_node<CNavQuadTreeMap> Node;
static C3DVector V(float x, float y) { C3DVector v; v.X = x; v.Y = y; v.Z = 0; return v; }
int main()
{
    static CNavigatorManager m;
    Node header, n1, n2;
    n1._M_data.MapArea.TLX = 0; n1._M_data.MapArea.TLY = 0; n1._M_data.MapArea.BRX = 10; n1._M_data.MapArea.BRY = 10;
    n2._M_data.MapArea.TLX = 10; n2._M_data.MapArea.TLY = 0; n2._M_data.MapArea.BRX = 20; n2._M_data.MapArea.BRY = 10;
    header._M_next = &n1; n1._M_next = &n2; n2._M_next = &header;
    m.QuadTrees._M_node = &header;
    bool ok = m.IsInSameMap(V(1.5f, 2.0f), V(9.9f, 9.9f));          // both in map 1
    ok = ok && !m.IsInSameMap(V(1.5f, 2.0f), V(10.0f, 5.0f));       // a in map 1, b in map 2 (BRX exclusive)
    ok = ok && !m.IsInSameMap(V(15.0f, 2.0f), V(5.0f, 5.0f));       // b's map 1 comes first
    ok = ok && m.IsInSameMap(V(15.0f, 2.0f), V(19.0f, 0.0f));       // both in map 2
    ok = ok && !m.IsInSameMap(V(-5.0f, 2.0f), V(-6.0f, 3.0f));      // neither (truncation toward zero keeps -5)
    header._M_next = &header;
    ok = ok && !m.IsInSameMap(V(1.0f, 1.0f), V(1.0f, 1.0f));        // empty list
    if (!ok) { std::printf("00a14fd0_TEST FAIL\n"); return 1; }
    std::printf("00a14fd0_TEST PASS\n");
    return 0;
}

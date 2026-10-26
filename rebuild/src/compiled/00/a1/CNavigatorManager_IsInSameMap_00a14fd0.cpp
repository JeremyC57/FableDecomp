// CNavigatorManager::IsInSameMap(const C3DVector&, const C3DVector&) -- find the first quad-tree map whose area holds either point: true only when it is the first point's map and that map also holds the second point
// Leaf: IsInSameMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14FD0, 200 bytes (+ int3 padding; `ret 8`).
// Callees: __ftol2 0x00BFEA70 (each float coordinate truncated to a map cell; the first point's pair is hoisted out of the loop
//   by the compiler, the second point's is redone in each test).
// Layout: Ego_r.pdb CNavigatorManager with STLport containers: QuadTrees (list<CNavQuadTreeMap>, a 4-byte header pointer) at
//   retail +0x24 (PDB +0x30); CNavQuadTreeMap {PQuadTree +0, Index +8, MapArea +0xc (C2DBoxI TLX, TLY, BRX, BRY), SectionName
//   +0x1c}, list node data at +8.
// Shape: the half-open C2DBoxI::Contains(C2DCoordI) is inline; the hit on the first point returns the second test's 1/0 in eax,
//   the misses return `xor al,al` through their own epilogues.
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(const C3DVector& v) : X((long)v.X), Y((long)v.Y) { }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    bool Contains(const C2DCoordI& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};
class CCharString { public: void* m_pData; };
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CNavQuadTree;
class CNavQuadTreeMap { public:                           // 32 bytes
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
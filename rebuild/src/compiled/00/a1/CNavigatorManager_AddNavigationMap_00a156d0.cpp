// CNavigatorManager::AddNavigationMap(long, CNavigationAreaInit&) -- add a quad-tree map for the map index unless one exists: append an entry, give it the init's world area, a new CNavQuadTree set up over that area and initialised from the init record
// Leaf: ?AddNavigationMap@CNavigatorManager@@QAEXJAAVCNavigationAreaInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A156D0, 237 bytes (+ int3 padding; `ret 8`).
// Callees: list<CNavQuadTreeMap>::insert(iterator) 0x00A17480 (STLport insert(pos, T()), landed under the propagated label
//   "CCombatSequence_WillForcePush::OnCreate"), operator new 0x00BFEA1A, CNavQuadTree::CNavQuadTree 0x00A7A760 (manifest label
//   "CPSCOrbit::CPSCOrbit" is propagated), CCountedPointer<CNavQuadTree>::Reset(T*) 0x00A16640, CNavQuadTree::SetUpMapStructure
//   (const C2DVector&, float, float, long) 0x00A762F0, CNavQuadTree::Initialise(CNavigationAreaInit&) 0x00A7A8D0.
// Layout: Ego_r.pdb CNavigatorManager with STLport containers: QuadTrees (list<CNavQuadTreeMap>) at retail +0x24;
//   CNavQuadTreeMap {PQuadTree +0, Index +8, MapArea +0xc, SectionName +0x1c}; CNavigationAreaInit WorldArea at retail +0x30
//   (PDB +0x40).
// Shape: the duplicate scan returns straight to the epilogue; the inserted iterator lands in the dead index-argument slot and
//   the entry is re-read as back(); the area is the implicit C2DBoxI copy; the origin is a named C2DVector local built before
//   the width/height arguments (each `(float)long` goes through a frame slot and fild).
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const class C2DCoordI& c);
};
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
inline C2DVector::C2DVector(const C2DCoordI& c) : X((float)c.X), Y((float)c.Y) { }
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DCoordI GetTopLeft() const { return C2DCoordI(TLX, TLY); }
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
};
class CCharString { public:
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CAFile;
class CASuspendableProcess;
class CNavigationAreaInit;
class CNavQuadTree {                               // 0x88 bytes
public:
    unsigned char _body[0x88];
    CNavQuadTree();                                // 0x00A7A760
    void SetUpMapStructure(const C2DVector& origin, float width, float height, long mapIndex);   // 0x00A762F0
    void Initialise(CNavigationAreaInit& init);    // 0x00A7A8D0
    bool LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess* process);   // 0x00A7AF60
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    void Reset(T* object);                         // 0x00A16640 for CNavQuadTree
};
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
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) { }
    _List_iterator(const _List_iterator& it) : _M_node(it._M_node) { }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
template <class T> class list {                    // STLport list, 4 bytes
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
    T& back() { return _M_node->_M_prev->_M_data; }
    iterator insert(iterator pos);                 // 0x00A17480: insert(pos, T())
    iterator erase(iterator pos);                  // 0x00A17110
};
class CNavigationAreaInit { public:
    unsigned char _pad0[0x30];
    C2DBoxI       WorldArea;                       // +0x30
};
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    void AddNavigationMap(long mapIndex, CNavigationAreaInit& init);
};

void CNavigatorManager::AddNavigationMap(long mapIndex, CNavigationAreaInit& init)
{
    {
        const list<CNavQuadTreeMap>& maps = QuadTrees;
        for (list<CNavQuadTreeMap>::iterator it = maps.begin(); it != maps.end(); ++it) {
            if (it->Index == mapIndex)
                return;
        }
    }
    QuadTrees.insert(QuadTrees.end());
    CNavQuadTreeMap& map = QuadTrees.back();
    map.Index = mapIndex;
    map.MapArea = init.WorldArea;
    map.PQuadTree.Reset(new CNavQuadTree);
    map.PQuadTree->SetUpMapStructure(map.MapArea.GetTopLeft(), (float)map.MapArea.GetWidth(), (float)map.MapArea.GetHeight(), mapIndex);
    map.PQuadTree->Initialise(init);
}
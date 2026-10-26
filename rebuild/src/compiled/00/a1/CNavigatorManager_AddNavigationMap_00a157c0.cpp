// CNavigatorManager::AddNavigationMap(long, const C2DBoxI&, const CCharString&, CAFile&, CASuspendableProcess*) -- add a quad-tree map for the map index unless one exists: append an entry with the area and section name, load a new CNavQuadTree for it from the file, and drop the entry again when the load fails
// Leaf: ?AddNavigationMap@CNavigatorManager@@QAEXJABVC2DBoxI@@ABVCCharString@@AAVCAFile@@PAVCASuspendableProcess@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A157C0, 196 bytes (+ int3 padding; `ret 14h`).
// Callees: list<CNavQuadTreeMap>::insert(iterator) 0x00A17480 and erase(iterator) 0x00A17110, CCharString::operator= 0x0099EFB0,
//   operator new 0x00BFEA1A, CNavQuadTree::CNavQuadTree 0x00A7A760, CCountedPointer<CNavQuadTree>::Reset(T*) 0x00A16640,
//   CNavQuadTree::LoadFromFile(CAFile&, long, const C2DBoxI&, CASuspendableProcess*) 0x00A7AF60 (a bool in al).
// Layout: as the CNavigationAreaInit overload 0x00A156D0: QuadTrees at retail +0x24; CNavQuadTreeMap {PQuadTree +0, Index +8,
//   MapArea +0xc, SectionName +0x1c}.
// Shape: the inserted entry is re-read as back(); on a failed load the last entry is erased through a by-value iterator
//   (--end(), the prev link of the header) whose result lands in the dead index-argument slot.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
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
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
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
class CNavigatorManager {
public:
    void*                 __vftable;               // +0x00
    unsigned char         _pad04[0x20];
    list<CNavQuadTreeMap> QuadTrees;               // +0x24
    void AddNavigationMap(long mapIndex, const C2DBoxI& area, const CCharString& sectionName, CAFile& file, CASuspendableProcess* process);
};

void CNavigatorManager::AddNavigationMap(long mapIndex, const C2DBoxI& area, const CCharString& sectionName, CAFile& file, CASuspendableProcess* process)
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
    map.MapArea = area;
    map.SectionName = sectionName;
    map.PQuadTree.Reset(new CNavQuadTree);
    if (!map.PQuadTree->LoadFromFile(file, mapIndex, area, process))
        QuadTrees.erase(QuadTrees._M_node->_M_prev);
}
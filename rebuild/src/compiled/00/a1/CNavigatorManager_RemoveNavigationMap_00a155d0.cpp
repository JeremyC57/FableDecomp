// CNavigatorManager::RemoveNavigationMap(long) -- unload one navigation map: every other loaded quad tree drops its links to it, its own entry is erased from the map list, and every registered navigator is notified
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A155D0, 147 bytes (+ int3 padding; `ret 4`).
// Callees: CNavQuadTree::OnNavMapUnload(long) @ 0x00A79290 (label "CVectorMap_EraseRange_Keys" is propagated; the PDB names the
//   only (long) member of CNavQuadTree that fits), ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC (counted block),
//   free @ 0x00BFEA14 (STLport malloc-allocator list node), the navigator's virtual slot 18 (+0x48).
// Layout (retail, PDB minus 0xc): the navigator list at +0x18 (STLport list<CNavigatorBase*>), QuadTrees at +0x24
//   (STLport list<CNavQuadTreeMap>, 0x28-byte nodes: next, prev, then PQuadTree {Data, Info} +8, Index +0x10, MapArea +0x14,
//   SectionName +0x24 -- as the landed ctor 0x00A15670 allocates them).
// Shape: the walk remembers the matching node (no break) and calls OnNavMapUnload on every other one; STLport `erase` unlinks
//   (prev->next, next->prev), runs the implicit ~CNavQuadTreeMap (out-of-line ~CCharString, then the inline counted-pointer
//   release with its DeleteFunc block and the two zero stores) and frees the node.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
struct CCPPointerInfo {                                  // RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
    T* operator->() const { return Data; }
};
struct CCharString {
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
};
struct CNavQuadTree {
    void OnNavMapUnload(long index);                     // 0x00A79290
};
struct CNavQuadTreeMap {
    CCountedPointer<CNavQuadTree> PQuadTree;             // +0x00
    long                          Index;                 // +0x08
    C2DBoxI                       MapArea;               // +0x0c
    CCharString                   SectionName;           // +0x1c
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* node) : _M_node(node) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    T* operator->() const { return &((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> struct list {                         // STLport list over the malloc allocator: the sentinel pointer only
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node<T>* n = (_List_node<T>*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~T();
        free(n);
        return iterator(next);
    }
};
}
struct CNavigatorBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void OnNavigationMapRemoved();               // slot 18 (+0x48)
};
struct CNavigatorManager {
    void*                            __vftable;          // +0x00
    unsigned char                    _pad04[0x14];
    std::list<CNavigatorBase*>       Navigators;         // +0x18
    void*                            m_pOwner;           // +0x1c
    long                             m_capacity;         // +0x20
    std::list<CNavQuadTreeMap>       QuadTrees;          // +0x24
    void RemoveNavigationMap(long index);
};

void CNavigatorManager::RemoveNavigationMap(long index)
{
    std::list<CNavQuadTreeMap>::iterator found = QuadTrees.end();
    for (std::list<CNavQuadTreeMap>::iterator it = QuadTrees.begin(); it != QuadTrees.end(); ++it) {
        if (it->Index == index)
            found = it;
        else
            it->PQuadTree->OnNavMapUnload(index);
    }
    if (found != QuadTrees.end())
        QuadTrees.erase(found);
    for (std::list<CNavigatorBase*>::iterator nav = Navigators.begin(); nav != Navigators.end(); ++nav)
        (*nav)->OnNavigationMapRemoved();
}
// CTCInventoryMap::UpdateRegionsCorrespondances(long startFrame, long endFrame) -- pairs up the region-left (0x26) and region-entered (0x27) messages of the frame window, newest first, and records every new (to, from) region pair in the map's correspondence list unless either orientation is already there
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FC0C0, 524 bytes, `ret 8`. Speed TU (no pragmas, no frame pointer).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list allocator), CTCBase::GetWorld 0x00686A70,
//   CMessageEventManager::FindAllMessages<CFilter_Bind2<IsOfType, OccuredBetween>> 0x006178F0 (same-TU stand-in),
//   the filters' operator() 0x006AF1C0 / 0x006AF180, CWorld slot 13 (+0x34) GetWorldMap, CWorldMap::GetRegion 0x004FC180.
// Shape: the search goes through an inline CWorld::FindAllMessages(filter, list&) wrapper, so the filter temporaries are
//   stored before GetWorld and the pushes follow it (the IsOfType temporaries land in the dead argument slots); both
//   lists walk newest first (reverse iterators kept in the argument slots); the first find leaves a `bool absent = true`
//   cleared when it hits, and end() is read once into a named local reused by the second compare and the insert.
// Layout: CWorld +0x60 message manager (engine/CWorld.h); STLport list nodes {next, prev, data +8}; the message's map
//   number +0x3C; local view of CTCInventoryMap: region-correspondence list<pair<CRegion*, CRegion*>> +0x24C.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

template <class T>
struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T>
struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) {}
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) {}
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
    bool operator==(const _List_iterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class It, class T>
struct reverse_iterator {
    It current;
    reverse_iterator(const It& it) : current(it) {}
    T& operator*() const { It tmp = current; return *--tmp; }
    reverse_iterator& operator++() { --current; return *this; }
    bool operator!=(const reverse_iterator& other) const { return current != other.current; }
};
template <class T>
struct _STLP_alloc_proxy {
    _List_node<T>* _M_data;
    _STLP_alloc_proxy(_List_node<T>* p) : _M_data(p) {}
};
template <class T>
struct list {
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    typedef reverse_iterator<iterator, T> riterator;
    _STLP_alloc_proxy<T> _M_node;
    list() : _M_node(0)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node._M_data = node;
    }
    ~list()
    {
        clear();
        free(_M_node._M_data);
    }
    void clear()
    {
        _Node* cur = _M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    iterator begin() const { return iterator(_M_node._M_data->_M_next); }
    iterator end() const { return iterator(_M_node._M_data); }
    riterator rbegin() const { return riterator(end()); }
    riterator rend() const { return riterator(begin()); }
    _Node* _M_create_node(const T& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        new (&tmp->_M_data) T(x);
        return tmp;
    }
    void insert(iterator pos, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _Node* n = pos._M_node;
        _Node* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};
template <class It, class T>
inline It find(It first, It last, const T& val)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}

struct CRegion;
struct CRegionPair {                                    // pair<CRegion*, CRegion*>
    CRegion* first;
    CRegion* second;
    CRegionPair(CRegion* a, CRegion* b) : first(a), second(b) {}
    CRegionPair(const CRegionPair& o) : first(o.first), second(o.second) {}
    bool operator==(const CRegionPair& o) const { return first == o.first && second == o.second; }
};

struct CMessageEvent {
    unsigned char _pad[0x3c];
    long          MapNumber;                            // +0x3c
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
};
struct CMessageEventFilter_OccuredBetween {
    long m_start;
    long m_end;
    CMessageEventFilter_OccuredBetween(long start, long end) : m_start(start), m_end(end) { }
    bool operator()(const CMessageEvent& message) const;   // 0x006AF180
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
    bool operator()(const CMessageEvent& message) const { return (*m_pFirst)(message) && (*m_pSecond)(message); }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> CBetweenFilter;

struct CMessageEventManager {
    void*               __vftable;                      // +0x00
    list<CMessageEvent> Messages;                       // +0x04
    long FindAllMessages(const CBetweenFilter& filter, list<CMessageEvent*>* found);   // 0x006178F0
};
#pragma auto_inline(off)
// Same-TU stand-in for FindAllMessages<CFilter_Bind2<IsOfType, OccuredBetween>> 0x006178F0 (the landed 0x0089AC80 form):
// every accepted message's address appended to the out list, the out list's size returned.
long CMessageEventManager::FindAllMessages(const CBetweenFilter& filter, list<CMessageEvent*>* found)
{
    for (list<CMessageEvent>::iterator it = Messages.begin(); it != Messages.end(); ++it)
        if (filter(*it))
            found->push_back(&*it);
    long count = 0;
    for (list<CMessageEvent*>::iterator it = found->begin(); it != found->end(); ++it)
        ++count;
    return count;
}
#pragma auto_inline(on)

struct CWorldMap {
    CRegion& GetRegion(long map);                       // 0x004FC180
};
struct CWorld {
    virtual void w00(); virtual void w01(); virtual void w02(); virtual void w03(); virtual void w04(); virtual void w05(); virtual void w06();
    virtual void w07(); virtual void w08(); virtual void w09(); virtual void w10(); virtual void w11(); virtual void w12();
    virtual CWorldMap* GetWorldMap();                   // slot 13 (+0x34)
    unsigned char _pad04[0x5c];
    CMessageEventManager* PMessageEventManager_Data;   // +0x60
    long FindAllMessages(const CBetweenFilter& filter, list<CMessageEvent*>& found) { return PMessageEventManager_Data->FindAllMessages(filter, &found); }
};

struct CTCInventoryMap {
    void*             __vftable;                        // +0x00
    unsigned char     _pad04[0x24c - 0x04];
    list<CRegionPair> RegionCorrespondances;            // +0x24C
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    void UpdateRegionsCorrespondances(long startFrame, long endFrame);
};
void CTCInventoryMap::UpdateRegionsCorrespondances(long startFrame, long endFrame)
{
    list<CMessageEvent*> lefts;
    list<CMessageEvent*> entereds;
    GetWorld()->FindAllMessages(CMessageEventFilter_IsOfType(0x26) && CMessageEventFilter_OccuredBetween(startFrame, endFrame), lefts);
    GetWorld()->FindAllMessages(CMessageEventFilter_IsOfType(0x27) && CMessageEventFilter_OccuredBetween(startFrame, endFrame), entereds);
    list<CMessageEvent*>::riterator left = lefts.rbegin();
    list<CMessageEvent*>::riterator entered = entereds.rbegin();
    for (; entered != entereds.rend(); ++left, ++entered) {
        CMessageEvent* from = *left;
        CMessageEvent* to = *entered;
        CRegion* a = &GetWorld()->GetWorldMap()->GetRegion(from->MapNumber);
        CRegion* b = &GetWorld()->GetWorldMap()->GetRegion(to->MapNumber);
        bool absent = true;
        list<CRegionPair>::iterator it = find(RegionCorrespondances.begin(), RegionCorrespondances.end(), CRegionPair(a, b));
        list<CRegionPair>::iterator last = RegionCorrespondances.end();
        if (it != last)
            absent = false;
        if (find(RegionCorrespondances.begin(), RegionCorrespondances.end(), CRegionPair(b, a)) == last && absent)
            RegionCorrespondances.insert(last, CRegionPair(b, a));
    }
}

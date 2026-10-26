// CGameScriptInterface::MsgOnLevelUnloaded(std::list<CCharString>& levels) const -- collect every "level unloaded" message (type 0x25) seen or heard since the previous frame, append each one's level script name to the list, and say whether the list holds any name
// Leaf: ?MsgOnLevelUnloaded@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089AC80, length 256 bytes (+ int3 padding), `ret 4`.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the STLport list allocator), the message frame getters
//   0x006E7510 / 0x006E7530 (landed MsgOn* family), CMessageEventManager::FindAllMessages<CFilter_Bind2<IsOfType,
//   CanBeSeenOrHeard>>(const filter&, list<CMessageEvent*>*) @ 0x006178F0 (compiled below as a same-TU stand-in so VC7.1
//   knows the out list's node word is neither written nor kept -- retail keeps it in edi and reuses its slot),
//   the filters' operator() @ 0x006AF1C0 / 0x006AF180, CWorld virtual slot 13 (+0x34) GetWorldMap,
//   CWorldMap::GetLevelScriptName(long) @ 0x004FCFC0, CCharString(const CCharString&) @ 0x0099EC30.
// Layout: STLport list nodes {next +0, prev +4, data +8}; the message's level (map) number at +0x3c; the manager's
//   message list at +0x04 (CWorld +0x60).
// Shape: the message list is built first, the frame window is (current - 1, max), the manager read into a local
//   before the filters (landed MsgOnExpressionPerformed 0x00893B00 form), push_back = insert(end(), name) with end()
//   read before the node allocation, the result `!levels.empty()` taken before the message list is destroyed.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

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
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
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
            tmp->_M_data.~T();
            free(tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    bool empty() const { return _M_node._M_data->_M_next == _M_node._M_data; }
    iterator begin() const { return iterator(_M_node._M_data->_M_next); }
    iterator end() const { return iterator(_M_node._M_data); }
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

class CMessageEvent { public:
    unsigned char _pad[0x3c];
    long          MapNumber;                            // +0x3c
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
    bool operator()(const CMessageEvent& message) const;   // 0x006AF180
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
    bool operator()(const CMessageEvent& message) const { return (*m_pFirst)(message) && (*m_pSecond)(message); }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;

class CMessageEventManager { public:
    void*               __vftable;                      // +0x00
    list<CMessageEvent> Messages;                       // +0x04
    long FindAllMessages(const CSeenOrHeardFilter& filter, list<CMessageEvent*>* found);   // 0x006178F0
};
#pragma auto_inline(off)
// Same-TU stand-in for FindAllMessages<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> 0x006178F0 (retail's LTCG knowledge):
// the retail body -- every accepted message's address appended to the out list, the out list's size returned.
long CMessageEventManager::FindAllMessages(const CSeenOrHeardFilter& filter, list<CMessageEvent*>* found)
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

class CWorldMap { public:
    const CCharString& GetLevelScriptName(long map);    // 0x004FCFC0
};
class CWorld { public:
    virtual void w00(); virtual void w01(); virtual void w02(); virtual void w03(); virtual void w04(); virtual void w05(); virtual void w06();
    virtual void w07(); virtual void w08(); virtual void w09(); virtual void w10(); virtual void w11(); virtual void w12();
    virtual CWorldMap* GetWorldMap();                   // slot 13 (+0x34)
    unsigned char _pad04[0x5c];
    CMessageEventManager* PMessageEventManager_Data;   // +0x60
};

class CGameScriptInterface { public:
    virtual bool MsgOnLevelUnloaded(list<CCharString>& levels) const;
    CWorld* World;                                  // +0x04
    CWorld* GetWorld() const { return World; }
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
};

bool CGameScriptInterface::MsgOnLevelUnloaded(list<CCharString>& levels) const
{
    list<CMessageEvent*> messages;
    long frame = GetCurrentWorldFrameForMessages() - 1;
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager* manager = World->PMessageEventManager_Data;
    manager->FindAllMessages(CMessageEventFilter_IsOfType(0x25) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame), &messages);
    for (list<CMessageEvent*>::iterator it = messages.begin(); it != messages.end(); ++it) {
        CMessageEvent* message = *it;
        levels.push_back(World->GetWorldMap()->GetLevelScriptName(message->MapNumber));
    }
    return !levels.empty();
}
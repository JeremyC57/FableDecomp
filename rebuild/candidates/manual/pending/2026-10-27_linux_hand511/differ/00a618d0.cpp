// CBusyResourceManager::OnPostSwapScreens() -- after a present: retire every finished GPU event query (or the oldest ones while more than 8 are in flight) into the free list, recording the finished frame; then issue a fresh event query (a recycled one, else a new one from the device) for this frame
// Grade: DIFFER(421v421) -- register allocation only, in the move block: retail keeps the free list's sentinel in edi (the dead size() walk runs on edi directly, `mov edi,edi` pad) and the source node payload in ebx; ours swaps them (ebx sentinel, walk through `mov ecx,ebx`, edi payload). Every other instruction matches.
// Leaf: ?OnPostSwapScreens@CBusyResourceManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A618D0, 421 bytes, thiscall, plain `ret`.
// Identity: the layout is Ego_r.pdb CBusyResourceManager (FreeQueoryObjects +0x00, PendingQueories +0x04 -- STLport
//   lists of 4 bytes --, PendingFrame +0x08, FinishedFrame +0x0C, D3DDevice +0x10; CPendingQuery {IDirect3DQuery9* Query;
//   long Frame;} = the 8-byte node payload), and of its PDB methods (SetDevice, ClearAllQueries, OnPostSwapScreens)
//   only OnPostSwapScreens fits a no-argument body that issues and polls queries. Its landed neighbours are this class's
//   ctor 0x00A61880 (PendingFrame = 1, FinishedFrame = 0), ClearAllQueries 0x00A61A80 and dtor 0x00A61B10 (catalogued as
//   "CDirectInputDeviceLists_00a61b10"). No direct callers (an inlined call site elsewhere). Catalog label
//   "Deque_Process_And_Rebalance" is descriptive.
// Callees: IDirect3DQuery9::GetData (slot 7, D3DGETDATA_FLUSH) and Issue (slot 6, D3DISSUE_END),
//   IDirect3DDevice9::CreateQuery (slot 118, D3DQUERYTYPE_EVENT), malloc 0x00BFEA0E / free 0x00BFEA14 (the node allocator).
// Shape: STLport list with the walking size(); the two counter-less walks before the move are unused size() calls VC7.1
//   keeps (it cannot prove the walk terminates).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct IDirect3DQuery9 {
    virtual long __stdcall QueryInterface(const void* iid, void** out) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual long __stdcall GetDevice(void** device) = 0;
    virtual long __stdcall GetType() = 0;
    virtual unsigned long __stdcall GetDataSize() = 0;
    virtual long __stdcall Issue(unsigned long issueFlags) = 0;                                    // slot 6
    virtual long __stdcall GetData(void* data, unsigned long size, unsigned long getDataFlags) = 0; // slot 7
};

struct IDirect3DDevice9 {
    virtual void __stdcall Slot000() = 0; virtual void __stdcall Slot001() = 0; virtual void __stdcall Slot002() = 0;
    virtual void __stdcall Slot003() = 0; virtual void __stdcall Slot004() = 0; virtual void __stdcall Slot005() = 0;
    virtual void __stdcall Slot006() = 0; virtual void __stdcall Slot007() = 0; virtual void __stdcall Slot008() = 0;
    virtual void __stdcall Slot009() = 0; virtual void __stdcall Slot010() = 0; virtual void __stdcall Slot011() = 0;
    virtual void __stdcall Slot012() = 0; virtual void __stdcall Slot013() = 0; virtual void __stdcall Slot014() = 0;
    virtual void __stdcall Slot015() = 0; virtual void __stdcall Slot016() = 0; virtual void __stdcall Slot017() = 0;
    virtual void __stdcall Slot018() = 0; virtual void __stdcall Slot019() = 0; virtual void __stdcall Slot020() = 0;
    virtual void __stdcall Slot021() = 0; virtual void __stdcall Slot022() = 0; virtual void __stdcall Slot023() = 0;
    virtual void __stdcall Slot024() = 0; virtual void __stdcall Slot025() = 0; virtual void __stdcall Slot026() = 0;
    virtual void __stdcall Slot027() = 0; virtual void __stdcall Slot028() = 0; virtual void __stdcall Slot029() = 0;
    virtual void __stdcall Slot030() = 0; virtual void __stdcall Slot031() = 0; virtual void __stdcall Slot032() = 0;
    virtual void __stdcall Slot033() = 0; virtual void __stdcall Slot034() = 0; virtual void __stdcall Slot035() = 0;
    virtual void __stdcall Slot036() = 0; virtual void __stdcall Slot037() = 0; virtual void __stdcall Slot038() = 0;
    virtual void __stdcall Slot039() = 0; virtual void __stdcall Slot040() = 0; virtual void __stdcall Slot041() = 0;
    virtual void __stdcall Slot042() = 0; virtual void __stdcall Slot043() = 0; virtual void __stdcall Slot044() = 0;
    virtual void __stdcall Slot045() = 0; virtual void __stdcall Slot046() = 0; virtual void __stdcall Slot047() = 0;
    virtual void __stdcall Slot048() = 0; virtual void __stdcall Slot049() = 0; virtual void __stdcall Slot050() = 0;
    virtual void __stdcall Slot051() = 0; virtual void __stdcall Slot052() = 0; virtual void __stdcall Slot053() = 0;
    virtual void __stdcall Slot054() = 0; virtual void __stdcall Slot055() = 0; virtual void __stdcall Slot056() = 0;
    virtual void __stdcall Slot057() = 0; virtual void __stdcall Slot058() = 0; virtual void __stdcall Slot059() = 0;
    virtual void __stdcall Slot060() = 0; virtual void __stdcall Slot061() = 0; virtual void __stdcall Slot062() = 0;
    virtual void __stdcall Slot063() = 0; virtual void __stdcall Slot064() = 0; virtual void __stdcall Slot065() = 0;
    virtual void __stdcall Slot066() = 0; virtual void __stdcall Slot067() = 0; virtual void __stdcall Slot068() = 0;
    virtual void __stdcall Slot069() = 0; virtual void __stdcall Slot070() = 0; virtual void __stdcall Slot071() = 0;
    virtual void __stdcall Slot072() = 0; virtual void __stdcall Slot073() = 0; virtual void __stdcall Slot074() = 0;
    virtual void __stdcall Slot075() = 0; virtual void __stdcall Slot076() = 0; virtual void __stdcall Slot077() = 0;
    virtual void __stdcall Slot078() = 0; virtual void __stdcall Slot079() = 0; virtual void __stdcall Slot080() = 0;
    virtual void __stdcall Slot081() = 0; virtual void __stdcall Slot082() = 0; virtual void __stdcall Slot083() = 0;
    virtual void __stdcall Slot084() = 0; virtual void __stdcall Slot085() = 0; virtual void __stdcall Slot086() = 0;
    virtual void __stdcall Slot087() = 0; virtual void __stdcall Slot088() = 0; virtual void __stdcall Slot089() = 0;
    virtual void __stdcall Slot090() = 0; virtual void __stdcall Slot091() = 0; virtual void __stdcall Slot092() = 0;
    virtual void __stdcall Slot093() = 0; virtual void __stdcall Slot094() = 0; virtual void __stdcall Slot095() = 0;
    virtual void __stdcall Slot096() = 0; virtual void __stdcall Slot097() = 0; virtual void __stdcall Slot098() = 0;
    virtual void __stdcall Slot099() = 0; virtual void __stdcall Slot100() = 0; virtual void __stdcall Slot101() = 0;
    virtual void __stdcall Slot102() = 0; virtual void __stdcall Slot103() = 0; virtual void __stdcall Slot104() = 0;
    virtual void __stdcall Slot105() = 0; virtual void __stdcall Slot106() = 0; virtual void __stdcall Slot107() = 0;
    virtual void __stdcall Slot108() = 0; virtual void __stdcall Slot109() = 0; virtual void __stdcall Slot110() = 0;
    virtual void __stdcall Slot111() = 0; virtual void __stdcall Slot112() = 0; virtual void __stdcall Slot113() = 0;
    virtual void __stdcall Slot114() = 0; virtual void __stdcall Slot115() = 0; virtual void __stdcall Slot116() = 0;
    virtual void __stdcall Slot117() = 0;
    virtual long __stdcall CreateQuery(long type, IDirect3DQuery9** query) = 0;                     // slot 118 (+0x1D8)
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { free(p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp> struct _Nonconst_traits;
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self& operator--() { this->_M_decr(); return *this; }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef unsigned int size_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = 0;
        for (_List_node_base* __n = this->_M_node._M_data->_M_next; __n != this->_M_node._M_data; __n = __n->_M_next)
            ++__result;
        return __result;
    }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void pop_back() { iterator __tmp = end(); erase(--__tmp); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

}   // namespace std

class CBusyResourceManager {
public:
    struct CPendingQuery {                                // Ego_r.pdb, 8 bytes
        IDirect3DQuery9* Query;                           // +0x00
        long             Frame;                           // +0x04
    };
    std::list<CPendingQuery> FreeQueoryObjects;           // +0x00
    std::list<CPendingQuery> PendingQueories;             // +0x04
    long                     PendingFrame;                // +0x08
    long                     FinishedFrame;               // +0x0c
    IDirect3DDevice9*        D3DDevice;                   // +0x10
    void OnPostSwapScreens();
};

void CBusyResourceManager::OnPostSwapScreens()
{
    while (PendingQueories.size() > 0 &&
           (PendingQueories.back().Query->GetData(0, 0, 1) == 0 || PendingQueories.size() > 8)) {
        PendingQueories.size();
        FreeQueoryObjects.size();
        FinishedFrame = PendingQueories.back().Frame;
        FreeQueoryObjects.push_back(PendingQueories.back());
        PendingQueories.pop_back();
    }
    if (FreeQueoryObjects.size() == 0) {
        CPendingQuery query;
        if (D3DDevice->CreateQuery(8, &query.Query) != 0) {
            FinishedFrame = PendingFrame - 2;
            return;
        }
        FreeQueoryObjects.push_back(query);
    }
    CPendingQuery query = FreeQueoryObjects.back();
    FreeQueoryObjects.pop_back();
    query.Query->Issue(1);
    PendingQueories.push_front(query);
    ++PendingFrame;
}

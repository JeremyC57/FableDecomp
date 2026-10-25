// std::_STLP_alloc_proxy::_STLP_alloc_proxy(const allocator& __a, _List_node* __p) -- STLport allocator proxy constructor of the list<CVsyncCallbackTimer::CPendingQuery> node pointer: copy the (empty) allocator and store the node pointer
// Leaf: ??0?$_STLP_alloc_proxy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F77B0, length 24 bytes, thiscall, `ret 8`, returns this.
// Label correction: the chunk label "__EH_epilog3" is propagated garbage; the body stores its second argument at +0 and
//   returns `this` -- the out-of-line _STLP_alloc_proxy(const _MaybeReboundAlloc&, _Value) the _List_base ctor inlines.
// TU: the CVsyncCallbackTimer module (0x009F7020..0x009F7F40) is compiled UNOPTIMISED (`#pragma optimize("", off)`: ebp frame,
//   `this` and every temporary homed in the frame) while inline expansion stays on (/Ob of the /O2 build): STLport's in-class
//   list members are expanded and each candidate expansion reserves its own frame slots, even where VC7.1 declines it
//   (`_M_create_node` in insert, size()/pop_back() in ClearAllQueries). The non-template iterator base (_List_iterator_base:
//   ctor 0x0041C780, _M_incr 0x0041C7A0, _M_decr 0x0041C7B0, operator!= 0x0041C7C0) is optimised code shared by /OPT:ICF, so it
//   is declared only. /Od frame slots are ordered by the locals' NAMES, so STLport's own identifiers (__tmp, __n, __next_node,
//   __prev_node, __result, __it, __first, ...) are kept verbatim. Retail STLport list = one node pointer (4 bytes); node =
//   {_M_next, _M_prev, _M_data}; CVsyncCallbackTimer::CPendingQuery (Ego_r.pdb, 16 bytes) = {Query, Frame, double TimeStamp}.
// Pragma: #pragma optimize("", off).
#pragma optimize("", off)
extern "C" void __cdecl free(void* block);                         // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E
void* __cdecl operator new(unsigned int size, void* where);        // 0x00405C10 (placement new, called out of line)
namespace std {

struct input_iterator_tag { input_iterator_tag() {} };
struct bidirectional_iterator_tag : input_iterator_tag { bidirectional_iterator_tag() {} };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {                                        // compiled optimised elsewhere (ICF-shared bodies)
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x);                      // 0x0041C780
    void _M_incr();                                                 // 0x0041C7A0
    void _M_decr();                                                 // 0x0041C7B0
    bool operator!=(const _List_iterator_base& __y) const;          // 0x0041C7C0
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
    _List_iterator& operator--() { _M_decr(); return *this; }
};

template <class _InputIterator>
inline int __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}

template <class _InputIterator>
inline int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, bidirectional_iterator_tag());
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

class __malloc_alloc {
public:
    static void* allocate(unsigned int __n) { void* __result = malloc(__n); return __result; }
};

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)__malloc_alloc::allocate(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    void clear();
};

template <class _Tp, class _Alloc> class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __tmp = this->_M_node.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        return __tmp;
    }
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    unsigned int size() const
    {
        unsigned int __result = distance(begin(), end());
        return __result;
    }
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
};

template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur->_M_next;
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

}

struct IDirect3DQuery9 {
    virtual long __stdcall QueryInterface(const void* riid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {
        IDirect3DQuery9* Query;
        long             Frame;
        double           TimeStamp;
    };
    void* Device;                                                       // +0x00 IDirect3DDevice9*
    std::list<CPendingQuery, std::allocator<CPendingQuery> > FreeQueoryObjects;   // +0x04
    std::list<CPendingQuery, std::allocator<CPendingQuery> > PendingQueories;     // +0x08
    long PendingFrame;                                                  // +0x0c
    long FinishedFrame;                                                 // +0x10
    void ClearAllQueries();
    void OnPreDeviceReset();
};

template class std::_STLP_alloc_proxy<std::_List_node<CVsyncCallbackTimer::CPendingQuery>*, std::_List_node<CVsyncCallbackTimer::CPendingQuery>,
    std::allocator<std::_List_node<CVsyncCallbackTimer::CPendingQuery> > >;
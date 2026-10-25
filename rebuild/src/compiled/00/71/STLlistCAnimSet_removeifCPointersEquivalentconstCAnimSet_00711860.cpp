// _STL::list<CAnimSet>::remove_if<CPointersEquivalent<const CAnimSet> >(pred) -- remove the one animation set whose address the predicate holds (destroying it and freeing its node)
// Leaf: ??$remove_if@V?$CPointersEquivalent@$$CBVCAnimSet@@@@@?$list@VCAnimSet@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711860, 91 bytes, `__fastcall`, the 4-byte predicate by value on the
//   stack, `ret 4`.
// Identity: FableWin.pdb ??$remove_if@V?$CPointersEquivalent@$$CBVCAnimSet@@@@@?$list@VCAnimSet@@...@std@@QAEXV?$CPointersEquivalent@$$CBVCAnimSet@@@@@Z
//   and ??R?$CPointersEquivalent@$$CBVCAnimSet@@@@QBE_NABVCAnimSet@@@Z; Ego_r.pdb CPointersEquivalent<const CAnimSet>
//   {const CAnimSet* P} (4 bytes). The element destructor frees two vector buffers at data+0x10 and data+4: Ego_r.pdb
//   CAnimSet {EAnimUpdateMode UpdateMode; vector<short> Anims; vector<CTransitionBlendAlpha> InternalAlphas; ...}
//   with retail 12-byte vectors. Callers 0x0070D8EC, 0x0070D8F5 and a tail jump at 0x00711B60.
// Callees: free @ 0x00BFEA14 (the two vector buffers, null-tested, then the node).
// Shape: STLport 4.6 `remove_if`: `while (first != last) { next = first; ++next; if (pred(*first)) erase(first);
//   first = next; }` with erase and CAnimSet's implicit destructor inline (InternalAlphas then Anims); the predicate
//   is `P == &x` (re-read from its argument slot each pass).
extern "C" void __cdecl free(void* block);               // 0x00BFEA14 (import thunk)

template <class T> class CVector {                       // STLport vector of trivially destructible T, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CVector() { if (_M_start != 0) free(_M_start); }
};
class CTransitionBlendAlpha { public: float CurrentAlpha, LastAlpha; };
class CAnimateThing;

class CAnimSet {                                         // retail 84 bytes
public:
    long                           UpdateMode;           // +0x00 EAnimUpdateMode
    CVector<short>                 Anims;                // +0x04
    CVector<CTransitionBlendAlpha> InternalAlphas;       // +0x10
    float                          CurrentPercentageComplete;   // +0x1c
    float                          LastPercentageComplete;
    float                          Speed;
    unsigned long                  FrameInternalAlphasUpdated;
    float                          CurrentAlpha;
    float                          LastAlpha;
    float                          LastUpdateChange;
    float                          ChangeAlphaBy;
    CAnimateThing*                 PAnimator;
    unsigned long                  Flags;
    unsigned long                  TypeFlags;
    long                           AnimDelay;
    long                           Priority;
    unsigned char                  Type;
    unsigned char                  BitFlags;             // Looping, Finished, Forwards, HadUpdate, OwnedByMainAnim
};

template <class T> class CPointersEquivalent {           // 4 bytes
public:
    T* P;                                                // +0x00
    CPointersEquivalent(T* p) : P(p) { }
    bool operator()(const T& x) const { return P == &x; }
};

namespace _STL {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }   // malloc-backed node allocator: no null test
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 the header node
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first)) erase(__first);
            __first = __next;
        }
    }
};
}

template void _STL::list<CAnimSet>::remove_if<CPointersEquivalent<const CAnimSet> >(CPointersEquivalent<const CAnimSet>);
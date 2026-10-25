// std::list::remove_if(CIsQuestCardWithDefName pred) -- STLport list<CQuestManager::CQuestCard>: erase every quest card whose def name equals the predicate's string
// Leaf: ??$remove_if@VCIsQuestCardWithDefName@@@?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9250, 117 bytes (+ int3 padding), __fastcall `ret 4` (the 4-byte predicate by value).
// Identity: STLport 4.6 member-template list::remove_if with the inline single-node erase, as its FeatIndex twin 0x004B91A0.
//   Ego_r.pdb CIsQuestCardWithDefName { CCharString* DefName; }; CQuestCard as there (DefName +0 = node +8).
// Callees: Compare(const char*, const char*) @ 0x00411570 (__fastcall, the inline CCharString operator==),
//   CCharString::~CCharString @ 0x0099EAE0 (ScriptName, then DefName), free @ 0x00BFEA14.
// Shape: CCharString::operator== inline (same block -> equal; both blocks with the same length -> `Compare(...) ? false : true`,
//   the 8-bit `neg ; sbb al,al ; inc al`); with ebp holding the predicate's string the end node is spilled to a frame slot.
//   Compare's real body (landed 0x00411570) sits above under `#pragma auto_inline(off)`: VC7.1 then knows the call writes no
//   memory and reuses the `next` it loaded before the test for the unlink (declared only, the erase re-reads both links).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

struct CCharStringData {                                // the shared string block
    const char*   m_pData;                              // +0x00
    unsigned long m_length;                             // +0x04
};

#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)   // 0x00411570 (the landed body; see Shape)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString {
public:
    CCharStringData* PData;
    ~CCharString();                                     // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PData == other.PData)
            return true;
        if (PData && other.PData && PData->m_length == other.PData->m_length)
            return Compare(PData->m_pData, other.PData->m_pData) ? false : true;
        return false;
    }
};

class CQuestManager {
public:
    class CQuestCard {                                  // 16 bytes (Ego_r.pdb)
    public:
        CCharString DefName;                            // +0x00
        CCharString ScriptName;                         // +0x04
        bool        Replayable;                         // +0x08
        long        FeatIndex;                          // +0x0C
    };
};

class CIsQuestCardWithDefName {
public:
    const CCharString* DefName;                         // +0x00
    bool operator()(const CQuestManager::CQuestCard& card) const { return card.DefName == *DefName; }
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator(__next_node);
    }

    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};

template void list<CQuestManager::CQuestCard>::remove_if<CIsQuestCardWithDefName>(CIsQuestCardWithDefName);

} // namespace std
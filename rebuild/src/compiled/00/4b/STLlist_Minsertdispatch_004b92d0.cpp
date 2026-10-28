// std::list::_M_insert_dispatch(list<CCountedPointer<CQuestManager::CActiveQuest> >::iterator pos, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert of the active-quest list: for every counted quest pointer in [first, last) allocate a node, copy the pointer (adding a reference) and link it in before `pos`
// Instantiation: std::list<CCountedPointer<CQuestManager::CActiveQuest> >::_M_insert_dispatch
// Leaf: ??$_M_insert_dispatch@U?$_List_iterator@V?$CCountedPointer@VCActiveQuest@CQuestManager@@@@U?$_Const_traits@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B92D0, 78 bytes (+ int3 padding), thiscall (`this` unused: the allocator
//   is stateless), `ret 10h` (pos, first, last, the tag reference).
// Identity: the active-quest list's range insert, reached three ways: list::insert(pos, first, last) @ 0x004B9EB0, the
//   list copy constructor @ 0x004BADC0 and CQuestManager @ 0x004B2AD0, which copies CQuestManager::ActiveQuests (retail
//   +0x38) into a local list. Ego_r.pdb: `list<CCountedPointer<CQuestManager::CActiveQuest> >` is CQuestManager's
//   ActiveQuests; one instantiation serves the copy constructor, so the input iterator is the const_iterator. The
//   catalog name `std::list_Insert_Range` is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator<_List_node<..> >::allocate(1), 16-byte node).
// Layout: node { next +0, prev +4, CCountedPointer<CActiveQuest> { Data +8, Info +0xc } }; CCPPointerInfo::RefCount +0.
// Shape: STLport 4.6 `_M_insert_dispatch(pos, first, last, const __false_type&)` = `for (; first != last; ++first)
//   insert(pos, *first);` with insert / _M_create_node / _Construct inlined: the placement new is null-tested on
//   `&node->_M_data`, the counted pointer's copy constructor bumps RefCount when Info is set, then the four link stores.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCPPointerInfo { public:                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class CQuestManager {
public:
    class CActiveQuest;
};

namespace std {
struct __false_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    explicit _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;  // +0x0 sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    public:
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
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

typedef CCountedPointer<CQuestManager::CActiveQuest> _QuestPtr;
template void list<_QuestPtr, allocator<_QuestPtr> >::_M_insert_dispatch(
    list<_QuestPtr, allocator<_QuestPtr> >::iterator, list<_QuestPtr, allocator<_QuestPtr> >::const_iterator,
    list<_QuestPtr, allocator<_QuestPtr> >::const_iterator, const __false_type&);
}
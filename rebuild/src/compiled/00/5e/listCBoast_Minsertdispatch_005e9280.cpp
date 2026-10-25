// list<CBoast>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert of quest boasts: copy every element of [first, last) into a new node linked in before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9280, 146 bytes (+ padding to 160), `ret 10h` (__fastcall: the three
//   iterators by value and the tag reference; `this` unused -- the malloc allocator is stateless).
// Element: CQuestManager::CBoast (Ego_r.pdb, 0x20 bytes at node+8; see 0x005E90B0); the list is
//   CTCQuestCompletionUI::Boasts.
// Callees: 0x00BFEA0E malloc(0x28), 0x0099EC30 CCharString copy ctor, 0x0099B720 CWideString copy ctor (twice).
// Callers: list<CBoast>::operator= 0x005E9630 (the `first1 == last1` arm, through the inline range insert), 0x005E9608.
// Shape: STLport `for (; first != last; ++first) insert(pos, *first);` with insert(pos, x) and _M_create_node inline:
//   malloc through allocator::allocate(1), the null-checked placement copy at node+8 (the element's implicit copy
//   constructor), then the STLport 4.6 link with `p = n->_M_prev` read into a named local first (VC then stores
//   tmp->prev before tmp->next, as retail; the 4.0 `pos._M_node->_M_prev` re-reads give the opposite order); the position
//   node is hoisted into ebp, `last` is re-read from its argument slot each pass.
// Label correction: catalogued as `std_List_CloneNodeRange`; the body is STLport `list::_M_insert_dispatch`.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    CWideString& operator=(const CWideString& other);      // 0x0099B7D0
};

class CQuestManager {
public:
    struct CBoast {                                        // Ego_r.pdb, 0x20 bytes; implicit copy / assign / destroy
        long        ID;                                    // +0x00
        long        Cost;                                  // +0x04
        long        Reward;                                // +0x08
        CCharString ScriptName;                            // +0x0c
        CWideString Text;                                  // +0x10
        CWideString Title;                                 // +0x14
        bool        Evil;                                  // +0x18
        bool        Failed;                                // +0x19
        bool        Completed;                             // +0x1a
        long        GroupIndex;                            // +0x1c
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator {                       // STLport allocator over malloc / free
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { free(p); }
};

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x00
    _List_node_base* _M_prev;                              // +0x04
};

template <class T> struct _List_node : _List_node_base {
    T _M_data;                                             // +0x08
};

struct _List_iterator_base {                              // implicit (trivial) copy
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};

// STLport 4.x: `_List_iterator(const iterator&)` is the USER copy constructor of iterator (built in place at esp when
// passed by value) but only a converting constructor of const_iterator, whose implicit copy is a plain `push`.
template <class T, class Ref> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, T&>  iterator;
    typedef _List_iterator<T, Ref> _Self;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    Ref operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _Self& operator++() { _M_incr(); return *this; }
    _Self operator++(int) { _Self tmp = *this; _M_incr(); return tmp; }
};

template <class T> class list {
public:
    typedef _List_node<T>                _Node;
    typedef _List_iterator<T, T&>        iterator;
    typedef _List_iterator<T, const T&>  const_iterator;

    _List_node_base* _M_node;                              // +0x00 the allocated sentinel

    iterator begin() { return _M_node->_M_next; }
    iterator end() { return _M_node; }
    const_iterator begin() const { return _M_node->_M_next; }
    const_iterator end() const { return _M_node; }

    _Node* _M_create_node(const T& x)
    {
        _Node* p = allocator<_Node>().allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node_base* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    template <class InputIter> void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&);
    template <class InputIter> void insert(iterator pos, InputIter first, InputIter last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        _Destroy(&n->_M_data);
        allocator<_Node>().deallocate(n, 1);
        return iterator(next_node);
    }
    iterator erase(iterator first, iterator last);
    list& operator=(const list& x);
};

} // namespace std

namespace std {
template <class T> template <class InputIter>
void list<T>::_M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&)
{
    for (; first != last; ++first)
        insert(pos, *first);
}
} // namespace std

template void std::list<CQuestManager::CBoast>::_M_insert_dispatch<std::list<CQuestManager::CBoast>::const_iterator>(
    std::list<CQuestManager::CBoast>::iterator, std::list<CQuestManager::CBoast>::const_iterator,
    std::list<CQuestManager::CBoast>::const_iterator, const std::__false_type&);
// list<CBoast>::operator=(const list& x) -- STLport list assignment of quest boasts: assign element-wise over the common prefix, then erase this list's surplus or insert x's remainder
// Leaf: ??4?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9630, 224 bytes, `ret 4` (__fastcall), returns this.
// Element: CQuestManager::CBoast (Ego_r.pdb, 0x20 bytes at node+8; see 0x005E90B0); the list is
//   CTCQuestCompletionUI::Boasts (copied from the quest manager's boast list in CTCQuestCompletionUI::Activate 0x005E4430).
// Callees: 0x0099EFB0 CCharString::operator=, 0x0099B7D0 CWideString::operator= (twice), 0x005E90B0
//   erase(first, last), 0x005E9280 _M_insert_dispatch(pos, first, last, __false_type).
// Shape: STLport `if (this != &x) { first1 = begin(); last1 = end(); first2 = x.begin(); last2 = x.end();
//   while (first1 != last1 && first2 != last2) *first1++ = *first2++; if (first2 == last2) erase(first1, last1);
//   else insert(last1, first2, last2); } return *this;` -- the element's implicit operator= inline; the by-value iterator
//   arguments are built in place at esp (iterator's user copy ctor) while the const_iterators are plain pushes (the
//   STLport `_List_iterator(const iterator&)` is only a converting ctor for them); erase's discarded result
//   and the range insert's __false_type tag are homed in x's
//   argument slot.
// Label correction: catalogued as `std_List_Swap`; the body is STLport `list::operator=`.
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
template <class T> list<T>& list<T>::operator=(const list<T>& x)
{
    if (this != &x) {
        iterator first1 = begin();
        iterator last1 = end();
        const_iterator first2 = x.begin();
        const_iterator last2 = x.end();
        while (first1 != last1 && first2 != last2)
            *first1++ = *first2++;
        if (first2 == last2)
            erase(first1, last1);
        else
            insert(last1, first2, last2);
    }
    return *this;
}
} // namespace std

template class std::list<CQuestManager::CBoast>;
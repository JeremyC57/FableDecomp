// _STL::list<CScriptThing>::push_back(const CScriptThing& x) -- append a copy of a script thing at the end of the list
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA01E0, 30 bytes, `__fastcall`, one stack argument (`ret 4`).
//   Callers: the minion-camp entity script's Main @ 0x00DA0160 (twice: ParentClass->LeftSpawnPoints +0x90 and
//   RightSpawnPoints +0x94 of CQ_MinionCampScript, both list<CScriptThing>).
// Label correction: the catalog "NScript_CDanyalTestScriptsData_InsertHead" is descriptive and wrong (it appends at end()).
// Callees: list<CScriptThing>::_M_create_node(const CScriptThing&) @ 0x00DA0200 (out of line in this TU).
// Shape: STLport `push_back(x) { insert(end(), x); }` with insert inline: end() (the header node) is read into esi
//   before the node is created, then tmp->next = n, tmp->prev = p, p->next = tmp, n->prev = tmp.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CScriptThing {                                        // 12 bytes, vtable 0x01238C8C
public:
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
};

namespace _STL {
class __malloc_alloc {                                      // Fable's STLport node allocator: malloc
public:
    static void* allocate(unsigned int n) { return malloc(n); }
};
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)__malloc_alloc::allocate(n * sizeof(T)) : 0; }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    Tp* allocate(unsigned int n) { return A::allocate(n); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base {
    _List_node_base* _M_next;                               // +0
    _List_node_base* _M_prev;                               // +4
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                              // +8
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) { }
};
template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // the header node pointer
};
template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _Node* _M_create_node(const T& x);                      // 0x00DA0200
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};
template class list<CScriptThing>;
} // namespace _STL
// _STL::list<CScriptThing>::_M_create_node(const CScriptThing& x) -- allocate a 0x14-byte list node and copy-construct the script thing into it
// Leaf: ?_M_create_node@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0200, 36 bytes, `__fastcall`, one stack argument (`ret 4`), returns the node.
//   Its only caller is list<CScriptThing>::push_back @ 0x00DA01E0 (CQ_MinionCampScript's LeftSpawnPoints +0x90 /
//   RightSpawnPoints +0x94, appended by a minion-camp entity script's Main @ 0x00DA0160).
// Label correction: the catalog "Alloc" (`__fastcall` CScriptDataBase*) is a propagated script-factory label.
// Callees: malloc @ 0x00BFEA0E (0x14 bytes: next, prev, CScriptThing), CScriptThing::CScriptThing(const CScriptThing&)
//   @ 0x004ABE90.
// Shape: STLport `_M_create_node`: `_M_node.allocate(1)` then `_Construct(&tmp->_M_data, x)`; the placement new tests the
//   element address (`lea ecx,[esi+8] ; test ecx,ecx`), the malloc result itself is not tested. Fable's STLport node
//   allocator is the malloc one (as in the tree/vector rows), not ::operator new @ 0x00BFEA1A. No EH frame (no /EHsc),
//   so the STLport _STLP_TRY/_STLP_UNWIND guard compiles away.
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
    _Node* _M_create_node(const T& x)
    {
        _Node* tmp = this->_M_node.allocate(1);
        _Construct(&tmp->_M_data, x);
        return tmp;
    }
};
template class list<CScriptThing>;
} // namespace _STL
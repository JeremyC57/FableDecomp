// pair<CThingBuilding* const, list<CThingAICreature*> >::pair(const pair& o) -- STLport pair copy constructor (out of line): copy the building key, then copy-construct the creature list (fresh sentinel + insert(begin(), o.begin(), o.end()))
// Leaf: ??0?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@
//
// Retail 0x00517590, 73 bytes (+ int3 padding to 80), thiscall `ret 4`.  Callees: 0x00BFEA0E malloc (the list's 0xC-byte sentinel node),
//   0x00513B60 list<CThingAICreature*>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last,
//   const __false_type&) (`ret 10h`).
// Callers: the map's _M_insert 0x0051AA00 (twice, building the 0x18-byte node's value) and 0x005190A5 / 0x00519C69.
// Identity: the map is keyed by a pointer (unsigned `jb` compare in 0x0051AA00) and maps to a list of 4-byte values;
//   Ego_r.pdb's one type of that shape is map<CThingBuilding*, list<CThingAICreature*> >, next to the landed
//   CIsHouseLargerThan (0x00512410) building code.
// Shape: STLport `pair(const pair& o) : first(o.first), second(o.second) {}` with list's copy constructor inline:
//   `_List_base(a)` (proxy zeroed, allocate(1) = malloc(0xC), sentinel self-linked) then `insert(begin(), x.begin(),
//   x.end())` forwarding to the out-of-line _M_insert_dispatch.  As in the landed 0x00A84470 recipe, _M_insert_dispatch
//   gets a same-TU __declspec(noinline) body so the empty `__false_type()` temporary overlays the dead argument slot
//   (`lea ebx,[esp+10h]`); iterator has a user copy ctor (pos is built in its slot through `mov ecx,esp`), and
//   const_iterator only a converting one (first/last are plain pushes).
// Label: catalog "DoublyLinkedListNode_CreateAndCopy" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CThingBuilding;
class CThingAICreature;

namespace std {
struct __false_type { __false_type() {} };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) {}
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) {}
};
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
    _List_base(const allocator<_List_node<T> >& a) : _M_node(a, (_List_node<T>*)0)
    {
        _List_node<T>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    allocator<_Node> get_allocator() const { return allocator<_Node>(); }
    _List_iterator begin() { return _List_iterator(this->_M_node._M_data->_M_next); }
    _List_const_iterator begin() const { return _List_const_iterator(this->_M_node._M_data->_M_next); }
    _List_const_iterator end() const { return _List_const_iterator(this->_M_node._M_data); }
    __declspec(noinline) void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
                                                 const __false_type&)   // 0x00513B60 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _Node* n = this->_M_node.allocate(1);
            new (&n->_M_data) T(((_Node*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(_List_iterator pos, _List_const_iterator first, _List_const_iterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list(const list<T>& x) : _List_base<T>(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}

template std::pair<CThingBuilding* const, std::list<CThingAICreature*> >::pair(const std::pair<CThingBuilding* const, std::list<CThingAICreature*> >&);
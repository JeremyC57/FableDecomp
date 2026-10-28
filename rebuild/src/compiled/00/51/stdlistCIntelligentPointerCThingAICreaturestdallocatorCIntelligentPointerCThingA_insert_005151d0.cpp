// std::list<CIntelligentPointer<CThingAICreature>,std::allocator<CIntelligentPointer<CThingAICreature> > >::insert(iterator position, const CIntelligentPointer<CThingAICreature>& x) -- STLport list insert: build a node holding a copy of the creature pointer and link it before position, return an iterator to it
// Leaf: ?insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005151D0, 79 bytes, `ret 0Ch` (thiscall: the hidden iterator result, position,
//   the value reference; `this` is never read -- the node allocator is empty).
// Caller: 0x006604FC. Element: CIntelligentPointer<CThingAICreature> (8 bytes: vptr + item), vtable 0x0123E7E0 -- the
//   landed CThingAICreature constructor 0x00831F80 names that vtable. Node = 0x10 bytes. Catalog label
//   "ListWrapper_InsertElement" is descriptive.
// Callees: malloc @ 0x00BFEA0E; CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::GetPItem() const
//   @ 0x00A01B50, ::SetPItem(CBaseObject*) @ 0x00A01B90.
// Shape: STLport `insert(pos, x) { _Node* tmp = _M_create_node(x); n = pos._M_node; p = n->_M_prev; tmp->_M_next = n;
//   tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp; return tmp; }`; _M_create_node's placement copy runs the
//   pointer's inline copy constructor (base ctor, derived vtable store, `SetPItem(x.GetPItem())`) behind the null
//   test; the iterator (user constructors) comes back through the hidden pointer.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CThingAICreature;
class CBaseObject;

class CBaseIntelligentPointer {                     // 8 bytes: vptr + the pointee link
public:
    virtual void v00();
    CBaseObject* PItem;                    // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
    void SetPItem(CBaseObject*);   // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <CThingAICreature>: vtable 0x0123E7E0
public:
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

namespace std {

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                       // +0
    _List_node_base* _M_prev;                       // +4
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                      // +8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _Nonconst_traits { typedef T& reference; };

template <class T, class Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
};

template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;    // +0
};

template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;

    protected:
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    public:
    iterator insert(iterator position, const T& x);
};

template <class T, class A>
typename list<T, A>::iterator list<T, A>::insert(iterator position, const T& x)
{
    _Node* tmp = _M_create_node(x);
    _List_node_base* n = position._M_node;
    _List_node_base* p = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = p;
    p->_M_next = tmp;
    n->_M_prev = tmp;
    return tmp;
}

template list<CIntelligentPointer<CThingAICreature> >::iterator list<CIntelligentPointer<CThingAICreature> >::insert(
    list<CIntelligentPointer<CThingAICreature> >::iterator position, const CIntelligentPointer<CThingAICreature>& x);

}
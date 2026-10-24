// back_insert_iterator<list<CIntelligentPointer<CThingAICreature>>>::operator=(const value_type& value) -- push a copy of the intelligent pointer onto the back of the list and return the iterator
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C1E70, 82 bytes (+ int3 padding), thiscall `ret 4`, returns `this`.
// Callers: none by direct call (xref finds no call site; the copy_if output iterator of the
//   CThingSearchTools::GetAllThings<CThingAICreature, list, CThingFilter_HasBuildingAsHomeOrWork> family -- Ego_r.pdb
//   names a copy_if with this back_insert_iterator; the GetAllThings row 0x007BB660 calls list::push_back 0x006604EB).
// Callees: malloc @ 0x00BFEA0E (the STLport node allocator), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
//   CBaseIntelligentPointer::GetPItem() @ 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) @ 0x00A01B90
//   (all landed); CIntelligentPointer<CThingAICreature> vtable @ 0x0123E7E0.
// Layout: back_insert_iterator {container} (+0); STLport list {_M_node._M_data = header node}; node {next, prev, data}
//   = 0x10 bytes; CIntelligentPointer<T> {vptr, PData} = 8 bytes.
// Shape: STLport 4.x `container->push_back(value); return *this;` with push_back = insert(end(), x): end() is read
//   before the node is created, `_M_create_node` = allocate(1) + placement copy (null test on &node->_M_data), then the
//   four link stores in STLport order. The CIntelligentPointer copy constructor is `CBaseIntelligentPointer()` +
//   `SetPItem(other.GetPItem())` through the TYPED inline wrappers `T* GetPItem() const` / `SetPItem(T*)`: calling the
//   base members directly swaps this/data between ebp and ebx (the extra inline layer raises the data pointer's weight).
// Label correction: the catalog `LinkedListNode_Create` is descriptive.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CBaseObjectPointer;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7E0
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CThingAICreature;

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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

class _List_node_base {
public:
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> class _List_node : public _List_node_base {
public:
    _Tp _M_data;
};

template <class _Tp> class _List_iterator {
public:
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return this->_M_node._M_data; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __val)
    {
        container->push_back(__val);
        return *this;
    }
};

template class back_insert_iterator<list<CIntelligentPointer<CThingAICreature> > >;

}
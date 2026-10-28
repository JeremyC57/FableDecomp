// std::list<CIntelligentPointer<CThing> >::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert: copy every thing pointer of [first, last) into a new node linked in before pos
// Leaf: ??$_M_insert_dispatch@U_List_const_iterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00618100, 98 bytes (+14 int3), thiscall `ret 10h` (this never read: the
//   node allocator is malloc).
// Callees: malloc 0x00BFEA0E (0x10-byte nodes: links + the 8-byte smart pointer), CBaseIntelligentPointer::
//   CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::GetPItem() const 0x00A01B50,
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90; vtable 0x01238C6C = CIntelligentPointer<CThing> (as in
//   the landed CGameScriptThing ctor 0x008CFF60 and CThingDeadCreature ctor 0x00835C20).
// Callers: twelve sites -- CTCInventoryMap screen builders (0x005FCC59, GetRegionsToDisplay 0x005FD4AA,
//   ConstructRegionDescriptionQuests 0x005FD629, 0x005FDE69, CreateMapMarkers 0x005FF325 / 0x005FF6DF,
//   BuildMapTeleportUI 0x0060CBFE, 0x0060F1C2), the list helpers 0x00618790 / 0x00618DA0 (calls at 0x00618848 /
//   0x00618DD3), CTCCampfire code 0x0077DCB0 and the bandit campfire AI state 0x0093ABFF (one shared COMDAT).
// Label correction: the catalog `Campfire_InsertRegionNode` is behavioural (named after one caller); the body is STLport
//   `_M_insert_dispatch(pos, first, last, __false_type)` of list<CIntelligentPointer<CThing> >.
// Shape: speed TU (no pragma), the landed list<CIntelligentPointer<CThingAICreature> > twin 0x00517130 byte for byte
//   but the vtable: `for (; first != last; ++first) insert(pos, *first);` with insert and _M_create_node inline (the
//   placement new null-tests node+8; the smart pointer's copy constructor = base ctor, own vtable,
//   SetPItem(other.GetPItem())), then the link with both neighbours cached.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseIntelligentPointer();                 // 0x00A01B10
    CBaseObject* GetPItem() const;             // 0x00A01B50
    void SetPItem(CBaseObject* item);          // 0x00A01B90
    void* PData;                               // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

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
    bool operator!=(const _List_const_iterator& y) const { return _M_node != y._M_node; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    protected:
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    public:
    _List_iterator insert(_List_iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    template <class _InputIter>
    void _M_insert_dispatch(_List_iterator pos, _InputIter first, _InputIter last, const __false_type&)
    {
        for (; first != last; ++first)
            insert(pos, ((_Node*)first._M_node)->_M_data);
    }
};
}

template void std::list<CIntelligentPointer<CThing> >::_M_insert_dispatch<std::_List_const_iterator>(
    std::_List_iterator, std::_List_const_iterator, std::_List_const_iterator, const std::__false_type&);
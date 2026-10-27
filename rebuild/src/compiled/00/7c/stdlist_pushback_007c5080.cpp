// std::list::push_back(const CIntelligentPointer<CThingCreatureBase>& __x) -- (list<CIntelligentPointer<CThingCreatureBase> >) append a copy of the pointer
// Leaf: ?push_back@?$list@V?$CIntelligentPointer@VCThingCreatureBase@@@@U?$allocator@V?$CIntelligentPointer@VCThingCreatureBase@@@@@std@@@std@@QAEXABV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C5080, 73 bytes, thiscall, `ret 4`.
// Identity (proven): the inline node copy stores vtable 0x0123E7D8, RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@ (the element); its caller is CTCHealLife::StartHealingEffect 0x007C4430.
// Callees: malloc 0x00BFEA0E (the node allocator), CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Shape (family M085, representative 0x004BA4E0): STLport `push_back(x) { insert(end(), x); }` with insert and _M_create_node
//   inline: allocate(1), `_Construct(&__p->_M_data, __x)` (placement null test), then next = end, prev = end->prev,
//   prev->next = tmp, end->prev = tmp; the iterator's user copy constructor (as the landed list push_back 0x004BA420)
//   fixes the sentinel's register, and the intelligent pointer's copy goes through its typed
//   GetPItem/SetPItem wrappers (the landed back_insert_iterator 0x006C1E70 model); speed TU.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CBaseObject;
class CThingCreatureBase;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                             // +0x0
    _List_node_base* _M_prev;                             // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                          // +0x8
};
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(unsigned int __n) { return static_cast<_Tp*>(malloc(__n * sizeof(_Tp))); }
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    __forceinline _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    __forceinline _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    struct : allocator<_Node> { _List_node_base* _M_data; } _M_node;   // _STLP_alloc_proxy: the sentinel
    __forceinline iterator end() { return iterator(_M_node._M_data); }
    __forceinline _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    __forceinline iterator insert(iterator __position, const _Tp& __x)
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
    void push_back(const _Tp& __x);
};
template <class _Tp, class _Alloc> void list<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    insert(end(), __x);
}
template void list<CIntelligentPointer<CThingCreatureBase> >::push_back(const CIntelligentPointer<CThingCreatureBase>& __x);
}
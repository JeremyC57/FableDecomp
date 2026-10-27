// std::list::insert(iterator __position, const pair<CIntelligentPointer<CThing>, long>& __x) -- (list<pair<CIntelligentPointer<CThing>, long> >) insert a copy before position
// Leaf: ?insert@?$list@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@U?$allocator@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@@2@@std@@QAE?AU?$_List_iterator@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@@2@U32@ABU?$pair@V?$CIntelligentPointer@VCThing@@@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810C20, 89 bytes, thiscall, `ret 0Ch` (the iterator by hidden pointer, position, x).
// Identity (proven): bound ?insert@?$list@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@@std@@...; its caller is that list's push_back 0x00810D40 in the CTCDExplosiveTrail TU (Ego_r.pdb CTCDExplosiveTrail::ComponentTraps is list<pair<CIntelligentPointer<CThing>, long> >).
// Callees: malloc 0x00BFEA0E (the 0x14-byte node), CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Shape (family M092, representative 0x004CCC80): STLport 4.6 list::insert with _M_create_node inline: allocate(1), the
//   placement copy under its null test (STLport pair's copy constructor: the intelligent pointer through its typed
//   GetPItem/SetPItem, then the dword second), then the four link stores and the iterator result; speed TU.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace std {
template <class _T1, class _T2> struct pair {
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    _T1 first;
    _T2 second;
};
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
    __forceinline _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x);
};
template <class _Tp, class _Alloc> typename list<_Tp, _Alloc>::iterator list<_Tp, _Alloc>::insert(iterator __position, const _Tp& __x)
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
typedef pair<CIntelligentPointer<CThing>, long> _E;
template list<_E>::iterator list<_E>::insert(list<_E>::iterator, const _E&);
}
// std::list::push_back(const CWitnessesSearchingDeed& __x) -- (list<CWitnessesSearchingDeed>) append a copy of the record
// Leaf: ?push_back@?$list@VCWitnessesSearchingDeed@@U?$allocator@VCWitnessesSearchingDeed@@@std@@@std@@QAEXABVCWitnessesSearchingDeed@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C91E0, 51 bytes, thiscall, `ret 4`.
// Identity (proven): the copy constructor 0x006C5460 is the landed CWitnessesSearchingDeed(const CWitnessesSearchingDeed&) (0x24-byte node); its caller is CTCHeroOpinionDeedLog::PostDeed 0x006C6990 (Ego_r.pdb CTCHeroOpinionDeedLog::OpinionDeedsSearchingForWitnesses is list<CWitnessesSearchingDeed>).
// Callees: malloc 0x00BFEA0E (the node allocator), CWitnessesSearchingDeed::CWitnessesSearchingDeed(const CWitnessesSearchingDeed&) 0x006C5460.
// Shape (family M086, representative 0x004BA550): the landed list push_back 0x004BA420 model -- `insert(end(), x)` with insert
//   and _M_create_node inline: allocate(1), the placement copy (null test), then the four link stores; speed TU.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CWitnessesSearchingDeed {                           // Ego_r.pdb, 0x1C bytes
public:
    CWitnessesSearchingDeed(const CWitnessesSearchingDeed& other);   // 0x006C5460
    unsigned long Members[7];
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
template void list<CWitnessesSearchingDeed>::push_back(const CWitnessesSearchingDeed& __x);
}
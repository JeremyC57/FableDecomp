// std::list::_M_create_node(const CStringParserCommentBoundaries& __x) -- (list<CStringParserCommentBoundaries>) allocate a node and copy-construct x into it
// Leaf: ?_M_create_node@?$list@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAEPAU?$_List_node@VCStringParserCommentBoundaries@@@2@ABVCStringParserCommentBoundaries@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00463901, 34 bytes, thiscall (this unused), `ret 4` (size-optimised module).
// Identity (proven): its caller is the landed list<CStringParserCommentBoundaries>::_M_insert_dispatch 0x004638C8 (the CStringParser copy constructor's comment-boundary list).
// Callees: malloc 0x00BFEA0E (the node allocator).
// Shape (family M047, representative 0x00463901): STLport `__p = _M_node.allocate(1); _Construct(&__p->_M_data, __x);
//   return __p;` with allocate/_Construct `__forceinline`; no null test of the block, the placement copy's test on node+8.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CStringParserCommentBoundaries { public: char* PStart; char* PEnd; };   // Ego_r.pdb, 8 bytes
namespace std {
template <class _Tp> class allocator { public: allocator() { } __forceinline _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
public:
    struct _Proxy : public allocator<_List_node<_Tp> > { _List_node<_Tp>* _M_data; } _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
};
template class list<CStringParserCommentBoundaries >;
}
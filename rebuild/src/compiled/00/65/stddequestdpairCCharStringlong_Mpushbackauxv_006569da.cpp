// std::deque<std::pair<CCharString,long> >::_M_push_back_aux_v(const value_type& t) -- push_back slow path: copy t, make room for one more map slot, allocate the next block, construct t at the old finish and move finish into the new block
// Leaf: _M_push_back_aux_v
// Retail: Fable.exe (TLC, VC7.1) @ 0x006569DA, 102 bytes, thiscall `ret 4`. Only caller: deque::push_back @ 0x00656E0C
// (call at 0x00656E35), the message queue of NPlayerGui::CDrawGuildSeal (Ego_r.pdb: Messages is a
// queue<pair<CCharString,long>> over deque<pair<CCharString,long>>; landed AddMessage 0x0064E0DA pushes into it).
// Callees: pair<CCharString,long> copy constructor @ 0x0050B7B0 (out of line: CCharString copy then the long; manifest
// "CComboMultiplierData" is an identical-body label), _M_reserve_map_at_back(1) @ 0x0065676F (out of line; manifest
// "CTCUserActionPointBase::GetNumberOfActionPointsInUse" is propagated), malloc @ 0x00BFEA0E (one 128-byte block:
// 16 elements of 8 bytes, the allocate's n != 0 test folded away), ~CCharString @ 0x0099EAE0 (t_copy's implicit pair
// destructor).
// Label correction: the catalog "CTCUserActionPointBase_InsertActionPoint" is propagated; this is STLport 4.x
// `deque::_M_push_back_aux_v`.
// Shape: STLport verbatim without `_STLP_TRY`: `value_type __t_copy = __t;` in the frame, reserve, allocate the next
// node, `_Copy_Construct` at finish.cur (placement null test), `_M_set_node(node + 1)`, cur = first.
// Layout: deque _M_start +0x00, _M_finish +0x10 ({cur, first, last, node}), _M_map +0x20, _M_map_size +0x24.
// Opt: size TU (`optimize("s", on)`; the frame comes from the t_copy local).
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(size_t, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
    pair(const pair& __p);                          // 0x0050B7B0 (out of line in retail)
};

__forceinline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }

template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> struct _Deque_iterator {
    typedef _Tp** _Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    _Tp*         _M_cur;                                 // +0x00
    _Tp*         _M_first;                               // +0x04
    _Tp*         _M_last;                                // +0x08
    _Map_pointer _M_node;                                // +0x0c
    __forceinline void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + ptrdiff_t(_S_buffer_size());
    }
};

template <class _Tp> struct _Deque_base {
    static size_t buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    _Deque_iterator<_Tp>                                 _M_start;      // +0x00
    _Deque_iterator<_Tp>                                 _M_finish;     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> >     _M_map;        // +0x20
    _STLP_alloc_proxy<size_t, _Tp, allocator<_Tp> >      _M_map_size;   // +0x24
};

template <class _Tp> struct deque : public _Deque_base<_Tp> {
    typedef _Tp value_type;
    void _M_reserve_map_at_back(size_t __nodes_to_add = 1);            // 0x0065676F (out of line in retail)
    void _M_push_back_aux_v(const value_type& __t);
};

template <class _Tp>
void deque<_Tp>::_M_push_back_aux_v(const value_type& __t)
{
    value_type __t_copy = __t;
    _M_reserve_map_at_back();
    *(this->_M_finish._M_node + 1) = this->_M_map_size.allocate(this->buffer_size());
    _Copy_Construct(this->_M_finish._M_cur, __t_copy);
    this->_M_finish._M_set_node(this->_M_finish._M_node + 1);
    this->_M_finish._M_cur = this->_M_finish._M_first;
}

template void deque<pair<CCharString, long> >::_M_push_back_aux_v(const pair<CCharString, long>&);
}
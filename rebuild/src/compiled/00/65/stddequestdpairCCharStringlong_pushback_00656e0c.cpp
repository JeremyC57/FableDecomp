// std::deque<std::pair<CCharString,long> >::push_back(const value_type& t) -- construct t at the finish cursor when the current block has room, else take the _M_push_back_aux_v path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) @ 0x00656E0C, 50 bytes, thiscall `ret 4`. Callers: NPlayerGui::CDrawGuildSeal::AddMessage
// (landed 0x0064E0DA, call at 0x0064E1AA: the Messages queue's push) and a jmp at 0x00656F2A.
// Callees: pair<CCharString,long> copy constructor @ 0x0050B7B0 (placement copy at finish.cur), _M_push_back_aux_v
// @ 0x006569DA (the block-full path).
// Label correction: the catalog "ComboMultiplierData_ConstructOrAppend" is propagated; this is STLport 4.x
// `deque<pair<CCharString,long>>::push_back`.
// Shape: STLport verbatim: `if (finish.cur != finish.last - 1) { _Copy_Construct(finish.cur, t); ++finish.cur; }
// else _M_push_back_aux_v(t);` -- `last - 1` is the `sub eax,8` on the loaded last pointer.
// Opt: size TU (`optimize("s", on)`; no frame-pointer pragma: with `y` off the lane adds `push ebp`).
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
    protected:
    void _M_reserve_map_at_back(size_t __nodes_to_add = 1);            // 0x0065676F (out of line in retail)
    public:
    protected:
    void _M_push_back_aux_v(const value_type& __t);                    // 0x006569DA
    public:
    void push_back(const value_type& __t)
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_last - 1) {
            _Copy_Construct(this->_M_finish._M_cur, __t);
            ++this->_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(__t);
    }
};

template void deque<pair<CCharString, long> >::push_back(const pair<CCharString, long>&);
}
// _STL::deque<CSetTextInfo>::_M_push_back_aux_v(const CSetTextInfo&) -- STLport deque push_back slow path for the subtitle text-info queue: copy the value, reserve a map slot at the back, allocate a new 4-record node, construct the copy at the old finish and step the finish iterator into the new node
// Leaf: _M_push_back_aux_v
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00494106, 98 bytes, `ret 4` (__fastcall; size-optimised TU with an ebp
//   frame and `leave`: `#pragma optimize("s", on)`; the 28-byte value copy is the only local).
// Callees: 0x004935A7 CSetTextInfo copy constructor (landed), 0x00493FA8 _M_reserve_map_at_back (out of line in this TU,
//   `push 1`), 0x00BFEA0E malloc (STLport malloc allocator; buffer_size = 128 / 28 = 4 records = 0x70 bytes, folded),
//   0x0043E3D0 ~CSetTextInfo (four CWideString destructors).
// Callers: deque::push_back 0x004941C0 (this chunk) from the CSubtitleRenderer text-info queue
//   (`queue<CSubtitleRenderer::CSetTextInfo, deque<...> > TextInfoQueue`).
// Shape: STLport 4.x `_M_push_back_aux_v`: `value_type __t_copy = __t; _M_reserve_map_at_back();
//   *(_M_finish._M_node + 1) = _M_map_size.allocate(buffer_size()); _Construct(_M_finish._M_cur, __t_copy);
//   _M_finish._M_set_node(_M_finish._M_node + 1); _M_finish._M_cur = _M_finish._M_first;` with `_M_set_node` written
//   as STLport's chained `_M_last = (_M_first = *(_M_node = __new_node)) + buffer_size()`; the try/unwind is compiled
//   out (no EH in this TU). STLport deque layout: _M_start +0x00, _M_finish +0x10 {cur, first, last, node}, _M_map +0x20,
//   _M_map_size +0x24 (the Ego_r.pdb 0x18-byte Dinkumware deque is the debug build's).
// Label correction: catalogued as `CTCUserActionPointBase_CreateTextInfo`; the body is the STLport deque slow push of
//   CSetTextInfo (byte layout of the landed CSetTextInfo copy constructor 0x004935A7).
#pragma optimize("s", on)
#include <stdlib.h>                                 // malloc @ 0x00BFEA0E
#include <new>

struct CWideString {
    void* m_pStringData;
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
};

struct CSetTextInfo {
    CWideString   Text;                             // +0x00
    CWideString   Button1Text;                      // +0x04
    CWideString   Button2Text;                      // +0x08
    CWideString   Button3Text;                      // +0x0C
    unsigned long DisplayFlags;                     // +0x10
    float         DisplayTime;                      // +0x14
    bool          UpdateWhilePaused;                // +0x18
    bool          ChangePlayerMode;                 // +0x19
    CSetTextInfo(const CSetTextInfo& other);        // 0x004935A7
    ~CSetTextInfo();                                // 0x0043E3D0
};

template <class _Tp>
struct allocator {
    allocator() { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

inline unsigned int __deque_buf_size(unsigned int __size) { return __size < 128 ? (128 / __size) : 1; }

struct _Deque_iterator_CSetTextInfo {
    CSetTextInfo*  _M_cur;                          // +0x00
    CSetTextInfo*  _M_first;                        // +0x04
    CSetTextInfo*  _M_last;                         // +0x08
    CSetTextInfo** _M_node;                         // +0x0C
    static unsigned int buffer_size() { return __deque_buf_size(sizeof(CSetTextInfo)); }
    void _M_set_node(CSetTextInfo** __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(buffer_size());
    }
};

struct CSetTextInfoDeque {
    _Deque_iterator_CSetTextInfo _M_start;                                                       // +0x00
    _Deque_iterator_CSetTextInfo _M_finish;                                                      // +0x10
    _STLP_alloc_proxy<CSetTextInfo**, CSetTextInfo*, allocator<CSetTextInfo*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, CSetTextInfo, allocator<CSetTextInfo> > _M_map_size;           // +0x24

    static unsigned int buffer_size() { return __deque_buf_size(sizeof(CSetTextInfo)); }
    void _M_reserve_map_at_back(unsigned int __nodes_to_add = 1);                                 // 0x00493FA8
    void _M_push_back_aux_v(const CSetTextInfo& __t);
};

void CSetTextInfoDeque::_M_push_back_aux_v(const CSetTextInfo& __t)
{
    CSetTextInfo __t_copy = __t;
    _M_reserve_map_at_back();
    *(_M_finish._M_node + 1) = _M_map_size.allocate(buffer_size());
    new (_M_finish._M_cur) CSetTextInfo(__t_copy);
    _M_finish._M_set_node(_M_finish._M_node + 1);
    _M_finish._M_cur = _M_finish._M_first;
}
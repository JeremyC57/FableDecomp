// std::deque<CSetTextInfo>::push_back(const CSetTextInfo&) -- STLport deque push_back for the subtitle text-info queue: copy-construct at the finish cursor when the finish node has room for more than this record, else the slow path _M_push_back_aux_v @ 0x00494106
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004941C0, 50 bytes, `ret 4` (__fastcall; size-optimised TU:
//   `#pragma optimize("s", on)`; the reference argument is re-pushed from its slot, `push dword ptr [esp+8]`).
// Callees: 0x004935A7 CSetTextInfo copy constructor (landed; placement new, hence the null test on the cursor),
//   0x00494106 _M_push_back_aux_v (this chunk).
// Callers: 0x004929B0 (CSubtitleRenderer, `TextInfoQueue.push(info)`) and the queue::push tail jump at 0x004942BE.
// Shape: STLport 4.x `if (_M_finish._M_cur != _M_finish._M_last - 1) { _Construct(_M_finish._M_cur, __t);
//   ++_M_finish._M_cur; } else _M_push_back_aux_v(__t);` (the increment is a memory `add [esi+10h],1Ch`).
//   STLport deque layout as in 0x00494106: _M_finish {cur +0x10, first +0x14, last +0x18, node +0x1C}.
// Label correction: catalogued as `CSubtitleRenderer_ManageTextStorage`; the body is STLport deque<CSetTextInfo>::push_back
//   (the deque is CSubtitleRenderer's TextInfoQueue container).
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
    void _M_push_back_aux_v(const CSetTextInfo& __t);                                           // 0x00494106
    void push_back(const CSetTextInfo& __t);
};

void CSetTextInfoDeque::push_back(const CSetTextInfo& __t)
{
    if (_M_finish._M_cur != _M_finish._M_last - 1) {
        new (_M_finish._M_cur) CSetTextInfo(__t);
        ++_M_finish._M_cur;
    }
    else
        _M_push_back_aux_v(__t);
}
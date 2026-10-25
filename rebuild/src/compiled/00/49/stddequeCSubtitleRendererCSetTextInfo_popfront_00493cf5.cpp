// std::deque::pop_front() -- STLport deque pop_front for the subtitle text-info queue: destroy the front record and step the cursor, or, for the last record of the front node, the slow path _M_pop_front_aux @ 0x00493C25
// Instantiation: std::deque<CSubtitleRenderer::CSetTextInfo,std::allocator<CSubtitleRenderer::CSetTextInfo> >::pop_front
// Leaf: ?pop_front@?$deque@VCSetTextInfo@CSubtitleRenderer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493CF5, 33 bytes, thiscall, plain `ret`; unaligned in the packed
//   size-optimised run (`#pragma optimize("s", on)`).
// Identity: called from CSubtitleRenderer::Clear (0x00492E37) and CSubtitleRenderer::SetAsInCutscene (0x0049326E),
//   i.e. their inlined `TextInfoQueue.pop()`; the matching push side shows the TU's shape (queue::push's out-of-line
//   copy 0x004942BE is a bare `jmp` to the out-of-line deque::push_back 0x004941C0). The Ego_r.pdb name set holds
//   deque<CSubtitleRenderer::CSetTextInfo>::pop_front. The landed name `CEnginePrimitiveRenderer2D::AddPrimitive`
//   (with callees "InitPrimitiveSlot" / "GrowAndAddPrimitive") is propagated.
// Callees: ~CSetTextInfo @ 0x0043E3D0, _M_pop_front_aux @ 0x00493C25 (tail `jmp`).
// Shape: STLport 4.6 `pop_front`: `_M_cur != _M_last - 1` (stride 0x1C), `_Destroy(_M_cur); ++_M_cur;` (a memory
//   `add [esi],1Ch`), else `_M_pop_front_aux()`.
// Family (owner item 1g, sweep2): CSubtitleRenderer's TextInfoQueue, Ego_r.pdb `queue<CSubtitleRenderer::CSetTextInfo,
//   deque<..> >`, over STLport 4.6 deque<CSubtitleRenderer::CSetTextInfo> in a size-optimised TU. The rows are the
//   CSetTextInfo copy ctor 0x004935A7, deque::pop_front 0x00493CF5, _M_pop_front_aux 0x00493C25, _M_push_back_aux_v
//   0x00494106 and push_back 0x004941C0; queue::push's out-of-line copy 0x004942BE is a bare `jmp` to push_back. The
//   element is the nested class CSubtitleRenderer::CSetTextInfo (Ego_r.pdb, 28 bytes; the name sets mangle it
//   `VCSetTextInfo@CSubtitleRenderer@@`): its destructor 0x0043E3D0 is landed under that name, and the landed
//   _M_pop_front_aux 0x00493C25 binds it. These rows had modelled a non-nested CSetTextInfo and flat stand-in
//   deques, so they bound `??1CSetTextInfo@@QAE@XZ`, `??0CSetTextInfo@@QAE@ABU0@@Z` and
//   `?InitPrimitiveSlot@@YIXPAX@Z` for the same retail functions.
#pragma optimize("s", on)
#include <stdlib.h>                                     // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <new>

struct CWideString {
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};

class CSubtitleRenderer {
public:
    class CSetTextInfo {                                // Ego_r.pdb, 28 bytes (a class: mangled V)
    public:
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
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> struct _Deque_iterator {
    typedef _Tp** _Map_pointer;
    static unsigned int _S_buffer_size() { return sizeof(_Tp) < 128 ? (128 / sizeof(_Tp)) : 1; }   // _MAX_BYTES
    _Tp*         _M_cur;                                // +0x00
    _Tp*         _M_first;                              // +0x04
    _Tp*         _M_last;                               // +0x08
    _Map_pointer _M_node;                               // +0x0C
    void _M_set_node(_Map_pointer __new_node) {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(_S_buffer_size());
    }
};
template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp> iterator;
    static unsigned int buffer_size() { return iterator::_S_buffer_size(); }
    iterator _M_start;                                  // +0x00
    iterator _M_finish;                                 // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;          // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;     // +0x24
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    void push_back(const value_type& __t) {
        if (this->_M_finish._M_cur != this->_M_finish._M_last - 1) {
            _Construct(this->_M_finish._M_cur, __t);
            ++this->_M_finish._M_cur;
        } else
            _M_push_back_aux_v(__t);
    }
    void pop_front() {
        if (this->_M_start._M_cur != this->_M_start._M_last - 1) {
            _Destroy(this->_M_start._M_cur);
            ++this->_M_start._M_cur;
        } else
            _M_pop_front_aux();
    }
protected:
    void _M_push_back_aux_v(const value_type& __t);
    void _M_pop_front_aux();                                                         // 0x00493C25
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1);                       // 0x00493FA8 (out of line here)
};

template void deque<CSubtitleRenderer::CSetTextInfo>::pop_front();
}

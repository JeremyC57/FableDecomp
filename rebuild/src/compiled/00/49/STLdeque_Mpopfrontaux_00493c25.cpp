// _STL::deque::_M_pop_front_aux() -- deque<CSubtitleRenderer::CSetTextInfo>: pop the last element of the front buffer: destroy it, free the buffer and step the start iterator to the next map node
// Leaf: ?_M_pop_front_aux@?$deque@VCSetTextInfo@CSubtitleRenderer@@
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00493C25, 51 bytes, `__fastcall`, plain `ret`. It sits unaligned in
//   a packed (/Os-style) run of STL instantiations.
// Identity: its only caller is the inlined `deque::pop_front` body at 0x00493CF5 (tail `jmp` when
//   `_M_cur == _M_last - 1`, element stride 0x1c), which is called from 0x00492E37 and from
//   CSubtitleRenderer::SetAsInCutscene (0x0049326E; the run 0x00492E4B..0x00493281 is the SetText overloads,
//   Update, SetAsInCutscene and ClickPast). The element destructor 0x0043E3D0 destroys four CWideStrings
//   (0x0099B510) at +0xc, +8, +4 and +0, which is CSubtitleRenderer::CSetTextInfo (Ego_r.pdb, 28 bytes: Text,
//   Button1Text, Button2Text, Button3Text, DisplayFlags, DisplayTime, UpdateWhilePaused, ChangePlayerMode), the
//   element of CSubtitleRenderer::TextInfoQueue. The manifest's "allocator<pair<CCharString const ...>>::construct"
//   is propagated.
// Callees: CSetTextInfo::~CSetTextInfo @ 0x0043E3D0 (out of line), free @ 0x00BFEA14 (the malloc-backed allocator's
//   deallocate, null-tested inline).
// Shape: STLport 4.6 `_M_pop_front_aux`: `_Destroy(_M_cur)`, deallocate the front buffer, `_M_set_node(node + 1)`
//   stores node, first and last = first + 0x70 (128-byte blocks hold four 28-byte elements), then `_M_cur =
//   _M_first` re-reads the member just stored.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14 (import thunk)

struct CWideString { void* PStringData; };

class CSubtitleRenderer {
public:
    class CSetTextInfo {                                 // 28 bytes (class: the PDB mangles it V)
    public:
        CWideString   Text;                              // +0x00
        CWideString   Button1Text;                       // +0x04
        CWideString   Button2Text;                       // +0x08
        CWideString   Button3Text;                       // +0x0c
        unsigned long DisplayFlags;                      // +0x10
        float         DisplayTime;                       // +0x14
        bool          UpdateWhilePaused;                 // +0x18
        bool          ChangePlayerMode;                  // +0x19
        ~CSetTextInfo();                                 // 0x0043E3D0
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* p, unsigned int) { if (p != 0) free(p); }   // malloc-backed node allocator
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> struct _Deque_iterator {
    typedef _Tp** _Map_pointer;
    static unsigned int _S_buffer_size() { return sizeof(_Tp) < 128 ? (128 / sizeof(_Tp)) : 1; }   // _MAX_BYTES
    _Tp*         _M_cur;                                  // +0x00
    _Tp*         _M_first;                                // +0x04
    _Tp*         _M_last;                                 // +0x08
    _Map_pointer _M_node;                                 // +0x0c
    void _M_set_node(_Map_pointer __new_node) {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(_S_buffer_size());
    }
};
template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp> iterator;
    static unsigned int buffer_size() { return iterator::_S_buffer_size(); }
    iterator _M_start;                                    // +0x00
    iterator _M_finish;                                   // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;       // +0x24
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    void pop_front() {
        if (this->_M_start._M_cur != this->_M_start._M_last - 1) {
            _Destroy(this->_M_start._M_cur);
            ++this->_M_start._M_cur;
        } else
            _M_pop_front_aux();
    }
protected:
    void _M_pop_front_aux();
};
// Called only when the front buffer holds exactly one element.
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_pop_front_aux()
{
    _Destroy(this->_M_start._M_cur);
    this->_M_map_size.deallocate(this->_M_start._M_first, this->buffer_size());
    this->_M_start._M_set_node(this->_M_start._M_node + 1);
    this->_M_start._M_cur = this->_M_start._M_first;
}
}

template void _STL::deque<CSubtitleRenderer::CSetTextInfo>::_M_pop_front_aux();
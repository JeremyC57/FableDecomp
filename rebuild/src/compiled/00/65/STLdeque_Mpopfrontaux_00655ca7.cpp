// std::deque::_M_pop_front_aux() -- deque<pair<CCharString,long> >: pop the last element of the front buffer: destroy it, free the buffer and step the start iterator to the next map node
// Leaf: ?_M_pop_front_aux@?$deque@U?$pair@VCCharString@@J@std@@
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00655CA7, 53 bytes, `__fastcall`, plain `ret`. It sits unaligned in
//   a packed (/Os-style) run of STL instantiations.
// Identity: its only caller is the inlined `deque::pop_front` body at 0x00655FE9 (tail `jmp` when
//   `_M_cur == _M_last - 1`, element stride 8, element destructor CCharString::~CCharString 0x0099EAE0 on +0),
//   called from 0x0064AE5C and from the "Clear" at 0x0064AE66 (`while (!q.empty()) q.pop();` over the deque at
//   this+0x68 after zeroing this+0x60). An 8-byte element whose first member is a CCharString is
//   pair<CCharString, long>; the only Ego_r.pdb deque of it is NPlayerGui::CDrawGuildSeal::Messages
//   (queue<pair<CCharString, long> >), and FableWin.pdb lists CDrawGuildSeal::ClearMessages, so 0x0064AE66 is
//   probably ClearMessages and this the Messages deque's helper (owner not proven). The manifest's
//   "allocator<pair<CCharString const ...>>::construct" is propagated.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (the pair's implicit destructor, inlined to the member call),
//   free @ 0x00BFEA14 (the malloc-backed allocator's deallocate, null-tested inline).
// Shape: the same STLport 4.6 `_M_pop_front_aux` as 0x00493C25: `_Destroy(_M_cur)`, deallocate, `_M_set_node(node +
//   1)` with last = first + 0x80 (sixteen 8-byte pairs per 128-byte block), then `_M_cur = _M_first` re-read.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14 (import thunk)

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};

namespace std {
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

namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}

template void std::deque<std::pair<CCharString, long> >::_M_pop_front_aux();
// std::vector<std::basic_string<char> >::~vector() -- STLport 4.6: destroy each 12-byte string (free its block), then free the buffer
// Leaf: ??1?$vector@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@QAE@XZ
// Naming wave 30 (family lhn): was CRetailVector_00569940::Destroy_CRetailVector_00569940 (descriptive). The out-of-line COMDAT
//   ~LHHttpHeaders 0x00569C30 tail-calls for HdrDocumentHeader (Ego_r.pdb vector<basic_string<char> >; the string dtor is expanded inline,
//   as the copy in ~LHHttpHeaderStatus is).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569940, 57 bytes, thiscall, plain `ret`.
// Shape: the end pointer is cached before the loop, every non-null string block is freed (`add esp,4`: a speed TU), then the buffer.
//   DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line `??1` (the chunk 656/657 idiom).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _CharT> class char_traits { };

template <class _Tp, class _Alloc> class _String_base {             // STLport 4.6: 12 bytes
public:
    ~_String_base() { _M_deallocate_block(); }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    ~basic_string() { }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

// Emission helper: destroying the vector by name is what makes VC7.1 emit the out-of-line COMDAT.
void DestroyOne(std::vector<std::basic_string<char> >* strings)
{
    strings->~vector();
}
// std::vector<std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>>::resize(size_type new_size) -- STLport one-argument resize: resize(new_size, std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>()) through the out-of-line two-argument resize; the zero default value needs no destruction
// Leaf: ?resize@?$vector@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$allocator@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086DE40, 36 bytes, thiscall, `ret 4`.
// Identity (proven): FableWin.pdb ?resize@?$vector@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@...@QAEXI@Z; its callee 0x0086D900 is bound as that vector's resize(n, x) (only caller of 0x0086D900 besides the landed GFReadVectorMapMemDump<CTextureMorph> 0x0086DEB0); no direct callers (an out-of-line inline COMDAT).
// Callees: vector<std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>>::resize(size_type, const value_type&) 0x0086D900 (thiscall, `ret 8`).
// Shape (family K030, representative 0x0086DE40): STLport 4.6 `void resize(size_type __new_size) { resize(__new_size,
//   _Tp()); }` -- the default value is three zero words in the frame (a value-initialised pair / an empty CArray), whose
//   inline destructor folds to nothing because its storage pointers are known null.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                // Ego_r.pdb, 8 bytes
    public:
        CTextureMorph() : NewTexture(0), GroupID(0) { }  // FableWin.pdb ??0CTextureMorph@CRandomAppearanceMorph@@QAE@XZ
        unsigned long NewTexture;                        // +0x00
        unsigned long GroupID;                           // +0x04
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x04
    pair() : first(_T1()), second(_T2()) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    static unsigned long _M_fill_value_words;               // stand-in sink (see below)
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x0086D900 for this element
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}
typedef std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> TElem;
template void std::vector<TElem>::resize(unsigned int);
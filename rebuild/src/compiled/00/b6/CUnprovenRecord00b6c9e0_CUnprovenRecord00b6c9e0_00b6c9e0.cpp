// CUnprovenRecord_00b6c9e0::~CUnprovenRecord_00b6c9e0() -- implicit destructor: destroy the vector at +0x14, then the one at +0x04 (free each block when there is one)
// Leaf: ??1CUnprovenRecord_00b6c9e0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b6c9e0, 37 bytes, thiscall, plain `ret`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT); its own bytes free the blocks of two vectors at +0x14 and +0x04 in that order (members destroyed in reverse) with nothing else to destroy, so the record is {4 bytes, vector, 4 bytes, vector} with trivially-destroyed elements; the record and element types are unproven.
// Callee: free 0x00BFEA14 (twice).
// Shape (family L047): two inlined STLport ~vector (trivial _Destroy, deallocate = `if (start) free(start)`), the later
//   member first.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
    ~vector()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
class CUnprovenElementA_00b6c9e0 {                            // descriptive: the element types are unproven
public:
    unsigned long Word;
};
class CUnprovenElementB_00b6c9e0 {
public:
    unsigned long Word;
};
class CUnprovenRecord_00b6c9e0 {                                              // descriptive: the record type is unproven
public:
    unsigned long Word00;                                 // +0x00 (trivially destroyed)
    std::vector<CUnprovenElementA_00b6c9e0> First;           // +0x04
    unsigned long Word10;                                 // +0x10 (trivially destroyed)
    std::vector<CUnprovenElementB_00b6c9e0> Second;          // +0x14
    ~CUnprovenRecord_00b6c9e0();
};
CUnprovenRecord_00b6c9e0::~CUnprovenRecord_00b6c9e0()
{
}
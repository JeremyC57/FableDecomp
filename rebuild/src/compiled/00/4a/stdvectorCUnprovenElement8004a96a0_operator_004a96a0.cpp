// std::vector<CUnprovenElement8_004a96a0>::operator[](unsigned int __n) -- STLport: *(_M_start + n) over 8-byte elements
// Leaf: ??A?$vector@VCUnprovenElement8_004a96a0@@V?$allocator@VCUnprovenElement8_004a96a0@@@std@@@std@@QAEAAVCUnprovenElement8_004a96a0@@I@Z
// Retail: Fable.exe @ 0x004A96A0, 12 bytes, __fastcall `ret 4`: `mov eax,[ecx] ; mov ecx,[esp+4] ; lea eax,[eax+ecx*8]`.
// Identity (descriptive): an out-of-line STLport 4.6 vector::operator[] over an 8-byte element (_M_start at +0, `*8`
// stride); no caller (gap-scan COMDAT), and its neighbours' "_Dest_val<CCountedPointer<COptimisedPrimitive>>" labels are
// propagated, so the element type (8 bytes: CCountedPointer-sized) and the const-ness are unproven.
class CUnprovenElement8_004a96a0 {
public:
    void* Words[2];                                       // 8 bytes
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef _Tp& reference;
    _Tp* _M_start;                                        // +0
    _Tp* _M_finish;                                       // +4
    _Tp* _M_end_of_storage;                               // +8
    reference operator[](unsigned int __n) { return *(_M_start + __n); }
};
template class vector<CUnprovenElement8_004a96a0>;
}
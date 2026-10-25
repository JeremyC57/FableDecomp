// std::vector<CBrainUpdateZone>::_M_clear() -- STLport vector storage release for the brain def's update-zone vector: destroy the (trivial) elements, then deallocate the block (free when non-null)
// Leaf: ?_M_clear@?$vector@VCBrainUpdateZone@@V?$allocator@VCBrainUpdateZone@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00462E04, 14 bytes, thiscall (size-optimised def TU: pop ecx).
// Identity (sweep2, 2026-10-25): the only caller is std::vector<CBrainUpdateZone>::operator= 0x00462CB0 (call 0x00462CF9,
//   between _M_allocate_and_copy 0x00462DC1 and the `_M_start = tmp` store: STLport's `_M_clear()` slot), whose element
//   identity comes from CBrainDef::operator= 0x00462B15 and the CAIBrain ctor 0x0088C160 (Ego_r.pdb
//   vector<CBrainUpdateZone>). The callee 0x00BFEA14 is the MSVCR71 free import thunk (STLport allocator::deallocate).
//   Renamed from the placeholder _unattributed::ReleaseFirstMember_00462e04 (fistp pass: "owner unknown").
// Shape: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, ...); }`;
//   the trivial-destructor loop folds away and deallocate is `if (p) free(p)`.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CBrainUpdateZone {
public:
    long Distance;                                      // +0x0
    long UpdateFrequency;                               // +0x4
    long InterruptionFrequency;                         // +0x8
};

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    void _M_clear();
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}

template void vector<CBrainUpdateZone>::_M_clear();

}   // namespace std

// ---- test harness ----
#include <stdio.h>
#include <stdlib.h>
struct VecView { CBrainUpdateZone* s; CBrainUpdateZone* f; CBrainUpdateZone* e; };
int main()
{
    std::vector<CBrainUpdateZone> v;
    VecView& w = *(VecView*)&v;
    w.s = (CBrainUpdateZone*)malloc(3 * sizeof(CBrainUpdateZone)); w.f = w.s + 2; w.e = w.s + 3;
    v._M_clear();                                       // frees the block
    w.s = 0; w.f = 0; w.e = 0;
    v._M_clear();                                       // null block: no call
    puts("00462e04_TEST PASS");
    return 0;
}

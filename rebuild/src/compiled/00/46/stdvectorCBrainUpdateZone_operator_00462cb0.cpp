// std::vector<CBrainUpdateZone>::operator=(const vector& x) -- STLport copy assignment of the brain def's update-zone vector (12-byte {Distance, UpdateFrequency, InterruptionFrequency} entries): reallocate through _M_allocate_and_copy when x outgrows the capacity, else element-wise copy over the live prefix and copy-construct the tail
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: ??4?$vector@VCBrainUpdateZone@@V?$allocator@VCBrainUpdateZone@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462CB0, 273 bytes, thiscall, `ret 4` (size-optimised def TU).
// Identity (sweep2, 2026-10-25): callers are CBrainDef::operator= 0x00462B15 (`lea ecx,[esi+54h]` = the retail
//   UpdateZones vector after Behaviours +0x3C and DisabledBehaviours +0x48; Ego_r CBrainDef holds exactly these three
//   vectors) and the CAIBrain ctor 0x0088C160 (copies the def's UpdateZones into CAIBrain::UpdateZones, Ego_r.pdb
//   vector<CBrainUpdateZone>). Ego_r.pdb name set: ??4?$vector@VCBrainUpdateZone@@V?$allocator@VCBrainUpdateZone@@@std@@@std@@QAEAAV01@ABV01@@Z.
//   CBrainUpdateZone (Ego_r.pdb, 12 bytes): long Distance, UpdateFrequency, InterruptionFrequency. The landed name
//   CActiveFile_CopyAssign was propagated; the model was a nested FileVectorAssignment::std (owner 2026-10-25: top-level std).
// Callees: _M_allocate_and_copy @ 0x00462DC1, _M_clear @ 0x00462E04 (both out of line in this size TU).
// Shape: STLport 4.6 vector::operator= over a class element (__type_traits all __false_type): __copy_ptrs ->
//   __copy random-access loop, __uninitialized_copy -> placement-new loop (VC7.1 null-tests the placement address).
#include <new>

class CBrainUpdateZone {
public:
    long Distance;                                          // +0x0
    long UpdateFrequency;                                   // +0x4
    long InterruptionFrequency;                             // +0x8
};

namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> __forceinline _Tp* _Copy(_Tp* __first, _Tp* __last, _Tp* __result)
{
    for (int __n = __last - __first; __n > 0; --__n, ++__first, ++__result)
        *__result = *__first;
    return __result;
}

template <class _Tp> __forceinline void _UninitializedCopy(_Tp* __result, _Tp* __first, _Tp* __last)
{
    while (__first != __last) {
        if (__result)
            new (__result) _Tp(*__first);
        ++__first;
        ++__result;
    }
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
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    template <class _ForwardIter> pointer _M_allocate_and_copy(size_type __n, _ForwardIter __first, _ForwardIter __last);   // 0x00462DC1
    public:
    protected:
    void _M_clear();                                                                           // 0x00462E04
    public:
    vector& operator=(const vector& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        size_type __xlen = __x._M_finish - __x._M_start;
        if (__xlen > size_type(this->_M_end_of_storage._M_data - this->_M_start)) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = __tmp + __xlen;
        }
        else if (size_type(this->_M_finish - this->_M_start) >= __xlen) {
            _Copy(__x._M_start, __x._M_finish, this->_M_start);
        }
        else {
            _Copy(__x._M_start, __x._M_start + (this->_M_finish - this->_M_start), this->_M_start);
            _UninitializedCopy(this->_M_finish, __x._M_start + (this->_M_finish - this->_M_start), __x._M_finish);
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CBrainUpdateZone>& vector<CBrainUpdateZone>::operator=(const vector<CBrainUpdateZone>&);

}   // namespace std

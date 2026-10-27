// std::vector::operator=(const vector& __x) -- (vector<long>) assign: reallocate, shrink or extend by memmove
// Leaf: ??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454886, 184 bytes, thiscall, `ret 4` (frame-pointer size module).
// Identity (proven): the bound ??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z; its callers are the operator= of CSpecialAbilitiesBurntEffectDef (Color, Ego_r.pdb vector<long>), CCreatureModeDef (AvailableIdleModes, vector<long>), CHitLocationsDef::Copy and the CVillagePeopleDef operator=.
// Callees: vector<long>::_M_allocate_and_copy<const long*> 0x00454961, vector<long>::_M_clear 0x00440080, memmove 0x00BFEAE6,
//   __uninitialized_copy<const long*, long*>(..., const __true_type&) 0x0045493E (__fastcall).
// Shape (family M038, representative 0x00454886): STLport 4.6 vector.c operator= -- `optimize("y", off)` +
//   `optimize("s", on)`; __copy_trivial must be `__forceinline` (the size module calls it out of line otherwise) and keeps
//   its `__last == __first` test; the extend arm's tag comes from an inline helper returning __true_type by value (the
//   zero-filled byte and its self-copy at [ebp-1]).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memmove(void*, const void*, unsigned int);
namespace std {
struct __true_type { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
__forceinline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp> __forceinline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __true_type&);
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
template <class _Tp> __forceinline __true_type _Is_POD(_Tp*) { return __true_type(); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return unsigned int(this->_M_end_of_storage._M_data - this->_M_start); }
    vector& operator=(const vector& __x);
protected:
    void _M_clear();
    template <class _ForwardIterator> _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last);
};
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            _Tp* __tmp = _M_allocate_and_copy(__xlen, (const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            _Tp* __i = __copy_ptrs((const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0, (_Tp*)this->_M_start, _TrivialAss());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const _Tp*)__x._M_start, (const _Tp*)__x._M_start + size(), (_Tp*)this->_M_start, _TrivialAss());
            __uninitialized_copy((const _Tp*)__x._M_start + size(), (const _Tp*)__x._M_finish + 0, this->_M_finish, _Is_POD(this->_M_finish));
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
template class vector<long>;
}
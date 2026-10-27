// std::vector::_M_allocate_and_copy(unsigned int __n, const CTextureReplacementDef::CEntry* __first, const CTextureReplacementDef::CEntry* __last) -- (vector<CTextureReplacementDef::CEntry>) a new block holding copies of [first, last)
// Leaf: ??$_M_allocate_and_copy@PBVCEntry@CTextureReplacementDef@@@?$vector@VCEntry@CTextureReplacementDef@@V?$allocator@VCEntry@CTextureReplacementDef@@@std@@@std@@IAEPAVCEntry@CTextureReplacementDef@@IPBV23@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456344, 69 bytes, thiscall (this unused), `ret 0Ch` (size-optimised module).
// Identity (proven): its only caller 0x00456263 is the vector operator= that CTextureReplacementDef::Copy 0x00456244 applies to the member at +0x28: Ego_r.pdb CTextureReplacementDef::Entries, CArray<CTextureReplacementDef::CEntry> (a vector).
// Callees: malloc 0x00BFEA0E (STLport __malloc_alloc, skipped for n == 0).
// Shape (family M039, representative 0x00456344): STLport `__result = allocate(__n); __uninitialized_copy(__first, __last,
//   __result, __false_type()); return __result;` -- allocate, _Construct and __uninitialized_copy `__forceinline` (the size
//   module calls them out of line otherwise); the copy loop addresses the target as `result - first + cur`.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CTextureReplacementDef {
public:
    class CEntry { public: long OriginalTexture; long NewTexture; };   // 8 bytes (the shl 3); names from the landed 0x00469CB2 (Ego_d.pdb CEntry(long, long))
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
protected:
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
public:
    void Instantiate(unsigned int n, const _Tp* f, const _Tp* l) { _M_allocate_and_copy(n, f, l); }
};
template class vector<CTextureReplacementDef::CEntry>;
}
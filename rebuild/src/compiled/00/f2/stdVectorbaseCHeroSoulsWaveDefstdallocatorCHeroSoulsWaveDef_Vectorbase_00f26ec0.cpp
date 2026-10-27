// std::_Vector_base<CHeroSoulsWaveDef, std::allocator<CHeroSoulsWaveDef> >::_Vector_base(size_type n, const allocator<CHeroSoulsWaveDef>& a) -- STLport: allocate room for n elements, empty
// Leaf: ??0?$_Vector_base@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAE@IABV?$allocator@VCHeroSoulsWaveDef@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f26ec0, 56 bytes, thiscall, `ret 8`, returns this.
// Identity (probable): bound only as the stand-in ??0CRetailVectorBase_CClanMember@@QAE@KABUCRetailAllocator@@@Z; its caller 0x00F26E60 is the vector copy that CHeroSoulsRoundDef's copy 0x00F26DB0 (L142) calls for its wave vector, whose 0x38-byte element is the landed CHeroSoulsWaveDef (0x00F27BC0's model).
// Callees: malloc 0x00BFEA0E (n * 0x38; allocate(0) is 0).
// Shape (family L147 = the base constructor of the landed std::vector<CTVertexNormTex1>::vector(size_type) 0x00BB9FD0): the
//   three zero stores, allocate(n), then start / finish / end of storage.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CHeroSoulsWaveDef {                                        // retail 0x38 bytes
public:
    unsigned char _body[0x38];
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};
template _Vector_base<CHeroSoulsWaveDef, allocator<CHeroSoulsWaveDef> >::_Vector_base(unsigned int, const allocator<CHeroSoulsWaveDef>&);
}
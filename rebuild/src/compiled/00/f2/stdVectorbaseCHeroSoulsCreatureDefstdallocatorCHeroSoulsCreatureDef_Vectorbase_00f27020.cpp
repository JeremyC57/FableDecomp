// std::_Vector_base<CHeroSoulsCreatureDef, std::allocator<CHeroSoulsCreatureDef> >::_Vector_base(size_type n, const allocator<CHeroSoulsCreatureDef>& a) -- STLport: allocate room for n elements, empty
// Leaf: ??0?$_Vector_base@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAE@IABV?$allocator@VCHeroSoulsCreatureDef@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f27020, 56 bytes, thiscall, `ret 8`, returns this.
// Identity (proven): bound ??0?$_Vector_base@VCHeroSoulsCreatureDef@@...; its caller 0x00F26FB0 copies CHeroSoulsWaveDef's creature vector (0x34-byte elements).
// Callees: malloc 0x00BFEA0E (n * 0x34; allocate(0) is 0).
// Shape (family L147 = the base constructor of the landed std::vector<CTVertexNormTex1>::vector(size_type) 0x00BB9FD0): the
//   three zero stores, allocate(n), then start / finish / end of storage.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CHeroSoulsCreatureDef {                                        // retail 0x34 bytes
public:
    unsigned char _body[0x34];
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
template _Vector_base<CHeroSoulsCreatureDef, allocator<CHeroSoulsCreatureDef> >::_Vector_base(unsigned int, const allocator<CHeroSoulsCreatureDef>&);
}
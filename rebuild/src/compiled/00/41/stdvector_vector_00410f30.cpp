// std::vector::vector(unsigned int __n) -- (<signed char>) the zero-filled size constructor: allocate n elements, fill with signed char()
// Leaf: ??0?$vector@CV?$allocator@C@std@@@std@@QAE@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410F30, 77 bytes, thiscall, `ret 4`.
// Identity (descriptive): the third 1-byte memset-filled instantiation, by elimination (callers CWorld::LoadQuestsFromFile 0x004A0D90 and CWorldMap::LoadFromFile 0x00507C30 only treat it as a raw file buffer); the element type is not proven.
// Callees: malloc 0x00BFEA0E (STLport __malloc_alloc through allocator<signed char>::allocate, skipped for n == 0).
// Shape (family M016, representative 0x00410F30): `_Vector_base(n, a)` zeroes the three words, allocates, sets
//   _M_end_of_storage = start + n; then `_M_finish = uninitialized_fill_n(_M_start, n, signed char())`, which STLport 4.6
//   specialises for the char types as memset (inlined /Oi as rep stosd + rep stosb).
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void* __cdecl memset(void*, int, unsigned int);
namespace std {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;    // +0x08
};
inline signed char* uninitialized_fill_n(signed char* __first, unsigned int __n, const signed char& __val)
{
    memset(__first, __val, __n);
    return __first + __n;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};
template class vector<signed char>;
}
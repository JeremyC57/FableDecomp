// CIDrawEnvironmentParams::CThemeChannel::CThemeChannel(const CThemeChannel& other) -- implicit memberwise copy of an environment theme channel: block-copy the 19 colours and 30 scalers, then deep-copy the two vectors of each of the three texture sets
// Leaf: ??0CThemeChannel@CIDrawEnvironmentParams@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6010, 291 bytes, thiscall, `ret 4`, no EH frame.
// Identity: the catalog's "CFeatDef_CopySpecializedData" is a descriptive auto label (CFeatDef is 100 bytes). This is
//   the compiler-generated copy constructor of CIDrawEnvironmentParams::CThemeChannel (Ego_r.pdb: Colours
//   CRGBFloatColour[19] +0, Scalers float[30] +0x130, TextureSets CIDrawEnvironmentTextureSet[3] +0x1A8 = {CArray<long>
//   Textures; CArray<float> Blends;}; 0x1F0 bytes with retail's 12-byte STLport vectors), emitted out of line as a
//   COMDAT: its callers are the theme-channel vector's __uninitialized_copy 0x006B6500, _M_insert_overflow 0x006B6990
//   (fill_len == 1 and the inlined fill_n) and _M_fill_insert 0x006B6B30.
// Callees: malloc 0x00BFEA0E (allocator::allocate), memmove 0x00BFEAE6 (__copy_trivial).
// Shape: memberwise copy in declaration order: the two POD arrays become `rep movsd` blocks of 0x4C and 0x1E dwords,
//   then the compiler's array-copy loop over the three texture sets (`mov [esp+10h],3` counter, strength-reduced source /
//   destination pointers), each STLport `vector(const vector&)` inline: `_Vector_base(x.size(), allocator)` zero-stores
//   the three words, allocates n elements and sets finish/end of storage, then `__uninitialized_copy(.., __true_type)` =
//   __copy_trivial (the `last == first` test around memmove + n). The out-of-line body comes from the same-TU
//   placement-copy loop below (the 0x006B6500 shape), which VC7.1 does not inline the 0x1F0-byte copy into.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14 (~_Vector_base, not emitted here)
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };


inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }

    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                               __true_type());
    }
    ~vector() { }

};

} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CRGBFloatColour {
public:
    float R, G, B, A;
};

class CIDrawEnvironmentTextureSet {
public:
    CArray<long>  Textures;                                 // +0x00
    CArray<float> Blends;                                   // +0x0C
};

class CIDrawEnvironmentParams {
public:
    class CThemeChannel {                                   // 0x1F0 bytes, implicit copy constructor @ 0x006B6010
    public:
        CRGBFloatColour             Colours[19];            // +0x000
        float                       Scalers[30];            // +0x130
        CIDrawEnvironmentTextureSet TextureSets[3];         // +0x1A8
    };
};


namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{                                                                                  // 0x006B6500
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
typedef CIDrawEnvironmentParams::CThemeChannel _ThemeChannel;
template _ThemeChannel* __fastcall __uninitialized_copy<_ThemeChannel*, _ThemeChannel*>(_ThemeChannel*, _ThemeChannel*,
                                                                                       _ThemeChannel*, const __false_type&);
} // namespace std
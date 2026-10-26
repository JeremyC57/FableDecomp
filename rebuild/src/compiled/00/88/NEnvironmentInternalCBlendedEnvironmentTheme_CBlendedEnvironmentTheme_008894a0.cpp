// NEnvironmentInternal::CBlendedEnvironmentTheme::CBlendedEnvironmentTheme(const CBlendedEnvironmentTheme& other) -- compiler-generated copy constructor: time of day, the 19 colours and 30 scalers as blocks, then each of the 3 texture sets' two CArrays deep-copied (allocate x.size() words, memmove)
// Leaf: ??0CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008894A0, 301 bytes incl. int3 padding (299 of code), thiscall, `ret 4`,
//   returns this.
// Identity: the catalog's "LargeStruct_CopyWithNestedArrays" is descriptive. This is the out-of-line copy constructor
//   the landed __uninitialized_copy 0x00889880 calls for every 0x1F4-byte NEnvironmentInternal::CBlendedEnvironmentTheme
//   (vector<CBlendedEnvironmentTheme> copy / regrowth); its layout is the one of the landed default constructor
//   0x008864A0 and assignment 0x006B5DF0: TimeOfDay +0, Colours CRGBFloatColour[19] +4, Scalers float[30] +0x134,
//   TextureSets CIDrawEnvironmentTextureSet[3] +0x1AC (Textures CArray<long> +0, Blends CArray<float> +0xC; the
//   retail CArray is a 12-byte STLport vector).
// Callees: malloc 0x00BFEA0E (STLport malloc allocator), memmove 0x00BFEAE6 (__copy_trivial).
// Shape: the implicit memberwise copy: one dword, `rep movsd` x 0x4C and x 0x1E for the POD arrays, then the 3-element
//   array of texture sets as a counted loop, each CArray copy inlining STLport `vector(const vector& x) :
//   _Vector_base(x.size(), x.get_allocator())` (three zero stores, allocate, start/finish/end stores) and
//   `_M_finish = __uninitialized_copy(x.begin(), x.end(), _M_start, __true_type())` = __copy_trivial (memmove + n).
extern "C" void* __cdecl malloc(unsigned int size);                                 // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);    // 0x00BFEAE6
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

namespace std {
struct __true_type { __true_type() { } };

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
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef const _Tp* const_pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    explicit vector(const _Alloc& __a = _Alloc()) : _Base(__a) { }
    vector(const vector<_Tp, _Alloc>& __x) : _Base(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start,
                                               __true_type());
    }
};
} // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
};

class CRGBFloatColour { public:
    float R, G, B, A;
};

struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                     // +0x00
    CArray<float> Blends;                       // +0x0C
};

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    float                       TimeOfDay;      // +0x000
    CRGBFloatColour             Colours[19];    // +0x004
    float                       Scalers[30];    // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3]; // +0x1AC
    CBlendedEnvironmentTheme();                 // 0x008864A0
};
}

// Emits the implicit copy constructor out of line (as the retail vector helpers call it).
void CopyConstructTheme(void* where, const NEnvironmentInternal::CBlendedEnvironmentTheme& theme)
{
    new (where) NEnvironmentInternal::CBlendedEnvironmentTheme(theme);
}
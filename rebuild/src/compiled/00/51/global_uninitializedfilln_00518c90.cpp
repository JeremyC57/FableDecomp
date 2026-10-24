// __uninitialized_fill_n(CSeaBodyDesc* first, unsigned int n, const CSeaBodyDesc& x, const __false_type&) -- STLport non-POD fill-construct of sea body records: n deep copies of x (Identifier string and Maps array) into raw storage
// Leaf: ??$__uninitialized_fill_n@PAUCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@
//
// Retail 0x00518C90, 140 bytes, `ret 8` (/Gr __fastcall: first in ecx, n in edx; &x and the tag on the stack).
// Callees: 0x0099EC30 CCharString::CCharString(const CCharString&), 0x00BFEA0E malloc, 0x00BFEAE6 memmove.
// Caller: the CSeaBodyDesc vector's _M_insert_overflow 0x0051C110 (the n != 1 fill).
// Element: CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc (see 0x00518C00 / 0x00519EB0).
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (unsigned n: `test edx,edx / jbe`; the
//   count is kept in the `push ecx` slot) with the same inline CSeaBodyDesc copy as 0x00518C00.  The
//   retail TU is /Gr (explicit __fastcall here).
// Label: catalog "CCharString_InitializeArrayRange" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CCharString {
public:
    const char* PString;
    CCharString(const CCharString& other);     // 0x0099EC30
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
                           : ((char*)memmove(result, first, ((const char*)last - (const char*)first)))
                               + ((const char*)last - (const char*)first);
}
template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* first, const _Tp* last, _Tp* result, const __true_type&)
{
    return (_Tp*)__copy_trivial(first, last, result);
}
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator<T>&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector<T>& x) : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
}

template <class T> class CArray : public std::vector<T> {};

class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {
        CCharString  Identifier;               // +0x0
        CArray<long> Maps;                     // +0x4
    };
};
typedef CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc CSeaBodyDesc;

template CSeaBodyDesc* __fastcall std::__uninitialized_fill_n<CSeaBodyDesc*, unsigned int, CSeaBodyDesc>(
    CSeaBodyDesc*, unsigned int, const CSeaBodyDesc&, const std::__false_type&);
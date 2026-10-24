// __uninitialized_copy(CSeaBodyDesc* first, CSeaBodyDesc* last, CSeaBodyDesc* result, const __false_type&) -- STLport non-POD range copy-construct of sea body records: each Identifier string and Maps array is deep-copied into raw storage
// Leaf: ??$__uninitialized_copy@PAUCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@
//
// Retail 0x00518C00, 142 bytes (+ int3 padding to 144), `ret 8` (/Gr __fastcall: first in ecx, last in edx; result and the tag on the stack).
// Callees: 0x0099EC30 CCharString::CCharString(const CCharString&), 0x00BFEA0E malloc, 0x00BFEAE6 memmove.
// Callers: the CSeaBodyDesc vector's _M_insert_overflow 0x0051C110 (twice) and 0x0050726F (the SeaBodies rebuild in
//   0x00507070).
// Element: Ego_r.pdb CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc = { CCharString Identifier; CArray<long> Maps; }
//   (16 bytes with STLport's three-pointer vector; see 0x00519EB0 for the identification).
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` with the implicit
//   CSeaBodyDesc copy constructor inline: CCharString's out-of-line copy, then CArray<long> = vector<long>(x):
//   _Vector_base zeroes the words, allocates x.size() longs, then __copy_trivial (memmove + n).  The placement new keeps
//   its null test on cur; VC strength-reduces &cur->Maps._M_finish into edi.  The retail TU is /Gr (explicit __fastcall).
// Label: catalog "std_uninitialized_copy_with_construction" (propagated).
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

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
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

template CSeaBodyDesc* __fastcall std::__uninitialized_copy<CSeaBodyDesc*, CSeaBodyDesc*>(CSeaBodyDesc*, CSeaBodyDesc*,
                                                                                         CSeaBodyDesc*, const std::__false_type&);
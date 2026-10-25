// std::__uninitialized_fill_n(CResourcePage* first, unsigned int n, const CResourcePage& x, const __false_type&) -- (CGPUScratchBufferTextureManager::CResourcePage) copy-construct n copies of x into raw storage at first, returning the end
// Leaf: ??$__uninitialized_fill_n@PAUCResourcePage@CGPUScratchBufferTextureManager@@IU12@@std@@YIPAUCResourcePage@CGPUScratchBufferTextureManager@@PAU12@IABU12@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F384E0, 163 bytes, __fastcall (first ecx, n edx), `ret 8`.
// Identity: called by vector<CResourcePage>::_M_insert_overflow 0x00F38770 when the fill count is not 1 (see 0x00F38440);
//   catalog "Vector_CopyMultiple_Deep" is descriptive.
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6 (the inlined vector<long> copy constructor, as 0x00F38440).
// Shape: STLport 4.6 `for (; n > 0; --n, ++cur) _Construct(&*cur, x)` with an unsigned size type (`jbe` on entry, the count
//   spilled to the `push ecx` slot and counted down), placement-new null test per element.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                    // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp>
inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

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
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, __true_type());
    }
};

}   // namespace std

class CGPUScratchBufferTextureManager {
public:
    struct CResourcePage {                       // 0x1c bytes in retail (Ego_r.pdb 32: CArray<long> is 16 there)
        long                ScratchPool;           // +0x00
        std::vector<long>  ResourceList;          // +0x04
        unsigned char*      NextFreeBufferAddress; // +0x10
        long                FreeSpaceSize;         // +0x14
        bool                EnableAutoRestore;     // +0x18
    };
};
typedef CGPUScratchBufferTextureManager::CResourcePage CResourcePage;

namespace std {

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CResourcePage* __fastcall __uninitialized_fill_n(CResourcePage*, unsigned int, const CResourcePage&, const __false_type&);

}   // namespace std
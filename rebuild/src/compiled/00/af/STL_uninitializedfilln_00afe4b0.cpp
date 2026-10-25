// std::__uninitialized_fill_n(CArray<long>* first, unsigned int n, const CArray<long>& x, const __false_type&) -- copy-construct n copies of one particle primitive-ID list into raw storage; returns the end of the run
// Flags: /Gr
// Leaf: ??$__uninitialized_fill_n@PAV?$CArray@J@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE4B0, length 127 bytes, __fastcall (first ecx, n edx), `ret 8` (&x and
//   the empty __false_type tag), the end pointer in eax.
// Callers: the n > 1 arm of vector<CArray<long>>::_M_insert_overflow @ 0x00AFE6F0 (call 0x00AFE77A), _M_fill_insert @
//   0x00AFE7C0 (call 0x00AFE876) and the 0x00AFE690 forwarder -- the growth chain of vector<CArray<long>>::resize @
//   0x00AFE910, which the particle renderers' PreparePrimitives call as `if (PrimIDList.size() == 0)
//   PrimIDList.resize(1)` (CParticle::PrimIDList, Ego_r.pdb CArray<CArray<long> > at +0x84).
// Callees: malloc @ 0x00BFEA0E and memmove @ 0x00BFEAE6 (the inline vector copy constructor).
// Shape: STLport `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` with the unsigned count homed in the
//   `push ecx` frame word and ebp as the zero register; CArray<long>'s implicit copy constructor is the STLport vector
//   copy (`_Vector_base(n)` zero-stores + allocate, then `__copy_trivial`), as the landed CVertexGroupBlock twin
//   0x00ADC3B0.
// Label: catalog "Vector_AllocateFromTemplate" is behavioural.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class T> class allocator {                        // STLport allocator<T>: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    _Vector_base(unsigned int n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> class vector : public _Vector_base<T> {  // POD element (__true_type copy)
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = (T*)__copy_trivial(x._M_start, x._M_finish, this->_M_start);
    }
};

} // namespace std

template <class T> class CArray : public std::vector<T> {   // Fable's CArray: the retail 12-byte STLport vector
};

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CArray<long>* __uninitialized_fill_n<CArray<long>*, unsigned int, CArray<long> >(
    CArray<long>*, unsigned int, const CArray<long>&, const __false_type&);
} // namespace std
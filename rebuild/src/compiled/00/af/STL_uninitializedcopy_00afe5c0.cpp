// _STL::__uninitialized_copy(CArray<long>* first, CArray<long>* last, CArray<long>* result, const __false_type&) -- copy-construct a run of particle primitive-ID lists into raw storage; returns the end of the copied run
// Flags: /Gr
// Leaf: ??$__uninitialized_copy@PAV?$CArray@J@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE5C0, length 115 bytes, __fastcall (first ecx, last edx), `ret 8`
//   (result and the empty __false_type tag), the end pointer in eax.
// Callers: vector<CArray<long>>::_M_insert_overflow @ 0x00AFE6F0 (calls 0x00AFE749 / 0x00AFE796) and _M_fill_insert @
//   0x00AFE7C0 (calls 0x00AFE83D / 0x00AFE897) -- the growth chain of CParticle::PrimIDList (CArray<CArray<long> >).
// Callees: malloc @ 0x00BFEA0E and memmove @ 0x00BFEAE6 (the inline vector copy constructor).
// Shape: STLport `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first); return __cur;` with
//   the placement null test and CArray<long>'s implicit copy constructor (the STLport vector copy) inline; eax is the
//   loop's zero register, so an empty source hands the zero straight to the stores.
// Label: catalog "Vector_CopyVectors" is behavioural.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace _STL {

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

} // namespace _STL

template <class T> class CArray : public _STL::vector<T> {   // Fable's CArray: the retail 12-byte STLport vector
};

namespace _STL {
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CArray<long>* __uninitialized_copy<CArray<long>*, CArray<long>*>(CArray<long>*, CArray<long>*, CArray<long>*,
                                                                          const __false_type&);
} // namespace _STL
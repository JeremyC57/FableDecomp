// std::__uninitialized_copy(const vector<CCard*>* first, const vector<CCard*>* last, vector<CCard*>* result, const __false_type&) -- STLport copy-construct a range of card-pointer vectors (the blackjack hands) into raw storage: each copy allocates exactly the source size and memmoves the pointers; returns the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DC500, 115 bytes (+13 int3), /Gr __fastcall (first in ecx, last in edx,
//   result and the tag reference on the stack, `ret 8`).
// Identity: the vector<vector<CCard*> > growth/copy helper of the CTCTavernGameCardBlackjack TU (PlayersCards; the
//   element copy is vector<CCard*>'s). The catalog label "std_vector_DeepCopy" is descriptive.
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6.
// Shape: STLport `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first); return __cur;` over the
//   inline vector copy constructor (the landed-shape 0x007ACEA0 `_Vector_base(__x.size(), a)` with the zeroed words,
//   `allocate(n)` = malloc(n * 4) or null reusing the zero register) whose element copy is the trivial
//   __uninitialized_copy (`__copy_trivial`: memmove unless empty, result advanced by the byte count).
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

class CCard;

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
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
template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};

template vector<CCard*>* __fastcall __uninitialized_copy<const vector<CCard*>*, vector<CCard*>*>(
    const vector<CCard*>*, const vector<CCard*>*, vector<CCard*>*, const __false_type&);
}
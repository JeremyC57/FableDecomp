// std::__uninitialized_fill_n(vector<CCard*>* first, unsigned int n, const vector<CCard*>& x, const __false_type&) -- STLport construct n copies of one card-pointer vector (a blackjack hand) into raw storage: each copy allocates exactly x's size and memmoves the pointers; returns the end of the copies
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DC580, 127 bytes (+1 int3), /Gr __fastcall (first in ecx, n in edx, the
//   value and the tag references on the stack, `ret 8`).
// Identity: the vector<vector<CCard*> > fill helper of the CTCTavernGameCardBlackjack TU (PlayersCards), next to the
//   range copy 0x007DC500. The catalog label "Vector_Allocate_Triplets" is descriptive.
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6.
// Shape: STLport `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return __cur;` (unsigned count: `jbe`,
//   the count spilled to the `push ecx` word) over the inline vector copy constructor of 0x007DC500.
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

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
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

template vector<CCard*>* __fastcall __uninitialized_fill_n<vector<CCard*>*, unsigned int, vector<CCard*> >(
    vector<CCard*>*, unsigned int, const vector<CCard*>&, const __false_type&);
}
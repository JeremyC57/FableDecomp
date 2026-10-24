// _STL::vector<CCharString>::vector(const vector& x) -- STLport vector copy constructor for CCharString elements: allocate exactly x.size() slots (none when empty), then copy-construct x's strings into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ACEA0, 106 bytes, thiscall, `ret 4`, returns this.
// Callees: CRT malloc 0x00BFEA0E (`add esp,4`), CCharString::CCharString(const CCharString&) 0x0099EC30 (out of line).
// Shape: STLport 4.x `vector(const _Self& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, __false_type()); }` (the landed vector<C3DVector> copy
//   constructor 0x006F0810 with a 4-byte element): x.size() is `sar 2`, `_Vector_base(n, a)` zeroes the three words and
//   then `allocate(n)` (malloc(n * 4) or null); the __uninitialized_copy loop is inline with the placement-new null test
//   before each out-of-line CCharString copy constructor call. The catalog's "Vector_Copy" is descriptive.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
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
    vector(const vector& x);
};

template <class T> vector<T>::vector(const vector& x)
    : _Vector_base<T>(x.size(), x.get_allocator())
{
    this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
}

template class vector<CCharString>;
}
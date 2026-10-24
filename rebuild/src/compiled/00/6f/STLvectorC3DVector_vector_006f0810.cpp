// _STL::vector<C3DVector>::vector(const vector& x) -- STLport vector copy constructor for 12-byte C3DVector points: allocate exactly x.size() elements (none when empty), then copy-construct x's range into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F0810, 137 bytes, thiscall, `ret 4`, returns this.
// Identity: 76 call sites (0x006EADCA, 0x006F3037, 0x0075DEBB, ...); the landed CTCShapeManager::InitialiseFrom 0x00762AB0
//   and NParticleEngine::CPSCEmitterGeneric::WriteBinary 0x00B12EF0 declare it as the vector<C3DVector> copy
//   constructor. The 12-byte element has a user copy constructor (STLport's __false_type arm: a placement-new loop with
//   a null test, not a memmove). The catalog's "Vector_CopyFrom" is descriptive.
// Callees: CRT malloc 0x00BFEA0E (`add esp,4`).
// Shape: STLport 4.x `vector(const _Self& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, __false_type()); }` -- x.size() is the 0x2AAAAAAB
//   reciprocal divide by 12, `_Vector_base(n, a)` zeroes the three words then `allocate(n)` (malloc(n * 12) or null),
//   and the __uninitialized_copy loop (three dword moves per element) is inline.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

#pragma pack(push, 1)
struct C3DVector {                                          // 1-aligned in retail (CProng::Prong sits at +1)
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

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

template <class T> struct allocator {                       // STLport allocator<T>: empty
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> struct _Vector_base {
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

template <class T> struct vector : _Vector_base<T> {
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x);
};

template <class T> vector<T>::vector(const vector& x)
    : _Vector_base<T>(x.size(), x.get_allocator())
{
    this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
}

template struct vector<C3DVector>;
}
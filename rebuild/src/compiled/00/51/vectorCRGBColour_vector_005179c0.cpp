// vector<CRGBColour>::vector(unsigned int n) -- STLport sized constructor: allocate n colours and fill them with copies of a default-constructed (uninitialised) CRGBColour
// Leaf: ??0?$vector@VCRGBColour@@
//
// Retail 0x005179C0, 78 bytes (+ int3 padding to 80), thiscall `ret 4`.  Callees: 0x00BFEA0E malloc, 0x00513B00 __uninitialized_fill_n(first,
//   n, const CRGBColour& x, const __false_type&) (__fastcall, the colour copied channel by channel).
// Caller: CWorldMap::ShowPassabilityOverlay 0x004FF570 (a width * height colour buffer).
// Shape: STLport `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }` with `_Vector_base(n, a) : _M_start(0), _M_finish(0),
//   _M_end_of_storage(a, 0) { _M_start = allocate(n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + n; }`
//   (allocate = `n ? malloc(n * 4) : 0`).  CRGBColour's default constructor is user-written and empty, so `_Tp()` stores
//   nothing; that temporary and the `__false_type()` tag are both homed in the dead n slot (`lea edx,[esp+0Ch]`,
//   `lea ecx,[esp+10h]` after one push).
// Lever: __uninitialized_fill_n gets a same-TU __declspec(noinline) body (STLport's `for (; n > 0; --n, ++cur)
//   _Construct(&*cur, x)`, tag unnamed and unused).  With only a declaration the CRGBColour temporary takes its own
//   frame slot (`push ecx`, 80 v 78 bytes); with the body VC sees neither reference escapes and overlays both on n.
//   The copy constructor is the landed 0x0046916B model (channels copied one by one, as 0x00513B00 does).
// Model: Ego_r.pdb CRGBColour = union { struct { uchar B, G, R, A; }; } (4 bytes), FableWin.pdb ??0CRGBColour@@QAE@XZ.
// Label: catalog "std_vector_ulong_Initialize" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x00513B00 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() {}
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
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};
}

template std::vector<CRGBColour>::vector(unsigned int);
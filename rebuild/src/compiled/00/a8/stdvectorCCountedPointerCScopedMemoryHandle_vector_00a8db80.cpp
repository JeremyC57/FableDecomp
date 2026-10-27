// std::vector<CCountedPointer<CScopedMemoryHandle>>::vector(unsigned int n) -- n default (zero) elements in freshly allocated storage
// Leaf: ??0?$vector@V?$CCountedPointer@VCScopedMemoryHandle@@@@V?$allocator@V?$CCountedPointer@VCScopedMemoryHandle@@@@@std@@@std@@QAE@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8DB80, 90 bytes, thiscall, `ret 4` (speed-optimised: the `lea ebx,[ebx]` loop alignment).
// Identity (probable): called by 0x00A8BFF0 (C3DMesh2 vertex code, which then allocates 8-byte {0, -1} handles and hands them to counted pointers) and through the out-of-line CArray constructor 0x00A8DCF0; FableWin.pdb's C3DMesh2 translation unit ends with ??0?$CArray@V?$CCountedPointer@VCScopedMemoryHandle@@@@@@QAE@I@Z and its vector<CCountedPointer<CScopedMemoryHandle> >::vector(unsigned int), the only 8-byte sized constructors there.
// Callees: malloc 0x00BFEA0E (the STLport allocator's allocate for n != 0).
// Shape (family M110, representative 0x00515930): STLport 4.6 `explicit vector(size_type __n) : _Vector_base(__n,
//   allocator_type()) { _M_finish = uninitialized_fill_n(_M_start, __n, _Tp()); }` -- the base zeroes its three words,
//   allocates and sets start/finish/end; the non-POD fill placement-copies the folded zero temporary (`cmp eax,0 ; je`
//   per element: placement new's null test).
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CScopedMemoryHandle;
void __cdecl operator delete(void* block);                // 0x00BFE9BC
class CCPPointerInfo { public:
    unsigned long refs;                                // +0x0
    void (__fastcall* destroy)(void*);                 // +0x4
    void* object;                                      // +0x8
};
template <class T> class CCountedPointer {             // the landed 0x00514790 model
public:
    CCountedPointer() : object(0), control(0) { }
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control)
    {
        if (control)
            ++control->refs;
    }
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
        }
    }
    T*              object;                            // +0x0
    CCPPointerInfo* control;                           // +0x4
};
namespace std {
template <class _T1, class _T2> struct pair {
    pair() : first(_T1()), second(_T2()) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
    _T1 first;
    _T2 second;
};
struct __false_type { };
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};
template class vector<CCountedPointer<CScopedMemoryHandle> >;
}
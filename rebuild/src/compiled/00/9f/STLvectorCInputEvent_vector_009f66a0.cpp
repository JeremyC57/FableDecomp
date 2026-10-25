// std::vector<CInputEvent>::vector(const vector& x) -- STLport vector copy constructor for 52-byte input events: allocate exactly x.size() slots (none when empty), then copy-construct x's events into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F66A0, 128 bytes, thiscall, `ret 4`, returns this.
// Callees: CRT malloc 0x00BFEA0E (`add esp,4`), CInputEvent::CInputEvent(const CInputEvent&) 0x009E4850 (out of line).
// Callers: 0x009F5438 (CInputManager), 0x009F69B9 / 0x009F6A18, 0x009F6C27 / 0x009F6CAA (the element copies of a vector of
//   records that each hold a vector<CInputEvent>, e.g. CInputEventPackage). The chunk label "Array_CopyConstruct" is descriptive.
// Shape: STLport 4.x `vector(const _Self& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, __false_type()); }` -- the landed vector<CCharString> copy
//   constructor 0x007ACEA0 with a 0x34-byte element: x.size() is the signed /0x34 (0x4EC4EC4F multiply), `_Vector_base(n, a)`
//   zeroes the three words (`mov eax,0` keeps the size's flags for the `je`) and then `allocate(n)` (malloc(n * 0x34) or null);
//   the __uninitialized_copy loop is inline with the placement-new null test before each out-of-line copy constructor call.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CInputEvent {                                         // 0x34 bytes
public:
    long m_words[13];
    CInputEvent(const CInputEvent& other);                  // 0x009E4850
};

namespace std {

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

template class vector<CInputEvent>;
}
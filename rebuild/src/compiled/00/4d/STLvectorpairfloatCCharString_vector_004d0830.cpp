// _STL::vector<pair<float,CCharString>>::vector(const vector& x) -- STLport vector copy constructor for the CSubtitledMessage text map's 8-byte pair<float, CCharString> elements: allocate exactly x.size() elements (none when empty), then __uninitialized_copy x's range into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0830, 87 bytes, `ret 4`.
// Callers: 0x004D18C8, 0x004D1E99, 0x004D202F (vector-map copies in the same TU).
// Callees: CRT malloc 0x00BFEA0E (`add esp,4`); __uninitialized_copy(const pair*, const pair*, pair*, const __false_type&)
//   0x004CF9D0 (`__fastcall`, first ecx / last edx, result + tag on the stack, `ret 8`: the key dword then the
//   CCharString copy constructor 0x0099EC30 per element; its `_Uninit_copy<...unsigned_int...>` label is propagated).
// Label correction: the catalog's `Vector_Copy_Uint` is descriptive and wrong about the element (8 bytes, `sar 3`).
// Shape: STLport `vector(const _Self& __x) : _Vector_base(__x.size(), __x.get_allocator())` then
//   `_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, _M_start, _IsPODType())`.
//   The base constructor nulls start/finish and the end-of-storage proxy (one shared zero register), `allocate(n)` is
//   `n != 0 ? malloc(n * 8) : 0`, then start = finish = p, end = p + n. The empty `__false_type()` tag is homed in the
//   dead `x` argument slot (`lea edx,[esp+10h]`), x itself stays in ebx.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

namespace _STL {

struct __false_type { __false_type() { } };

struct CCharString {                            // 4 bytes: the string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<float, CCharString> SFloatStringPair;   // 8 bytes

SFloatStringPair* __fastcall __uninitialized_copy(const SFloatStringPair* first, const SFloatStringPair* last,
                                                  SFloatStringPair* result, const __false_type&);   // 0x004CF9D0

template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> struct _Vector_base {
    T*                    _M_start;             // +0
    T*                    _M_finish;            // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;    // +8
    _Vector_base(size_t n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> struct vector : _Vector_base<T> {
    size_t size() const { return size_t(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start,
                                               __false_type());
    }
};

template vector<SFloatStringPair>::vector(const vector<SFloatStringPair>&);

}
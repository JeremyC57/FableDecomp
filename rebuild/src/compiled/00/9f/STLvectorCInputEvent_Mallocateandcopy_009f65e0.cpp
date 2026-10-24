// _STL::vector<CInputEvent>::_M_allocate_and_copy(unsigned int n, const CInputEvent* first, const CInputEvent* last) -- STLport: malloc n input-event slots (none for zero), copy-construct [first, last) into them, return the block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F65E0, length 77 bytes, `ret 0Ch` (thiscall: n, first, last; `this` unused).
// Callees: malloc @ 0x00BFEA0E, CInputEvent::CInputEvent(const CInputEvent&) @ 0x009E4850.
// Caller: vector<CInputEvent>::operator= @ 0x009F6890 (landed as CElement34Vector::operator=, call site 0x009F68E4).
//   The chunk label "CInputEvent_AllocateCopy" is descriptive.
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
//   _Construct inlined (`new (p) T(value)`: the placement-new null test before the out-of-line copy constructor) -- the same
//   body as the landed vector<CCharString>::_M_allocate_and_copy 0x004B83E0 with a 0x34-byte element.
// Shape: default /O2 TU (`add esp,4` after malloc); VC strength-reduces the destination to `edi = result - first`,
//   `lea ecx,[edi+esi]` per element.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CInputEvent {                              // 0x34 bytes
public:
    long m_words[13];
    CInputEvent(const CInputEvent& other);       // 0x009E4850
};

namespace _STL {
struct __false_type { __false_type() {} };

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> class allocator {
public:
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);
};

template <class T, class A> T* vector<T, A>::_M_allocate_and_copy(unsigned int n, const T* first, const T* last)
{
    T* result = _M_end_of_storage.allocate(n);
    __uninitialized_copy(first, last, result, __false_type());
    return result;
}

template class vector<CInputEvent, allocator<CInputEvent> >;
}
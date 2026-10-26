// std::vector<CCharString>::_M_allocate_and_copy(unsigned int n, const CCharString* first, const CCharString* last) -- STLport: malloc n CCharString slots (none for zero), copy-construct [first, last) into them, return the block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B83E0, length 77 bytes, `ret 0Ch` (thiscall: n, first, last).
// Callees: malloc @ 0x00BFEA0E, CCharString copy constructor @ 0x0099EC30.
// Label: the catalog "Vector_Allocate_And_CopyConstruct" is behavioural; its caller 0x004BA7A0 is
// vector<CCharString>::reserve (it calls this and _M_clear 0x00433040, the landed CCharStringVector clear).
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type)
// with _Construct inlined (`new (p) T(value)`: the placement-new null test before the out-of-line copy ctor).
// Shape: default /O2 TU (`add esp,4` after malloc); VC strength-reduces the destination to `edi = result - first`,
// `lea ecx,[edi+esi]` per element.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData;
class CCharString {
public:
    CCharString(const CCharString& other);      // 0x0099EC30
private:
    CCharStringData* m_pData;
};

namespace std {
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

template <class T> struct allocator {
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

template class vector<CCharString, allocator<CCharString> >;
}
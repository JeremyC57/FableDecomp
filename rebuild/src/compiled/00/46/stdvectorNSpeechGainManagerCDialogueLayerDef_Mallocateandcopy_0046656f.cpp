// std::vector<NSpeechGainManager::CDialogueLayerDef>::_M_allocate_and_copy(unsigned int n, const CDialogueLayerDef* first, const CDialogueLayerDef* last) -- STLport: malloc n 64-byte dialogue layer defs (none for zero), copy-construct [first, last) into them, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046656F, length 71 bytes, `ret 0Ch` (thiscall: n, first, last).
// Callees: malloc @ 0x00BFEA0E, NSpeechGainManager::CDialogueLayerDef::CDialogueLayerDef(const CDialogueLayerDef&) @ 0x004663E7
// (landed).
// Label: the catalog "CDialogueLayerDef_AllocateAndConstruct" is behavioural. The single caller is
// vector<CDialogueLayerDef>::operator= (landed 0x00466281, which names this callee as _M_allocate_and_copy).
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
// _Construct inlined (`new (p) T(value)`: the null test, then the out-of-line copy ctor). CDialogueLayerDef is the
// polymorphic 64-byte def (vptr + CDefClassBase + six floats) of the landed copy ctor.
// Shape: `shl eax,6`, `pop ecx` after malloc = size TU (#pragma optimize("s", on)); the result rides in ebx.
#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace NSpeechGainManager {
class CDialogueLayerDef { public:                         // 64 bytes: vptr, CDefClassBase body, six float parameters
    virtual void v00();
    unsigned long _body[15];
    CDialogueLayerDef(const CDialogueLayerDef& other);   // 0x004663E7
};
}

namespace std {
struct __false_type { };

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

template class vector<NSpeechGainManager::CDialogueLayerDef, allocator<NSpeechGainManager::CDialogueLayerDef> >;
}
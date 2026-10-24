// std::vector<std::pair<unsigned long, CSoundPair> >::_M_allocate_and_copy(unsigned int n, const pair* first, const pair* last) -- STLport: malloc n 12-byte (key, sound pair) entries (none for zero), placement-copy [first, last) into them, return the new block
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D125, length 80 bytes, `ret 0Ch` (thiscall: n, first, last).
// Callees: malloc @ 0x00BFEA0E.
// Label: the catalog "CActiveFile_AssignVector12" is behavioural. The single caller is the vector's operator= @ 0x0045CFDE,
// called from CSoundMap::operator= @ 0x0045CFAD for SoundMap, a CVectorMap<unsigned long, CSoundPair> (the PDB CVectorMap
// is a vector of pair<key, value> plus Compare/Dirty bytes: retail copies the byte at +0x0D right after); PDB CSoundPair
// is 8 bytes (FirstSound, LastSound).
// Model: STLport 4.x _M_allocate_and_copy = allocate(n) + __uninitialized_copy(first, last, result, __false_type) with
// _Construct inlined (`new (p) T(value)`: the null test); STLport's pair has an explicit memberwise copy ctor, so the
// element copies as three dword moves through one register instead of a `rep movsd` block.
// Shape: `pop ecx` after malloc = size TU (#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CSoundPair {                                // PDB 8 bytes
    long FirstSound;                               // +0x00
    long LastSound;                                // +0x04
};
namespace std {
template <class T1, class T2> struct pair {        // STLport pair: explicit memberwise copy ctor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
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

template class vector<pair<unsigned long, CSoundPair>, allocator<pair<unsigned long, CSoundPair> > >;
}
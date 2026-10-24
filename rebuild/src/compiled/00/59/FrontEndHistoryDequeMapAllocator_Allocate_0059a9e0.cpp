// std::allocator<NUISystem::CComponent**>::allocate(size_type n, const void* hint) -- deque map allocation
// Leaf: allocate
// Retail: Fable.exe @ 0x0059A9E0, 25 bytes, __thiscall `ret 8` (the allocator `this` is unused, the hint ignored).
// STLport 4.6 allocator::allocate: `n != 0 ? (T*)allocate(n * sizeof(T)) : 0`, the raw allocation being the CRT
// malloc import thunk (0x00BFEA0E). n * sizeof(T*) is `shl eax,2`. This is the map allocator of the CFrontEndManager
// screen-history deque (blocks of 32 CComponent* at 0x0059A929 use the sizeof 24 sibling).
// Opt: size TU (`pop ecx` after the cdecl call), as the deque code around it.
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E (import thunk)
namespace NUISystem { class CComponent; }
namespace std {
template <class T> class allocator {
public:
    typedef T* pointer;
    typedef size_t size_type;
    pointer allocate(size_type n, const void* = 0)
    {
        return n != 0 ? (pointer)malloc(n * sizeof(T)) : 0;
    }
};
template class allocator<NUISystem::CComponent**>;
}

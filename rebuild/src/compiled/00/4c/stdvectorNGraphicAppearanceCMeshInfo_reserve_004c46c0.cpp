// std::vector<NGraphicAppearance::CMeshInfo>::reserve(unsigned int n) -- STLport: grow the mesh-info storage to n slots when the capacity is smaller (copy into a fresh block, free the old one)
// Leaf: reserve
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C46C0, length 209 bytes, `ret 4` (thiscall: n).
// Callees: malloc @ 0x00BFEA0E, free @ 0x00BFEA14, __uninitialized_copy(const CMeshInfo*, const CMeshInfo*, CMeshInfo*,
// const __false_type&) @ 0x004C2AD0 (__fastcall /Gr; the catalog "_Uninit_copy<...unsigned_int...>").
// Label: the catalog "VectorReserve" is behavioural; the landed CTCGraphicAppearance::GetAllMeshes 0x004BD420 /
// GetAllHelperPointNames 0x004BDA30 / HasDummyObject 0x004BE2C0 call it as vector<CMeshInfo>::reserve (16 callers).
// Element (Ego_r.pdb): NGraphicAppearance::CMeshInfo { long Index; float SizeX; uchar PrimaryMesh:1; } -- 12 bytes
// (the `imul 2AAAAAABh ; sar edx,1` divides by 12).
// Shape: STLport 4.x reserve with _M_allocate_and_copy and _M_clear inlined: `if (capacity() < n) { old = size();
// tmp = start ? (allocate(n), __uninitialized_copy(start, finish, tmp, __false_type()), free(start)) : allocate(n);
// _M_set(tmp, tmp + old, tmp + n); }`. The empty __false_type temporary is homed over the live `n` argument slot
// (`lea edx,[esp+18h]`), which VC7.1 does only when the copy body is defined out of line in the TU (stand-in below).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace NGraphicAppearance {
class CMeshInfo {                                // Ego_r.pdb, 0xc bytes
public:
    long          Index;                         // +0x0
    float         SizeX;                         // +0x4
    unsigned char PrimaryMesh : 1;               // +0x8
};
}

namespace std {
struct __false_type { __false_type() {} };

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

// Same-TU stand-in for the out-of-line copy 0x004C2AD0 (its body: placement copy with the null test).
#pragma auto_inline(off)
__declspec(noinline) NGraphicAppearance::CMeshInfo* __fastcall __uninitialized_copy(const NGraphicAppearance::CMeshInfo* first,
    const NGraphicAppearance::CMeshInfo* last, NGraphicAppearance::CMeshInfo* result, const __false_type&)
{
    NGraphicAppearance::CMeshInfo* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
#pragma auto_inline(on)

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    __forceinline void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class T> __forceinline void _Destroy(T*, T*) {}

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    __forceinline unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    __forceinline unsigned int capacity() const { return unsigned int(_M_end_of_storage._M_data - _M_start); }
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    __forceinline T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last)
    {
        T* result = _M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    __forceinline void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, unsigned int(_M_end_of_storage._M_data - _M_start));
    }
    void reserve(unsigned int n);
};

template <class T, class A> void vector<T, A>::reserve(unsigned int n)
{
    if (capacity() < n) {
        const unsigned int old_size = size();
        T* tmp;
        if (_M_start) {
            tmp = _M_allocate_and_copy(n, _M_start, _M_finish);
            _M_clear();
        }
        else {
            tmp = _M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}

template void vector<NGraphicAppearance::CMeshInfo, allocator<NGraphicAppearance::CMeshInfo> >::reserve(unsigned int);
}
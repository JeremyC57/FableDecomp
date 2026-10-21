// std::_Deque_base<C2DVector>::_M_create_nodes(T** nstart, T** nfinish) -- mallocs one 128-byte block per map slot
// Leaf: _M_create_nodes
// Retail: Fable.exe @ 0x00654E46, 33 bytes, `ret 8` (this unused). Catalog label CBasicString<char>::Fill is wrong: the
// only caller is a deque _M_initialize_map 0x00655516 (`shr ebx,4`: 16 elements per block, so 8-byte elements). The
// element type is not pinned down; Ego_r.pdb's deque<C2DVector> is an 8-byte candidate and stands for it here.
// STLport: for each map slot, allocate(buffer_size()) through the malloc allocator (0x00BFEA0E, `push 80h`).
// Opt: size TU (loop entered by a jump to its test, `pop ecx` after the cdecl malloc).
#pragma optimize("s", on)
#include <stdlib.h>
class C2DVector { public: float X, Y; };
namespace std {
template <class T> class __malloc_node_allocator {      // STLport allocator over malloc/free (0x00BFEA0E / 0x00BFEA14)
public:
    T* allocate(size_t n) { return (T*)malloc(n * sizeof(T)); }
    void deallocate(T* p, size_t) { if (p) free(p); }
};
inline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> class _Deque_base {
public:
    static size_t buffer_size() { return __deque_buf_size(sizeof(T)); }
    unsigned char             _M_start[0x10];          // +0x00 iterator
    unsigned char             _M_finish[0x10];         // +0x10 iterator
    T**                       _M_map;                  // +0x20
    __malloc_node_allocator<T> _M_node_alloc;
    size_t                    _M_map_size;             // +0x24
    void _M_create_nodes(T** nstart, T** nfinish)
    {
        T** cur;
        for (cur = nstart; cur < nfinish; ++cur)
            *cur = _M_node_alloc.allocate(buffer_size());
    }
    void _M_destroy_nodes(T** nstart, T** nfinish)
    {
        for (T** n = nstart; n < nfinish; ++n)
            _M_node_alloc.deallocate(*n, buffer_size());
    }
};
template void _Deque_base<C2DVector>::_M_create_nodes(C2DVector**, C2DVector**);
}
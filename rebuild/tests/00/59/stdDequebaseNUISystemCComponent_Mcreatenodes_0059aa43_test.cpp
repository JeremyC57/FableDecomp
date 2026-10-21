// behaviour test for 0x0059AA43
#include <stdio.h>
#include <stdlib.h>
namespace NUISystem { class CComponent; }
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
template void _Deque_base<NUISystem::CComponent*>::_M_create_nodes(NUISystem::CComponent***, NUISystem::CComponent***);
}

int main()
{
    std::_Deque_base<NUISystem::CComponent*> d; NUISystem::CComponent** map[4] = { 0, 0, 0, 0 };
    d._M_create_nodes(map + 1, map + 3);
    int ok = map[0] == 0 && map[1] != 0 && map[2] != 0 && map[3] == 0 && std::_Deque_base<NUISystem::CComponent*>::buffer_size() * sizeof(NUISystem::CComponent*) == 128;
    free(map[1]); free(map[2]);
    map[2] = 0; d._M_create_nodes(map + 2, map + 2); ok &= map[2] == 0;
    printf(ok ? "0059aa43_TEST PASS\n" : "0059aa43_TEST FAIL\n");
    return ok ? 0 : 1;
}
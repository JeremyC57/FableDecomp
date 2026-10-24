// Grade: DIFFER(33v34) -- catalog row also holds the separate 1-byte `ret` function at 0x0059AA85; this source is a RELOCATION_MATCH against the 33-byte retail body (qx.py). Re-bound the row to 33 bytes, then land.
// std::_Deque_base<NUISystem::CComponent*>::_M_destroy_nodes(_Map_pointer start, _Map_pointer finish) -- free every block
// Leaf: _M_destroy_nodes
// Retail: Fable.exe @ 0x0059AA64, 33 bytes, __thiscall `ret 8`: for each map slot in [start, finish) the block is
// released through allocator::deallocate, which is `if (p) free(p)` (CRT free import thunk 0x00BFEA14).
// Catalog row: 34 bytes -- it also swallows the separate 1-byte function at 0x0059AA85 (a lone `ret`), so no
// single compiled symbol can equal the catalog oracle; this source is the 33-byte body.
// Opt: size TU (`pop ecx` after the cdecl call; unrotated `jmp` to the loop test).
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void __cdecl free(void* p);   // 0x00BFEA14 (import thunk)
namespace NUISystem { class CComponent; }
namespace std {
template <class T> class allocator {
public:
    void deallocate(T* p, size_t) { if (p) free(p); }
};
template <class T> class _Deque_base {
public:
    typedef T** _Map_pointer;
    void _M_destroy_nodes(_Map_pointer start, _Map_pointer finish);
    allocator<T> _M_data_allocator;
};
template <class T> void _Deque_base<T>::_M_destroy_nodes(_Map_pointer start, _Map_pointer finish)
{
    for (_Map_pointer n = start; n < finish; ++n)
        _M_data_allocator.deallocate(*n, 32);
}
template class _Deque_base<NUISystem::CComponent*>;
}

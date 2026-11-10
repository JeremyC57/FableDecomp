// Behaviour test for CHandleBasedMemoryPool_012b4098::InternalGetResource (0x00C28560): the resource pointer stored in the
// header of the area the handle table (+0x440) maps the handle to.
#include <cstdio>
struct CHeader { void* Resource; long Handle; long Size; };
struct CPool {
    CHeader** Handles;
    void* InternalGetResource(long handle) { return Handles[handle]->Resource; }
};
int main()
{
    int a = 0, b = 0;
    CHeader h[2] = { { &a, 0, 0 }, { &b, 1, 0 } };
    CHeader* table[3] = { &h[1], &h[0], &h[1] };
    CPool pool; pool.Handles = table;
    bool ok = pool.InternalGetResource(0) == &b && pool.InternalGetResource(1) == &a && pool.InternalGetResource(2) == &b;
    if (!ok) { std::printf("00c28560_TEST FAIL\n"); return 1; }
    std::printf("00c28560_TEST PASS\n");
    return 0;
}
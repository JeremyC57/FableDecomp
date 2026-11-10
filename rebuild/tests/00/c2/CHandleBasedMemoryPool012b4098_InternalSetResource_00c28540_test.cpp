// Behaviour test for CHandleBasedMemoryPool_012b4098::InternalSetResource (0x00C28540): the handle indexes the handle table
// (+0x440) and the resource pointer is stored in that area header's Resource (+0x00); other headers are untouched.
#include <cstdio>
struct CHeader { void* Resource; long Handle; long Size; };
struct CPool {
    CHeader** Handles;
    void InternalSetResource(long handle, void* resource) { Handles[handle]->Resource = resource; }
};
int main()
{
    CHeader h[3] = { { 0, 0, 0 }, { 0, 1, 0 }, { 0, 2, 0 } };
    CHeader* table[3] = { &h[0], &h[1], &h[2] };
    CPool pool; pool.Handles = table;
    int a = 0, b = 0;
    pool.InternalSetResource(1, &a);
    pool.InternalSetResource(2, &b);
    bool ok = h[0].Resource == 0 && h[1].Resource == &a && h[2].Resource == &b && h[1].Handle == 1;
    if (!ok) { std::printf("00c28540_TEST FAIL\n"); return 1; }
    std::printf("00c28540_TEST PASS\n");
    return 0;
}
// Test: CHandleBasedMemoryPool::GetPointerFromHandle -- the data sits right after the 32-byte area header the handle names.
#include <cstdio>
struct CHeader { int w[8]; };
struct CPool { CHeader** Handles; void* GetPointerFromHandle(int handle) { return Handles[handle] + 1; } };
int main()
{
    CHeader areas[3]; CHeader* table[3] = { &areas[0], &areas[2], &areas[1] }; CPool p; p.Handles = table;
    bool ok = sizeof(CHeader) == 32 && p.GetPointerFromHandle(1) == (char*)&areas[2] + 32 && p.GetPointerFromHandle(0) == &areas[1];
    std::printf(ok ? "00c28380_TEST PASS\n" : "00c28380_TEST FAIL\n");
    return ok ? 0 : 1;
}

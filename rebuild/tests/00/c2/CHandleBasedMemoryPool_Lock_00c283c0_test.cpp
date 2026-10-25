#include <stdio.h>
struct CHeader { unsigned char _pad00[0xf]; unsigned char Locked; };
struct CHandleBasedMemoryPool {
    unsigned char _pad00[0x18]; long LockCount; unsigned char _pad1c[0x440 - 0x1c]; CHeader** Handles;
    void Lock(long handle) { CHeader** slot = &Handles[handle]; if ((*slot)->Locked == 0) { (*slot)->Locked = 1; LockCount++; } }
};
int main()
{
    CHandleBasedMemoryPool pool;
    for (int i = 0; i < (int)sizeof(pool); ++i) ((char*)&pool)[i] = 0;
    CHeader e0, e1; e0.Locked = 0; e1.Locked = 0;
    CHeader* tbl[4] = { &e0, &e1, &e0, &e1 };
    pool.Handles = tbl; pool.LockCount = 0;
    pool.Lock(0); if (!(e0.Locked == 1 && pool.LockCount == 1)) { printf("BAD\n"); return 1; }
    pool.Lock(2); if (!(e0.Locked == 1 && pool.LockCount == 1)) { printf("BAD\n"); return 1; }
    pool.Lock(1); if (!(e1.Locked == 1 && pool.LockCount == 2)) { printf("BAD\n"); return 1; }
    printf("LOCK_OK\n");
    return 0;
}

#include <cstdio>
struct CHeader { int x; };
union CHandleSlot { CHeader* Header; long NextFreeHandle; };
struct Pool {
    CHandleSlot* Handles; long FirstUnusedHandle;
    void FreeHandle(long handle)
    {
        CHandleSlot& slot = Handles[handle];
        slot.Header = 0;
        slot.NextFreeHandle = FirstUnusedHandle;
        FirstUnusedHandle = handle;
    }
};
int main()
{
    CHeader h[4]; CHandleSlot slots[4]; for (int i = 0; i < 4; ++i) slots[i].Header = &h[i];
    Pool p; p.Handles = slots; p.FirstUnusedHandle = -1;
    p.FreeHandle(2);
    if (p.FirstUnusedHandle != 2 || slots[2].NextFreeHandle != -1 || slots[1].Header != &h[1]) { std::printf("00c28610_TEST FAIL\n"); return 1; }
    p.FreeHandle(0);
    if (p.FirstUnusedHandle != 0 || slots[0].NextFreeHandle != 2 || slots[3].Header != &h[3]) { std::printf("00c28610_TEST FAIL\n"); return 1; }
    std::printf("00c28610_TEST PASS\n");
    return 0;
}
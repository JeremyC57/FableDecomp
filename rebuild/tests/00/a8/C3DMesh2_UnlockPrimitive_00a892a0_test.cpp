#include <cstdio>
static long g_unlocked[4]; static int g_n = 0;
class CHandleBasedMemoryPool { public: void Unlock(long h) { g_unlocked[g_n++] = h; } };
class C3DMesh2 { public: void* m_vptr; CHandleBasedMemoryPool* MemoryPool; unsigned char _pad_0x08[0x74]; long MemoryHandle; long PrimMemoryHandle; long BoneNameMemoryHandle;
    void UnlockPrimitive(long) { MemoryPool->Unlock(MemoryHandle); MemoryPool->Unlock(PrimMemoryHandle); if (BoneNameMemoryHandle != -1) MemoryPool->Unlock(BoneNameMemoryHandle); } };
int main()
{
    CHandleBasedMemoryPool pool; C3DMesh2 m; m.MemoryPool = &pool; m.MemoryHandle = 11; m.PrimMemoryHandle = 22; m.BoneNameMemoryHandle = 33;
    if ((char*)&m.MemoryHandle != (char*)&m + 0x7c) { std::printf("00a892a0_TEST FAIL\n"); return 1; }
    m.UnlockPrimitive(0);
    if (g_n != 3 || g_unlocked[0] != 11 || g_unlocked[1] != 22 || g_unlocked[2] != 33) { std::printf("00a892a0_TEST FAIL\n"); return 1; }
    g_n = 0; m.BoneNameMemoryHandle = -1; m.UnlockPrimitive(5);
    if (g_n != 2) { std::printf("00a892a0_TEST FAIL\n"); return 1; }
    std::printf("00a892a0_TEST PASS\n"); return 0;
}
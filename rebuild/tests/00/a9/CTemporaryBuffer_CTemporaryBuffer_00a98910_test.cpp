#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
class CHandleBasedMemoryPool { public:
    long lastHandle;
    void Unlock(long h) { lastHandle = h; std::strcat(g_seq, "U"); }
    void Free(long h) { if (h == lastHandle) std::strcat(g_seq, "F"); }
};
class CScratchBuffer { public: bool Locked; void Unlock() { Locked = false; std::strcat(g_seq, "S"); } };
namespace N_0099a300 { struct CBase { virtual ~CBase() { std::strcat(g_seq, "B"); } }; } using N_0099a300::CBase;
struct CTemporaryBuffer : CBase {
    void* Data; CHandleBasedMemoryPool* MemoryPool; long MemoryHandle; CScratchBuffer* ScratchBuffer; int AllocateType;
    ~CTemporaryBuffer()
    {
        switch (AllocateType) {
        case 0: MemoryPool->Unlock(MemoryHandle); MemoryPool->Free(MemoryHandle); break;
        case 1: ScratchBuffer->Unlock(); break;
        case 2: delete[] (char*)Data; std::strcat(g_seq, "D"); break;
        }
    }
};
int main()
{
    CHandleBasedMemoryPool pool; pool.lastHandle = 0; CScratchBuffer sb; sb.Locked = true;
    { CTemporaryBuffer t; t.Data = 0; t.MemoryPool = &pool; t.MemoryHandle = 9; t.ScratchBuffer = &sb; t.AllocateType = 0; }
    { CTemporaryBuffer t; t.Data = 0; t.MemoryPool = &pool; t.MemoryHandle = 9; t.ScratchBuffer = &sb; t.AllocateType = 1; }
    { CTemporaryBuffer t; t.Data = new char[8]; t.MemoryPool = &pool; t.MemoryHandle = 9; t.ScratchBuffer = &sb; t.AllocateType = 2; }
    { CTemporaryBuffer t; t.Data = 0; t.MemoryPool = &pool; t.MemoryHandle = 9; t.ScratchBuffer = &sb; t.AllocateType = 3; }
    if (std::strcmp(g_seq, "UFBSBDBB") || sb.Locked || pool.lastHandle != 9) { std::printf("00a98910_TEST FAIL\n"); return 1; }
    std::printf("00a98910_TEST PASS\n");
    return 0;
}
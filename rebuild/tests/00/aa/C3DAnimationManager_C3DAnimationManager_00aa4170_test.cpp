// Behaviour test for 0x00AA4170 C3DAnimationManager::~C3DAnimationManager (compiler-generated): MemoryPool2 then MemoryPool
// (each pool clears its memory blocks, then its base part), then the BoneIndicesBuffer block, then the manager's base part.
#include <cstdio>
#include <cstdlib>
static char g_log[32];
static int g_n = 0;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }

struct CBaseClassNonCopyable { char BaseTag; CBaseClassNonCopyable() : BaseTag('B') {} virtual ~CBaseClassNonCopyable() { Log(BaseTag); } };
class CSimpleMemoryPool : public CBaseClassNonCopyable { public:
    char Tag;
    CSimpleMemoryPool(char tag) : Tag(tag) { BaseTag = 'b'; }
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
    void ClearMemoryBlocks() { Log(Tag); }
};
struct CArray { unsigned long* Start; ~CArray() { if (Start) { Log('F'); std::free(Start); } } };
class C3DAnimationManager : public CBaseClassNonCopyable { public:
    C3DAnimationManager() : MeshBank(0), MemoryPool('1'), MemoryPool2('2') { BoneIndicesBuffer.Start = 0; }
    void* MeshBank;
    CArray BoneIndicesBuffer;
    CSimpleMemoryPool MemoryPool;
    CSimpleMemoryPool MemoryPool2;
};

int main()
{
    C3DAnimationManager* m = new C3DAnimationManager;
    m->BoneIndicesBuffer.Start = (unsigned long*)std::malloc(16);
    delete (CBaseClassNonCopyable*)m;
    const char* want = "2b1bFB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("00aa4170_TEST FAIL\n"); return 1; }
    std::printf("00aa4170_TEST PASS\n");
    return 0;
}
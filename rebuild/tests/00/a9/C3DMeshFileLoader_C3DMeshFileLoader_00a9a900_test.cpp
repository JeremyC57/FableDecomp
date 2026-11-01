// Test for C3DMeshFileLoader::~C3DMeshFileLoader() -- the 22 counted-pointer chunk-type members release their shared info in reverse declaration order (each zeroes Data then Info), then the CChunkedFileLoader base destructor runs.
#include <cstdio>
static int g_order[32]; static int g_n = 0; static bool g_baseLast = false;
class CCPPointerInfo { public: long RefCount; int id; };
struct CCountedPointer {
    void* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) g_order[g_n++] = Info->id;
        Data = 0; Info = 0;
    }
};
class CChunkedFileLoader { public:
    virtual ~CChunkedFileLoader() { g_baseLast = (g_n == 22); }
    int ReadUnknownChunkTypes;
};
struct C3DMeshFileLoader : CChunkedFileLoader {
    CCountedPointer Chunk[22];
    virtual ~C3DMeshFileLoader() { }
};
int main()
{
    static CCPPointerInfo infos[22];
    C3DMeshFileLoader* loader = new C3DMeshFileLoader;
    for (int i = 0; i < 22; ++i) { infos[i].RefCount = 1; infos[i].id = i; loader->Chunk[i].Info = &infos[i]; loader->Chunk[i].Data = &infos[i]; }
    CChunkedFileLoader* base = loader;
    delete base;
    int ok = (g_n == 22) && g_baseLast;
    for (int i = 0; i < 22 && ok; ++i) if (g_order[i] != 21 - i || infos[i].RefCount != 0) ok = 0;
    if (!ok) { std::printf("00a9a900_TEST FAIL\n"); return 1; }
    std::printf("00a9a900_TEST PASS\n");
    return 0;
}
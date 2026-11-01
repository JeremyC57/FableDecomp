// Behaviour test for C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk (00a9a7c0, compiler-generated): free the EnableMask block when non-null, then the base destructor (CChunkedFileChunk::~CChunkedFileChunk).
#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[32];
static int g_freed;
static void Log(const char* s) { std::strcat(g_log, s); }
static void TrackedFree(void* p) { if (p) { std::free(p); ++g_freed; Log("F"); } }
template <class T> struct VectorModel {                 // STLport vector: start, finish, end of storage
    T* Start; T* Finish; T* EndOfStorage;
    VectorModel() : Start(0), Finish(0), EndOfStorage(0) { }
    ~VectorModel() { if (Start != 0) TrackedFree(Start); }
};
struct BaseModel { virtual ~BaseModel() { Log("B"); } };
struct DerivedModel : BaseModel { VectorModel<int> EnableMask; };
int main()
{
    {
        DerivedModel d;
        d.EnableMask.Start = (int*)std::malloc(16); d.EnableMask.Finish = d.EnableMask.Start + 2; d.EnableMask.EndOfStorage = d.EnableMask.Start + 4;
    }
    if (std::strcmp(g_log, "FB") != 0 || g_freed != 1) { std::printf("00a9a7c0_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    { DerivedModel d; }
    if (std::strcmp(g_log, "B") != 0 || g_freed != 1) { std::printf("00a9a7c0_TEST FAIL\n"); return 1; }
    BaseModel* p = new DerivedModel;                   // destroyed through the virtual chain (the ??_G path)
    ((DerivedModel*)p)->EnableMask.Start = (int*)std::malloc(8);
    g_log[0] = 0;
    delete p;
    if (std::strcmp(g_log, "FB") != 0 || g_freed != 2) { std::printf("00a9a7c0_TEST FAIL\n"); return 1; }
    std::printf("00a9a7c0_TEST PASS\n");
    return 0;
}
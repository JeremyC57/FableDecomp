// Behaviour test for CTCHitOnCollision::~CTCHitOnCollision (007c8560, compiler-generated): free the CollidedList block when non-null, then the base destructor (CTCBase::~CTCBase).
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
struct DerivedModel : BaseModel { VectorModel<int> CollidedList; };
int main()
{
    {
        DerivedModel d;
        d.CollidedList.Start = (int*)std::malloc(16); d.CollidedList.Finish = d.CollidedList.Start + 2; d.CollidedList.EndOfStorage = d.CollidedList.Start + 4;
    }
    if (std::strcmp(g_log, "FB") != 0 || g_freed != 1) { std::printf("007c8560_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    { DerivedModel d; }
    if (std::strcmp(g_log, "B") != 0 || g_freed != 1) { std::printf("007c8560_TEST FAIL\n"); return 1; }
    BaseModel* p = new DerivedModel;                   // destroyed through the virtual chain (the ??_G path)
    ((DerivedModel*)p)->CollidedList.Start = (int*)std::malloc(8);
    g_log[0] = 0;
    delete p;
    if (std::strcmp(g_log, "FB") != 0 || g_freed != 2) { std::printf("007c8560_TEST FAIL\n"); return 1; }
    std::printf("007c8560_TEST PASS\n");
    return 0;
}
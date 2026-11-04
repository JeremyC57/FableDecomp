// Behaviour test for the CEngineSurfaceCompositionManager vector-deleting-destructor adjustor thunk 0x00B2F5D0: deleting the object through its
// secondary CEngineComponent subobject (+0x7c in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to the deleting destructor).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtors = 0;
static void* g_freed = 0;
struct PrimaryBase { virtual ~PrimaryBase() {} char state[124]; };
struct SecondaryBase { virtual ~SecondaryBase() {} int word; };
struct CEngineSurfaceCompositionManager : PrimaryBase, SecondaryBase {
    char own[0x10];
    ~CEngineSurfaceCompositionManager() { ++g_dtors; }
    static void* operator new(std::size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { g_freed = p; std::free(p); }
};
int main()
{
    CEngineSurfaceCompositionManager* object = new CEngineSurfaceCompositionManager;
    SecondaryBase* sub = object;
    if ((void*)sub == (void*)object) { std::printf("00b2f5d0_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1 || g_freed != (void*)object) { std::printf("00b2f5d0_TEST FAIL\n"); return 1; }
    std::printf("00b2f5d0_TEST PASS\n");
    return 0;
}
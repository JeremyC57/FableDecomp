// Behaviour test for CVtable_012b4f84::CreateCombatAction (0x00C3CD00): pooled mode reuses the pool head (moved to
// the active list on success, kept on failure); unpooled mode news an object, deletes it on a negative result;
// non-zero results are reported.
#include <cstdio>
#include <cstdlib>
#include <cstddef>

static int g_news = 0, g_deletes = 0;
struct CVtable_012b4f74 {
    virtual ~CVtable_012b4f74() {}
    static void* operator new(size_t size) { ++g_news; return std::malloc(size); }
    static void operator delete(void* block) { ++g_deletes; std::free(block); }
};
static long g_nextResult = 0; static long g_reported = 0; static int g_reports = 0;
class CVtable_012b4f84 : public CVtable_012b4f74 {
public:
    void* Params;
    CVtable_012b4f84() : Params(0) {}
    virtual ~CVtable_012b4f84() {}
    long Initialise(void* params) { Params = params; return g_nextResult; }
    static CVtable_012b4f84* CreateCombatAction(void* params);
};
struct CObjectPool_0143bc00 {
    long Mode; CVtable_012b4f84* Head; int Removes; CVtable_012b4f84* Added; int Adds;
    CVtable_012b4f84* GetHead() { return Head; }
    void RemoveHead() { ++Removes; }
    void Add(CVtable_012b4f84* object) { Added = object; ++Adds; }
};
CObjectPool_0143bc00 g_objectPool_0143bc00;
struct CResultLog_0143bb58 { void Report(long result) { g_reported = result; ++g_reports; } };
CResultLog_0143bb58 g_resultLog_0143bb58;
CVtable_012b4f84* CVtable_012b4f84::CreateCombatAction(void* params)
{
    CVtable_012b4f84* object;
    long result;
    if (g_objectPool_0143bc00.Mode >= 0) {
        CVtable_012b4f84* pooled = g_objectPool_0143bc00.GetHead();
        if (pooled == 0)
            goto failed;
        object = pooled;
    } else {
        object = new CVtable_012b4f84;
        if (object == 0)
            goto failed;
    }
    result = object->Initialise(params);
    if (result)
        g_resultLog_0143bb58.Report(result);
    if (result >= 0) {
        if (g_objectPool_0143bc00.Mode >= 0)
            g_objectPool_0143bc00.RemoveHead();
        g_objectPool_0143bc00.Add(object);
        return object;
    }
    if (g_objectPool_0143bc00.Mode < 0)
        delete object;
failed:
    return 0;
}
int main()
{
    int params = 0;
    CVtable_012b4f84 pooledObject;
    g_objectPool_0143bc00.Mode = 0; g_objectPool_0143bc00.Head = 0; g_objectPool_0143bc00.Removes = 0; g_objectPool_0143bc00.Adds = 0;
    if (CVtable_012b4f84::CreateCombatAction(&params) != 0 || g_objectPool_0143bc00.Adds) { std::printf("00c3cd00_TEST FAIL\n"); return 1; }
    g_objectPool_0143bc00.Head = &pooledObject;
    if (CVtable_012b4f84::CreateCombatAction(&params) != &pooledObject || g_objectPool_0143bc00.Removes != 1 || g_objectPool_0143bc00.Added != &pooledObject || pooledObject.Params != &params || g_reports) { std::printf("00c3cd00_TEST FAIL\n"); return 1; }
    g_nextResult = -2;
    if (CVtable_012b4f84::CreateCombatAction(&params) != 0 || g_reports != 1 || g_reported != -2 || g_deletes || g_objectPool_0143bc00.Removes != 1) { std::printf("00c3cd00_TEST FAIL\n"); return 1; }
    g_objectPool_0143bc00.Mode = -1;
    if (CVtable_012b4f84::CreateCombatAction(&params) != 0 || g_news != 1 || g_deletes != 1) { std::printf("00c3cd00_TEST FAIL\n"); return 1; }
    g_nextResult = 1;
    CVtable_012b4f84* made = CVtable_012b4f84::CreateCombatAction(&params);
    if (made == 0 || g_news != 2 || g_objectPool_0143bc00.Added != made || g_objectPool_0143bc00.Removes != 1 || g_reported != 1) { std::printf("00c3cd00_TEST FAIL\n"); return 1; }
    delete made;
    std::printf("00c3cd00_TEST PASS\n");
    return 0;
}

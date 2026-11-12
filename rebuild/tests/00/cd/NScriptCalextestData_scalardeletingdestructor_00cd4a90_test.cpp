// Behaviour test for 0x00CD4A90 NScript::CalextestData::`scalar deleting destructor' (agentFAM08 portable twin; the catalog test only printed
// its pass line). Model: the deleting destructor runs the destructor chain -- members in reverse
// declaration order (none), then the implicit ~CScriptDataBase / ~CBaseClassNonCopyable -- and frees the object only when bit 0 of the
// flags is set (`delete p`); the explicit destructor call (flags 0) leaves the storage alone. Test-only model types.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_seq[16];
static int g_count = 0;
static int g_frees = 0;
static void Record(int v) { if (g_count < 16) g_seq[g_count++] = v; }
struct ModelBase {
    virtual ~ModelBase() { Record(100); }
};
struct ModelObject : ModelBase {
    static void* operator new(std::size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++g_frees; std::free(p); }
};
static bool CheckSequence()
{
    static const int expect[] = { 100 };
    if (g_count != (int)(sizeof(expect) / sizeof(expect[0]))) return false;
    for (int i = 0; i < g_count; ++i) if (g_seq[i] != expect[i]) return false;
    return true;
}
int main()
{
    ModelObject* p = new ModelObject;
    if (!p) { std::printf("00cd4a90_TEST FAIL alloc\n"); return 1; }
    p->~ModelObject();                       // flags 0: destroy only
    if (!CheckSequence() || g_frees != 0) { std::printf("00cd4a90_TEST FAIL destroy\n"); return 1; }
    ::new (p) ModelObject;
    g_count = 0;
    ModelBase* b = p;
    delete b;                                // flags 1: destroy, then free
    if (!CheckSequence() || g_frees != 1) { std::printf("00cd4a90_TEST FAIL delete\n"); return 1; }
    std::printf("00cd4a90_TEST PASS\n");
    return 0;
}

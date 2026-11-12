// Behaviour test for 0x00E9C300 NScript::CV_HeroDuelScript::Alloc (agentFAM08 portable twin; the catalog test was a __fastcall
// shim). Model: the factory allocates the 0x5C-byte script, runs CScriptBase(), stores the game interface (Game) and the master data
// (MasterScriptData), and returns 0 when the allocation fails. Test-only model types.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_baseCtors = 0;
static bool g_failAlloc = false;
struct ModelGame { int id; };
struct ModelData { int id; };
struct ModelScriptBase { ModelScriptBase() { ++g_baseCtors; } virtual ~ModelScriptBase() { } };
struct ModelHeroDuelScript : ModelScriptBase {
    ModelGame* Game;
    ModelData* MasterScriptData;
    ModelHeroDuelScript(ModelData* data, ModelGame& game) : Game(&game), MasterScriptData(data) { }
    static void* operator new(std::size_t n) { return g_failAlloc ? 0 : std::malloc(n); }
    static void operator delete(void* p) { std::free(p); }
    static ModelScriptBase* Alloc(ModelData* data, ModelGame& game)
    {
        void* mem = operator new(sizeof(ModelHeroDuelScript));
        return mem ? ::new (mem) ModelHeroDuelScript(data, game) : 0;
    }
};
int main()
{
    ModelGame game = { 7 };
    ModelData data = { 9 };
    ModelScriptBase* s = ModelHeroDuelScript::Alloc(&data, game);
    ModelHeroDuelScript* h = static_cast<ModelHeroDuelScript*>(s);
    if (!s || g_baseCtors != 1 || h->Game != &game || h->MasterScriptData != &data) { std::printf("00e9c300_TEST FAIL alloc\n"); return 1; }
    delete h;
    g_failAlloc = true;
    if (ModelHeroDuelScript::Alloc(&data, game) != 0 || g_baseCtors != 1) { std::printf("00e9c300_TEST FAIL null\n"); return 1; }
    std::printf("00e9c300_TEST PASS\n");
    return 0;
}

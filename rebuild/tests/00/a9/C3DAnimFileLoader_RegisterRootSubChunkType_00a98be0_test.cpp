#include <cstdio>
// Behaviour model: a counted pointer to a fresh copy is registered (the registry keeps a reference) and returned; once the
// local is gone the block holds the registry's and the caller's references.
struct Block { long refs; void* obj; };
struct CP {
    void* obj; Block* blk;
    explicit CP(void* o) : obj(o), blk(o ? new Block() : 0) { if (blk) { blk->refs = 1; blk->obj = o; } }
    CP(const CP& o) : obj(o.obj), blk(o.blk) { if (blk) ++blk->refs; }
    ~CP() { if (blk && --blk->refs == 0) delete blk; }
};
static CP* g_registry = 0;
static void Register(const CP& p) { g_registry = new CP(p); }
static CP Add(void* prototypeCopy) { CP local(prototypeCopy); Register(local); return local; }
int main()
{
    int object = 0;
    {
        CP r = Add(&object);
        if (r.blk->refs != 2 || g_registry->blk != r.blk || r.obj != &object) { std::printf("00a98be0_TEST FAIL\n"); return 1; }
    }
    if (g_registry->blk->refs != 1) { std::printf("00a98be0_TEST FAIL\n"); return 1; }
    delete g_registry;
    std::printf("00a98be0_TEST PASS\n");
    return 0;
}
#include <cstdio>
static int g_destroyed = 0;
static void Destroy(void*) { ++g_destroyed; }
struct Block { long refs; void (*destroy)(void*); void* obj; };
struct CP { void* obj; Block* blk;
    CP() : obj(0), blk(0) {}
    CP(void* o) : obj(o), blk(0) { if (o) { blk = new Block; blk->refs = 1; blk->destroy = Destroy; blk->obj = o; } }
    CP(const CP& o) : obj(o.obj), blk(o.blk) { if (blk) ++blk->refs; }
    ~CP() { Block* b = blk; if (b) { if (--b->refs == 0) { b->destroy(b->obj); delete b; } } }
    CP& operator=(const CP& o) { if (blk != o.blk) { if (blk) { --blk->refs; if (blk->refs == 0) { blk->destroy(blk->obj); delete blk; } } obj = o.obj; blk = o.blk; if (blk) ++blk->refs; } return *this; }
    void SetNull() { if (blk) { --blk->refs; if (blk->refs == 0) { blk->destroy(blk->obj); delete blk; } } blk = 0; obj = 0; } };
struct Bank { CP dyn[2]; unsigned long nDyn; void* entries[4];
    void NotifyEntryRemoved(unsigned long i) { if (i < nDyn) dyn[i].SetNull(); entries[i] = 0; } };
int main()
{
    int o; Bank b; b.nDyn = 2; b.dyn[1] = CP(&o); for (int i = 0; i < 4; ++i) b.entries[i] = &o;
    b.NotifyEntryRemoved(1); b.NotifyEntryRemoved(3);
    if (g_destroyed != 1 || b.dyn[1].blk != 0 || b.entries[1] != 0 || b.entries[3] != 0 || b.entries[0] != &o) { std::printf("00a24330_TEST FAIL\n"); return 1; }
    std::printf("00a24330_TEST PASS\n");
    return 0;
}
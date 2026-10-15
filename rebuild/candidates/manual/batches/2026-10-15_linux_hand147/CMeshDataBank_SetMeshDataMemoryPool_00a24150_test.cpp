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
struct Pool { void* owner; };
struct Bank { CP pool; int ownerSlot; void Set(CP p, bool attach) { pool = p; if (attach) ((Pool*)pool.obj)->owner = &ownerSlot; } };
int main()
{
    Pool p1, p2; p1.owner = 0; p2.owner = 0; Bank b;
    { CP a1(&p1); b.Set(a1, true); }
    if (b.pool.obj != &p1 || b.pool.blk->refs != 1 || p1.owner != &b.ownerSlot || g_destroyed != 0) { std::printf("00a24150_TEST FAIL\n"); return 1; }
    { CP a2(&p2); b.Set(a2, false); }
    if (b.pool.obj != &p2 || g_destroyed != 1 || p2.owner != 0) { std::printf("00a24150_TEST FAIL\n"); return 1; }
    b.Set(b.pool, false); if (b.pool.blk->refs != 1) { std::printf("00a24150_TEST FAIL\n"); return 1; }
    std::printf("00a24150_TEST PASS\n");
    return 0;
}

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
static int g_deleted = 0;
struct Entry { virtual ~Entry() { ++g_deleted; } };
struct Arr { Entry** first; Entry** last; unsigned long size() const { return last - first; } };
static void CloseShaders(Arr& a) { for (unsigned long i = 0; i < a.size(); ++i) { Entry* e = a.first[i]; if (e) delete e; } a.last = a.first; }
int main()
{
    Entry* store[3]; store[0] = new Entry; store[1] = 0; store[2] = new Entry; Arr a; a.first = store; a.last = store + 3;
    CloseShaders(a);
    if (g_deleted != 2 || a.size() != 0) { std::printf("00a62050_TEST FAIL\n"); return 1; }
    std::printf("00a62050_TEST PASS\n");
    return 0;
}
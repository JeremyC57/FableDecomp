#include <cstdio>
// LoadAnimFile: the root chunk is shared by the file (2 refs while loading, 1 after the local dies);
// the returned file holds one reference.
struct Block { long refs; };
struct Ptr { void* obj; Block* b; Ptr(void* o, Block* bl) : obj(o), b(bl) {} Ptr(const Ptr& p) : obj(p.obj), b(p.b) { ++b->refs; } ~Ptr() { --b->refs; } };
static int loaded = 0;
struct File { Ptr root; File(const Ptr& r) : root(r) {} };
int main() {
    int rootChunk; Block rb = { 1 }, fb = { 1 };
    File* f = 0;
    { Ptr root(&rootChunk, &rb); f = new File(root); if (rb.refs != 2) { std::printf("00a99420_TEST FAIL\n"); return 1; } ++loaded; }
    if (rb.refs != 1 || loaded != 1 || f->root.obj != &rootChunk || fb.refs != 1) { std::printf("00a99420_TEST FAIL\n"); return 1; }
    delete f;
    std::printf("00a99420_TEST PASS\n"); return 0; }
// Test for CEngineLandscapeMemoryManager::FreeDisposalList (0x00BD9CE0): for each of the three disposal generations,
// drain the texture list, then the vertex-buffer list, then the index-buffer list front to back; each item is
// destroyed in place and its block handed to FreeVirtual (global allocator under the lock; the allocator is deleted
// when it empties and free-when-empty is set).
#include <cstdio>
static int g_log[64]; static int g_n = 0;
static int g_lock = 0, g_allocCount = 0, g_allocDeleted = 0; static bool g_freeWhenEmpty = true; static bool g_haveAlloc = true;
struct Item { int id; };
struct List {
    Item* items[8]; int head, tail;
    List() : head(0), tail(0) { }
    void push_back(Item* p) { items[tail++] = p; ++g_allocCount; }
    unsigned size() const { return (unsigned)(tail - head); }
    Item* front() const { return items[head]; }
    void pop_front() { ++head; }
};
struct Manager {
    List TextureDisposalList[3], VertexBufferDisposalList[3], IndexBufferDisposalList[3];
    void FreeVirtual(Item* block)
    {
        if (block) {
            ++g_lock;
            if (g_haveAlloc) {
                g_log[g_n++] = block->id; --g_allocCount;
                if (g_freeWhenEmpty && g_allocCount == 0) { g_haveAlloc = false; ++g_allocDeleted; }
            }
            --g_lock;
        }
    }
    void Delete(Item* p) { p->id += 0; FreeVirtual(p); }
    void FreeDisposalList()
    {
        for (int i = 0; i < 3; ++i) {
            while (TextureDisposalList[i].size() != 0) { Delete(TextureDisposalList[i].front()); TextureDisposalList[i].pop_front(); }
            while (VertexBufferDisposalList[i].size() != 0) { Delete(VertexBufferDisposalList[i].front()); VertexBufferDisposalList[i].pop_front(); }
            while (IndexBufferDisposalList[i].size() != 0) { Delete(IndexBufferDisposalList[i].front()); IndexBufferDisposalList[i].pop_front(); }
        }
    }
};
int main()
{
    static Item it[12]; Manager m;
    for (int i = 0; i < 12; ++i) it[i].id = 100 + i;
    m.TextureDisposalList[0].push_back(&it[0]); m.TextureDisposalList[0].push_back(&it[1]);
    m.IndexBufferDisposalList[0].push_back(&it[2]);
    m.VertexBufferDisposalList[1].push_back(&it[3]); m.TextureDisposalList[1].push_back(&it[4]);
    m.IndexBufferDisposalList[2].push_back(&it[5]); m.VertexBufferDisposalList[2].push_back(&it[6]); m.TextureDisposalList[2].push_back(&it[7]);
    m.FreeDisposalList();
    static const int expect[8] = { 100, 101, 102, 104, 103, 107, 106, 105 };
    if (g_n != 8 || g_lock != 0 || g_allocDeleted != 1) { std::printf("00bd9ce0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) if (g_log[i] != expect[i]) { std::printf("00bd9ce0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (m.TextureDisposalList[i].size() || m.VertexBufferDisposalList[i].size() || m.IndexBufferDisposalList[i].size()) { std::printf("00bd9ce0_TEST FAIL\n"); return 1; }
    m.FreeDisposalList();
    if (g_n != 8) { std::printf("00bd9ce0_TEST FAIL\n"); return 1; }
    std::printf("00bd9ce0_TEST PASS\n");
    return 0;
}
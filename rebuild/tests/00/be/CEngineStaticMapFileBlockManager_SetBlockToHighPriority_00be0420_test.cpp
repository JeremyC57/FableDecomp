// Behaviour test for CEngineStaticMapFileBlockManager::SetBlockToHighPriority (0x00BE0420): a low-priority block's load is raised to the high priority and the flag cleared; otherwise nothing happens.
#include <cstdio>
struct Async { int priority; int changes; void ChangeLoadingPriority(int p) { priority = p; ++changes; } };
struct Block { int RefCount; bool LowPriority; Async* LoadingData; };
struct Entry { int key; Block block; };
static int g_highPriority = 3;
struct Manager { Entry entries[4]; int count;
    Block& find(int key) { for (int i = 0; i < count; ++i) if (entries[i].key == key) return entries[i].block; return entries[0].block; }
    void SetBlockToHighPriority(int index) { Block& b = find(index); if (b.LowPriority) { b.LoadingData->ChangeLoadingPriority(g_highPriority); b.LowPriority = false; } } };
int main()
{
    Async a1 = { 0, 0 }, a2 = { 0, 0 }; Manager m; m.count = 2;
    m.entries[0].key = 10; m.entries[0].block.LowPriority = true; m.entries[0].block.LoadingData = &a1;
    m.entries[1].key = 20; m.entries[1].block.LowPriority = false; m.entries[1].block.LoadingData = &a2;
    m.SetBlockToHighPriority(10); m.SetBlockToHighPriority(20);
    if (a1.priority != 3 || a1.changes != 1 || m.entries[0].block.LowPriority || a2.changes != 0) { std::printf("00be0420_TEST FAIL\n"); return 1; }
    m.SetBlockToHighPriority(10); if (a1.changes != 1) { std::printf("00be0420_TEST FAIL\n"); return 1; }
    std::printf("00be0420_TEST PASS\n");
    return 0;
}
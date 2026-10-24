// behaviour test for 0x00BE04F0 CEngineStaticMapFileBlockManager::DeleteUnusedBlocks (sorted-list model of the block map)
#include <cstdio>

static int g_discarded = 0, g_released = 0;
struct AsyncData { bool finished; bool IsFinished() { return finished; } };
struct Counted { AsyncData* Data; long* Info;
    AsyncData* operator->() const { return Data; }
    void Release() { if (Info) { --*Info; if (*Info == 0) ++g_released; } Data = 0; Info = 0; } };
static void DiscardAsyncData(Counted& c) { ++g_discarded; c.Data->finished = true; }
struct Block { long key; long RefCount; Counted AsyncData; Block* next; };

static int DeleteUnusedBlocks(Block*& head)
{
    int erased = 0;
    for (Block** link = &head; *link; ) {
        Block* it = *link;
        if (it->RefCount == 0) {
            if (!it->AsyncData->IsFinished())
                DiscardAsyncData(it->AsyncData);
            it->AsyncData.Release();
            *link = it->next;               // erase(it)
            ++erased;
        } else
            link = &it->next;
    }
    return erased;
}

int main()
{
    AsyncData d[4] = { { true }, { false }, { false }, { true } };
    long info[4] = { 1, 2, 1, 1 };
    Block b[4];
    for (int i = 0; i < 4; ++i) { b[i].key = i; b[i].AsyncData.Data = &d[i]; b[i].AsyncData.Info = &info[i]; b[i].next = i < 3 ? &b[i + 1] : 0; }
    b[0].RefCount = 0; b[1].RefCount = 0; b[2].RefCount = 3; b[3].RefCount = 0;
    Block* head = &b[0];
    int n = DeleteUnusedBlocks(head);
    if (n != 3 || head != &b[2] || b[2].next != 0) { std::printf("00be04f0_TEST FAIL\n"); return 1; }
    if (g_discarded != 1 || g_released != 2 || info[1] != 1 || info[2] != 1) { std::printf("00be04f0_TEST FAIL\n"); return 1; }
    std::printf("00be04f0_TEST PASS\n");
    return 0;
}
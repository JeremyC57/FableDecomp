// Behaviour test for 0x00bf65b0: the tree node allocates its load task (null allocation -> false, no request) and queues the file block.
#include <cstdio>
struct Manager { long pos, size; bool prio; int calls; void AddRequest(long p, long s, bool h) { pos = p; size = s; prio = h; ++calls; } };
struct Task { void* owner; Manager* mgr; long filePos; unsigned char lod; bool isPatch; bool requested; };
static Task g_task; static bool g_allocFail;
static Task* Alloc() { return g_allocFail ? 0 : &g_task; }
struct LOD { long FileBlockPos, FileBlockSize; };
struct Node {
    LOD LODLoadInfo[8]; long FileBlockPos, FileBlockSize; Task* AsyncLoadTask;
    Task* NewTask(Manager* m, long pos, unsigned char lod, bool patch)
    {
        Task* t = Alloc();
        if (t) { t->owner = 0; t->mgr = m; t->filePos = pos; t->lod = lod; t->isPatch = patch; }
        return t;
    }
    bool StartTreeLoadTask(Manager* m, bool h)
    {
        AsyncLoadTask = NewTask(m, FileBlockPos, 0, false);
        if (AsyncLoadTask) { m->AddRequest(FileBlockPos, FileBlockSize, h); return AsyncLoadTask->requested = true; }
        return false;
    }
    bool StartPatchLoadTask(long lod, Manager* m, bool h)
    {
        AsyncLoadTask = NewTask(m, LODLoadInfo[lod].FileBlockPos, (unsigned char)lod, true);
        if (AsyncLoadTask) { m->AddRequest(LODLoadInfo[lod].FileBlockPos, LODLoadInfo[lod].FileBlockSize, h); AsyncLoadTask->requested = true; return true; }
        return false;
    }
};
int main()
{
    Node n; Manager m; m.calls = 0;
    for (int i = 0; i < 8; ++i) { n.LODLoadInfo[i].FileBlockPos = 100 * i; n.LODLoadInfo[i].FileBlockSize = 10 * i; }
    n.FileBlockPos = 7; n.FileBlockSize = 9;
    int ok = 1;
    g_allocFail = true;
    ok &= !n.StartTreeLoadTask(&m, true) && n.AsyncLoadTask == 0 && m.calls == 0;
    ok &= !n.StartPatchLoadTask(3, &m, true) && m.calls == 0;
    g_allocFail = false; g_task.requested = false;
    ok &= n.StartTreeLoadTask(&m, true) && n.AsyncLoadTask == &g_task && g_task.requested && g_task.filePos == 7 && !g_task.isPatch && m.pos == 7 && m.size == 9 && m.prio;
    g_task.requested = false;
    ok &= n.StartPatchLoadTask(3, &m, false) && g_task.requested && g_task.filePos == 300 && g_task.lod == 3 && g_task.isPatch && m.pos == 300 && m.size == 30 && !m.prio && m.calls == 2;
    if (!ok) { std::printf("00bf65b0_TEST FAIL\n"); return 1; }
    std::printf("00bf65b0_TEST PASS\n");
    return 0;
}
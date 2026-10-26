#include <cstdio>
static int g_patchDtors = 0, g_treeDtors = 0, g_releases = 0, g_discards = 0, g_requestReleased = -1, g_removeFinished = 0;
class CReleaseTarget { public: void ReleaseMe() { ++g_releases; } };
typedef void (CReleaseTarget::*CReleaseMethod)();
struct SCountBlock { long RefCount; CReleaseMethod Release; CReleaseTarget* Object; };
template <class T> class CCountedPointer { public:
    T* Object; SCountBlock* Block;
    void Reset()
    {
        if (Block) { --Block->RefCount; if (Block->RefCount == 0) { (Block->Object->*Block->Release)(); delete Block; } }
        Block = 0; Object = 0;
    }
};
struct CData { int x; };
static void DiscardAsyncData(CCountedPointer<CData>* d) { ++g_discards; d->Object = 0; }
class CTree { public: virtual ~CTree() { ++g_treeDtors; } };
class CPatch { public: virtual ~CPatch() { ++g_patchDtors; } };
class CManager { public: void ReleaseRequest(long r) { g_requestReleased = (int)r; } void RemoveFinished() { ++g_removeFinished; } };
class CMap { public:
    CTree* PatchTree; CPatch* Patches; CCountedPointer<CData> MapFileEntry; CManager* FileBlockManager; unsigned int Words[5];
    CCountedPointer<CData> LoadingPatches; bool IsDynamic; bool PatchTreeBaseLoading;
    void Clear()
    {
        delete[] Patches; Patches = 0; delete PatchTree; PatchTree = 0;
        MapFileEntry.Reset(); IsDynamic = false;
        if (LoadingPatches.Object) DiscardAsyncData(&LoadingPatches);
        if (PatchTreeBaseLoading) { FileBlockManager->ReleaseRequest(Words[3]); PatchTreeBaseLoading = false; }
        Words[2] = 0; Words[3] = 0; Words[4] = 0;
    }
    void OnMapVisibilityChanged(bool visible)
    {
        if (!visible) {
            if (LoadingPatches.Object) DiscardAsyncData(&LoadingPatches);
            if (!IsDynamic) {
                if (Patches) { delete[] Patches; Patches = 0; }
                if (PatchTreeBaseLoading) { FileBlockManager->ReleaseRequest(Words[3]); PatchTreeBaseLoading = false; }
                delete PatchTree; PatchTree = 0;
            }
            if (FileBlockManager) FileBlockManager->RemoveFinished();
        }
    }
};
static CData g_data; static CManager g_manager; static CReleaseTarget g_target;
static void Fill(CMap& m)
{
    m.PatchTree = new CTree; m.Patches = new CPatch[3];
    SCountBlock* b = new SCountBlock; b->RefCount = 1; b->Release = &CReleaseTarget::ReleaseMe; b->Object = &g_target;
    m.MapFileEntry.Object = &g_data; m.MapFileEntry.Block = b;
    m.FileBlockManager = &g_manager; for (int i = 0; i < 5; ++i) m.Words[i] = 10 + i;
    m.LoadingPatches.Object = &g_data; m.LoadingPatches.Block = 0; m.IsDynamic = true; m.PatchTreeBaseLoading = true;
}
int main()
{
    CMap m; Fill(m);
    m.OnMapVisibilityChanged(true);                  // visible: nothing
    if (g_discards != 0 || g_removeFinished != 0) { std::printf("00bdc3c0_TEST FAIL\n"); return 1; }
    m.OnMapVisibilityChanged(false);                 // dynamic map: only the async data and the sweep
    if (g_discards != 1 || g_removeFinished != 1 || m.Patches == 0 || m.PatchTree == 0) { std::printf("00bdc3c0_TEST FAIL\n"); return 1; }
    m.IsDynamic = false; m.LoadingPatches.Object = &g_data;
    m.OnMapVisibilityChanged(false);                 // static map: patches, request, tree
    if (g_discards != 2 || g_patchDtors != 3 || g_treeDtors != 1 || g_requestReleased != 13 || m.PatchTreeBaseLoading || m.Patches != 0 || m.PatchTree != 0 || g_removeFinished != 2) {
        std::printf("00bdc3c0_TEST FAIL\n"); return 1;
    }
    delete m.MapFileEntry.Block;
    std::printf("00bdc3c0_TEST PASS\n");
    return 0;
}
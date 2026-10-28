// Behaviour test for 0x00A27350 CMeshDataBank::~CMeshDataBank(): a model of the destructor's order -- CloseMeshes,
// AnimationManager->Uninitialise, then the members in reverse declaration order (counted pointers release their shared
// block and null both words, element arrays run every element's destructor before the buffer is freed, the lip-sync map
// is cleared, loaders are deleted), then the CBankFileAsync base.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char g_Log[64][24];
static int g_LogCount = 0;
static void Log(const char* s) { if (g_LogCount < 64) strcpy(g_Log[g_LogCount++], s); }

struct Info { long RefCount; int Deleted; };
struct CountedPointer {
    void* Data; Info* Info_;
    const char* Tag;
    ~CountedPointer()
    {
        if (Info_) {
            --Info_->RefCount;
            if (Info_->RefCount == 0) { Info_->Deleted = 1; Log(Tag); }
        }
        Data = 0; Info_ = 0;
    }
};
struct Element { int* Counter; ~Element() { ++*Counter; } };
struct ElementArray {
    Element* Start; Element* Finish; const char* Tag;
    ~ElementArray() { for (Element* p = Start; p != Finish; ++p) p->~Element(); if (Start) { free(Start); Log(Tag); } }
};
struct Loader { const char* Tag; ~Loader() { Log(Tag); } };
struct ScopedLoader { Loader* Ptr; ~ScopedLoader() { delete Ptr; } };
struct Named { const char* Tag; ~Named() { Log(Tag); } };
struct AnimationManager { void* MeshBank; void Uninitialise() { MeshBank = 0; Log("Uninitialise"); } };
struct LipSyncMap {
    int NodeCount; int Erased;
    ~LipSyncMap() { if (NodeCount != 0) { Erased = NodeCount; NodeCount = 0; Log("LipSyncClear"); } Log("LipSyncHeader"); }
};
struct BankFileAsync { ~BankFileAsync() { Log("BankFileAsync"); } };

struct MeshDataBank : BankFileAsync {
    ScopedLoader MeshLoader, AnimLoader;
    Named Filename, Banks[4];
    CountedPointer Pools[4];
    ElementArray MeshInfoBuffer;
    Named AnimationEnumType;
    AnimationManager* Manager;
    LipSyncMap* LipSync;
    CountedPointer PushBufferBuildInfo;
    Named StateBlock;
    void CloseMeshes() { Log("CloseMeshes"); }
    ~MeshDataBank() { CloseMeshes(); Manager->Uninitialise(); delete LipSync; }
};

static int fail(int line) { printf("00a27350_TEST FAIL (line %d)\n", line); return 1; }
static int Find(const char* s) { for (int i = 0; i < g_LogCount; ++i) if (!strcmp(g_Log[i], s)) return i; return -1; }

int main()
{
    Info shared = { 2, 0 }, sole = { 1, 0 };
    int elementDtors = 0;
    AnimationManager manager; manager.MeshBank = &manager;
    {
        MeshDataBank b;
        b.MeshLoader.Ptr = new Loader; b.MeshLoader.Ptr->Tag = "MeshLoader";
        b.AnimLoader.Ptr = new Loader; b.AnimLoader.Ptr->Tag = "AnimLoader";
        b.Filename.Tag = "Filename"; b.AnimationEnumType.Tag = "AnimationEnumType"; b.StateBlock.Tag = "StateBlock";
        for (int i = 0; i < 4; ++i) { b.Banks[i].Tag = "Bank"; b.Pools[i].Data = 0; b.Pools[i].Info_ = 0; b.Pools[i].Tag = "Pool"; }
        b.Pools[0].Info_ = &shared;
        b.PushBufferBuildInfo.Info_ = &sole; b.PushBufferBuildInfo.Tag = "PushBufferBuildInfo";
        b.MeshInfoBuffer.Start = (Element*)malloc(3 * sizeof(Element)); b.MeshInfoBuffer.Finish = b.MeshInfoBuffer.Start + 3;
        for (int i = 0; i < 3; ++i) b.MeshInfoBuffer.Start[i].Counter = &elementDtors;
        b.MeshInfoBuffer.Tag = "MeshInfoBuffer";
        b.Manager = &manager;
        b.LipSync = new LipSyncMap; b.LipSync->NodeCount = 2; b.LipSync->Erased = 0;
    }
    if (Find("CloseMeshes") != 0 || Find("Uninitialise") != 1) return fail(__LINE__);
    if (manager.MeshBank != 0) return fail(__LINE__);
    if (Find("LipSyncClear") < 0 || Find("LipSyncHeader") < Find("LipSyncClear")) return fail(__LINE__);
    if (Find("StateBlock") > Find("PushBufferBuildInfo")) return fail(__LINE__);
    if (sole.Deleted != 1 || sole.RefCount != 0) return fail(__LINE__);
    if (shared.Deleted != 0 || shared.RefCount != 1) return fail(__LINE__);
    if (elementDtors != 3 || Find("MeshInfoBuffer") < 0) return fail(__LINE__);
    if (Find("AnimLoader") > Find("MeshLoader")) return fail(__LINE__);
    if (Find("Filename") > Find("AnimLoader")) return fail(__LINE__);
    if (Find("BankFileAsync") != g_LogCount - 1) return fail(__LINE__);
    printf("00a27350_TEST PASS\n");
    return 0;
}
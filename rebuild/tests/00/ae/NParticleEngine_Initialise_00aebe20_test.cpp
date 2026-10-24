// behaviour test for 0x00AEBE20 NParticleEngine::Initialise(): create the emitter database, the emitter manager and the PSC manager into their global counted pointers
#include <cstdio>
#include <cstring>
static char gLog[64];
static void Log(const char* s) { std::strcat(gLog, s); }
struct CCountBlock { long RefCount; void (*DeleteData)(void* data); void* Data; };
template <class T> struct TDeleter { static void DeleteData(void* data) { delete (T*)data; } };
template <class T> struct CCountedPointer {
    T* Data; CCountBlock* Block;
    void ReleaseData()
    {
        if (Block) { --Block->RefCount; if (Block->RefCount == 0) { Block->DeleteData(Block->Data); delete Block; } }
        Block = 0; Data = 0;
    }
    void Reset(T* data)
    {
        ReleaseData();
        if (data) { Block = new CCountBlock; Block->RefCount = 1; Block->DeleteData = &TDeleter<T>::DeleteData; Block->Data = data; }
        Data = data;
    }
};
struct SDatabase { SDatabase() { Log("D"); } ~SDatabase() { Log("d"); } };
struct SManager { SManager() { Log("M"); } ~SManager() { Log("m"); } };
struct SPSCManager { SPSCManager() { Log("P"); } ~SPSCManager() { Log("p"); } };
static CCountedPointer<SDatabase> PEmitterDatabase;
static CCountedPointer<SManager> PEmitterManager;
static CCountedPointer<SPSCManager> PPSCManager;
static void Initialise()
{
    PEmitterDatabase.Reset(new SDatabase);
    PEmitterManager.Reset(new SManager);
    PPSCManager.Reset(new SPSCManager);
}
static void Uninitialise()
{
    PEmitterManager.ReleaseData();
    PEmitterDatabase.ReleaseData();
    PPSCManager.ReleaseData();
}
int main()
{

    gLog[0] = 0;
    Initialise();
    if (std::strcmp(gLog, "DMP") != 0) { std::printf("00aebe20_TEST FAIL\n"); return 1; }
    if (!PEmitterDatabase.Data || !PEmitterManager.Data || !PPSCManager.Data || PEmitterManager.Block->RefCount != 1) { std::printf("00aebe20_TEST FAIL\n"); return 1; }
    gLog[0] = 0;
    Initialise();                                        // again: each Reset frees the previous object after building the new one
    if (std::strcmp(gLog, "DdMmPp") != 0) { std::printf("00aebe20_TEST FAIL\n"); return 1; }
    Uninitialise();
    std::printf("00aebe20_TEST PASS\n");
    return 0;
}
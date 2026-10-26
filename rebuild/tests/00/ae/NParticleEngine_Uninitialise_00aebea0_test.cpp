// behaviour test for 0x00AEBEA0 NParticleEngine::Uninitialise(): release the emitter manager, the emitter database and the PSC manager (in that order) and clear the pointers
#include <cstdio>
#include <cstring>
static char gLog[64];
static void Log(const char* s) { std::strcat(gLog, s); }
struct CCountBlock { long RefCount; void (*DeleteData)(void* data); void* Data; };
template <class T> struct TDeleter { static void DeleteData(void* data) { delete (T*)data; } };
template <class T> class CCountedPointer { public:
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

    Initialise();
    gLog[0] = 0;
    Uninitialise();
    if (std::strcmp(gLog, "mdp") != 0) { std::printf("00aebea0_TEST FAIL\n"); return 1; }
    if (PEmitterDatabase.Data || PEmitterDatabase.Block || PEmitterManager.Data || PEmitterManager.Block || PPSCManager.Data || PPSCManager.Block) { std::printf("00aebea0_TEST FAIL\n"); return 1; }
    gLog[0] = 0;
    Uninitialise();                                      // already empty: nothing happens
    if (gLog[0] != 0) { std::printf("00aebea0_TEST FAIL\n"); return 1; }
    std::printf("00aebea0_TEST PASS\n");
    return 0;
}
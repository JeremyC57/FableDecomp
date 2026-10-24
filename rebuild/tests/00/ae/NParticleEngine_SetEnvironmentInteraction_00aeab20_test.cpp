// behaviour test for 0x00AEAB20 NParticleEngine::SetEnvironmentInteraction(CCountedPointer<CAEnvironmentInteraction>): store the by-value pointer into the global, releasing the previous interaction
#include <cstdio>
static int gDeleted = 0;
struct CCountBlock { long RefCount; void (*DeleteData)(void* data); void* Data; };
template <class T> struct TDeleter { static void DeleteData(void* data) { ++gDeleted; delete (T*)data; } };
template <class T> struct CCountedPointer {
    T* Data; CCountBlock* Block;
    CCountedPointer() : Data(0), Block(0) { }
    explicit CCountedPointer(T* p) : Data(p), Block(0)
    { if (p) { Block = new CCountBlock; Block->RefCount = 1; Block->DeleteData = &TDeleter<T>::DeleteData; Block->Data = p; } }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Block(o.Block) { if (Block) ++Block->RefCount; }
    void ReleaseData()
    {
        if (Block) { --Block->RefCount; if (Block->RefCount == 0) { Block->DeleteData(Block->Data); delete Block; } }
        Block = 0; Data = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Block != o.Block) { ReleaseData(); Data = o.Data; Block = o.Block; if (Block) ++Block->RefCount; }
        return *this;
    }
    ~CCountedPointer() { ReleaseData(); }
    long Refs() const { return Block ? Block->RefCount : 0; }
};
struct SInteraction { int Unused; };
static CCountedPointer<SInteraction> PEnvironmentInteraction;
static void SetEnvironmentInteraction(CCountedPointer<SInteraction> interaction) { PEnvironmentInteraction = interaction; }
int main()
{

    {
        CCountedPointer<SInteraction> a(new SInteraction);
        SetEnvironmentInteraction(a);
        if (PEnvironmentInteraction.Data != a.Data || a.Refs() != 2) { std::printf("00aeab20_TEST FAIL\n"); return 1; }   // the parameter's reference is gone again
        SetEnvironmentInteraction(a);                                        // same block: unchanged
        if (a.Refs() != 2 || gDeleted != 0) { std::printf("00aeab20_TEST FAIL\n"); return 1; }
        CCountedPointer<SInteraction> b(new SInteraction);
        SetEnvironmentInteraction(b);
        if (PEnvironmentInteraction.Data != b.Data || a.Refs() != 1 || b.Refs() != 2) { std::printf("00aeab20_TEST FAIL\n"); return 1; }
        a.ReleaseData();
        if (gDeleted != 1) { std::printf("00aeab20_TEST FAIL\n"); return 1; }                                             // the first interaction died with its last owner
        SetEnvironmentInteraction(CCountedPointer<SInteraction>());
        if (PEnvironmentInteraction.Data != 0 || PEnvironmentInteraction.Block != 0 || b.Refs() != 1) { std::printf("00aeab20_TEST FAIL\n"); return 1; }
    }
    if (gDeleted != 2) { std::printf("00aeab20_TEST FAIL\n"); return 1; }
    std::printf("00aeab20_TEST PASS\n");
    return 0;
}
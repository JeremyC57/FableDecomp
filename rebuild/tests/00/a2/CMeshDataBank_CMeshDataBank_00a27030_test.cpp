// Behaviour test for CMeshDataBank::CMeshDataBank (0x00A27030): the defaults the constructor leaves in a fresh bank.
#include <cstdio>
#include <vector>

static int g_deleted;
struct Loader { virtual ~Loader() { ++g_deleted; } };
struct Observable { void* observer; int count; Observable() : observer(0), count(0) {} void AddExclusiveObserver(void* o) { observer = o; ++count; } };
struct Bank { unsigned int UnloadDelay; Bank() : UnloadDelay(30) {} void SetUnloadDelay(unsigned int d) { UnloadDelay = d; } };

template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) {}
    ~scoped_ptr() { delete ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; if (old) delete old; }
};

struct CMeshDataBank {
    scoped_ptr<Loader> MeshLoader, AnimLoader;
    Bank MeshResourceBank, MeshStatsResourceBank, AnimationResourceBank, PhysicsMeshResourceBank;
    Observable* AnimationManager;
    void* TextureBank;
    bool DP3BumpEnable;
    std::vector<unsigned int> GroupBlockSizes;
    unsigned int MaxBonesPerVertex, MaxParticleMeshFacesToRepeat;
    float MaximumLODErrors[4];
    float MiminumLODReductionPercent;
    unsigned int MiminumLODReductionFaces;
    float MinimumAnimationMatch;
    int UnloadableMeshMemory, UnloadMeshFrameDelay;
    bool StaticMap;
    CMeshDataBank() : AnimationManager(0), TextureBank((void*)1), DP3BumpEnable(true), StaticMap(false)
    {
        MeshLoader.reset(new Loader);
        AnimLoader.reset(new Loader);
        MaximumLODErrors[0] = 0.0f; MaximumLODErrors[1] = 0.025f; MaximumLODErrors[2] = 0.04f; MaximumLODErrors[3] = 0.065f;
        MiminumLODReductionPercent = 12.5f;
        MiminumLODReductionFaces = 5;
        MinimumAnimationMatch = 0.2f;
        TextureBank = 0;
        GroupBlockSizes.push_back(18);
        MaxBonesPerVertex = 3;
        DP3BumpEnable = false;
        AnimationManager = new Observable;
        AnimationManager->AddExclusiveObserver(this);
        AnimationResourceBank.SetUnloadDelay(0);
        PhysicsMeshResourceBank.SetUnloadDelay(0);
        MeshStatsResourceBank.SetUnloadDelay(0);
        UnloadableMeshMemory = 0;
        UnloadMeshFrameDelay = 5;
        MaxParticleMeshFacesToRepeat = 64;
        StaticMap = true;
    }
    ~CMeshDataBank() { delete AnimationManager; }
};

int main()
{
    bool ok;
    {
        CMeshDataBank b;
        ok = b.MeshLoader.ptr != 0 && b.AnimLoader.ptr != 0 && g_deleted == 0;
        ok = ok && b.GroupBlockSizes.size() == 1 && b.GroupBlockSizes[0] == 18;
        ok = ok && b.MaxBonesPerVertex == 3 && !b.DP3BumpEnable && b.TextureBank == 0;
        ok = ok && b.MaximumLODErrors[1] == 0.025f && b.MaximumLODErrors[3] == 0.065f && b.MiminumLODReductionPercent == 12.5f;
        ok = ok && b.MiminumLODReductionFaces == 5 && b.MinimumAnimationMatch == 0.2f;
        ok = ok && b.AnimationManager->observer == (void*)&b && b.AnimationManager->count == 1;
        ok = ok && b.AnimationResourceBank.UnloadDelay == 0 && b.PhysicsMeshResourceBank.UnloadDelay == 0;
        ok = ok && b.MeshStatsResourceBank.UnloadDelay == 0 && b.MeshResourceBank.UnloadDelay == 30;
        ok = ok && b.UnloadableMeshMemory == 0 && b.UnloadMeshFrameDelay == 5 && b.MaxParticleMeshFacesToRepeat == 64 && b.StaticMap;
        b.MeshLoader.reset(new Loader);
        ok = ok && g_deleted == 1;
    }
    ok = ok && g_deleted == 3;
    std::printf(ok ? "00a27030_TEST PASS\n" : "00a27030_TEST FAIL\n");
    return ok ? 0 : 1;
}
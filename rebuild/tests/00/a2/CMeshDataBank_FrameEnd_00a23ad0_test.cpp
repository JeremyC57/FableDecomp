// Behaviour test for 0x00A23AD0 CMeshDataBank::FrameEnd: the mesh, animation and physics-mesh banks end their frame
// (CResourceBank::FrameEnd: both lists' CurrentFrame advance, UnloadedThisFrame cleared); the mesh-stats bank is untouched.
#include <cstdio>
class CResourceList {
public:
    CResourceList() : CurrentFrame(0), UnloadedThisFrame(7) { }
    unsigned long CurrentFrame;
    unsigned long UnloadedThisFrame;
};
class CResourceBank {
public:
    void FrameEnd()
    {
        ++ResourceList.CurrentFrame; ResourceList.UnloadedThisFrame = 0;
        ++LoadingQueue.CurrentFrame; LoadingQueue.UnloadedThisFrame = 0;
    }
    CResourceList ResourceList;
    CResourceList LoadingQueue;
};
class CMeshDataBank {
public:
    CResourceBank MeshResourceBank;
    CResourceBank MeshStatsResourceBank;
    CResourceBank AnimationResourceBank;
    CResourceBank PhysicsMeshResourceBank;
    void FrameEnd()
    {
        MeshResourceBank.FrameEnd();
        AnimationResourceBank.FrameEnd();
        PhysicsMeshResourceBank.FrameEnd();
    }
};
int main()
{
    CMeshDataBank bank;
    bank.FrameEnd();
    bank.FrameEnd();
    if (bank.MeshResourceBank.ResourceList.CurrentFrame != 2 || bank.AnimationResourceBank.LoadingQueue.CurrentFrame != 2 ||
        bank.PhysicsMeshResourceBank.ResourceList.UnloadedThisFrame != 0 ||
        bank.MeshStatsResourceBank.ResourceList.CurrentFrame != 0 || bank.MeshStatsResourceBank.LoadingQueue.UnloadedThisFrame != 7) {
        std::printf("00a23ad0_TEST FAIL\n"); return 1;
    }
    std::printf("00a23ad0_TEST PASS\n");
    return 0;
}
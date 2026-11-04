// Behaviour test for CEngineResourceManager::OnReleaseDefaultPoolResources 0x00B3AFE0: the virtual (CEngineComponent slot 11)
// begins the GPU scratch-buffer texture manager's shutdown and returns nothing (BeginShutdown's bool is dropped).
#include <cstdio>
struct CGPUScratchBufferTextureManager { int Shutdowns; CGPUScratchBufferTextureManager() : Shutdowns(0) {} bool BeginShutdown() { ++Shutdowns; return true; } };
struct CEngineComponent { virtual ~CEngineComponent() {} virtual void OnReleaseDefaultPoolResources() {} };
struct CEngineResourceManager : CEngineComponent {
    CGPUScratchBufferTextureManager GPUScratchBufferTextureManager;
    CGPUScratchBufferTextureManager& GetGPUScratchBufferTextureManager() { return GPUScratchBufferTextureManager; }
private:
    void OnReleaseDefaultPoolResources() { GetGPUScratchBufferTextureManager().BeginShutdown(); }
};
int main()
{
    CEngineResourceManager m; CEngineComponent& c = m;
    c.OnReleaseDefaultPoolResources();
    if (m.GPUScratchBufferTextureManager.Shutdowns != 1) { std::printf("00b3afe0_TEST FAIL\n"); return 1; }
    std::printf("00b3afe0_TEST PASS\n");
    return 0;
}
// Behaviour test for CEngineSurfaceCompositionManager::ChangeSceneForTextureBank 0x00B2AD80: only the completed bank (the second of
// the three CGraphicDataBank members) changes scene.
#include <cstdio>
struct CGraphicDataBank { int SceneChanges; CGraphicDataBank() : SceneChanges(0) {} void ChangeScene() { ++SceneChanges; } };
struct CEngineSurfaceCompositionManager {
    CGraphicDataBank IntermediateBankFile, CompletedBankFile, CustomTextureBankFile;
    void ChangeSceneForTextureBank() { CompletedBankFile.ChangeScene(); }
};
int main()
{
    CEngineSurfaceCompositionManager m;
    m.ChangeSceneForTextureBank(); m.ChangeSceneForTextureBank();
    if (m.CompletedBankFile.SceneChanges != 2 || m.IntermediateBankFile.SceneChanges || m.CustomTextureBankFile.SceneChanges) { std::printf("00b2ad80_TEST FAIL\n"); return 1; }
    std::printf("00b2ad80_TEST PASS\n");
    return 0;
}
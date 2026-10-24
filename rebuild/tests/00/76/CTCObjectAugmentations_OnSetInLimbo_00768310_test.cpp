// Behaviour test for 00768310 (CTCObjectAugmentations::OnSetInLimbo): either limbo state re-attaches the augmentation particles, then updates the particle morphs.
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CTCObjectAugmentations {
public:
    void AttachAugmentationParticles() { g_order[g_count++] = 1; }   // stands in for 0x007677E0
    void UpdateParticleMorphs() { g_order[g_count++] = 2; }          // stands in for 0x00766C00
    void OnSetInLimbo(bool inLimbo);
};
void CTCObjectAugmentations::OnSetInLimbo(bool inLimbo)
{
    (void)inLimbo;
    AttachAugmentationParticles();
    UpdateParticleMorphs();
}
int main()
{
    CTCObjectAugmentations augmentations;
    augmentations.OnSetInLimbo(true);
    augmentations.OnSetInLimbo(false);
    if (g_count != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 1 || g_order[3] != 2) { std::printf("00768310_TEST FAIL\n"); return 1; }
    std::printf("00768310_TEST PASS\n");
    return 0;
}
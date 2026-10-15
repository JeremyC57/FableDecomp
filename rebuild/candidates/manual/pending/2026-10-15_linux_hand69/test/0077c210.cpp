// Behaviour test for CEngineMeshEffectDistanceFade::CEngineMeshEffectDistanceFade(float, float, float, float) @ 0x0077C210
#include <cstdio>

struct CEngineMeshEffectRoot {
    virtual ~CEngineMeshEffectRoot() { }
    long RefCount;
    CEngineMeshEffectRoot() : RefCount(1) { }
};

struct CEngineMeshEffect : CEngineMeshEffectRoot {
    long MeshEffect;
    CEngineMeshEffect(long effect) : MeshEffect(effect) { }
};

struct CEngineMeshEffectDistanceFade : CEngineMeshEffect {
    float NearClipDistance, NearFadeDistance, FarFadeDistance, FarClipDistance;
    CEngineMeshEffectDistanceFade(float nearClip, float nearFade, float farFade, float farClip)
        : CEngineMeshEffect(2), NearClipDistance(nearClip), NearFadeDistance(nearFade), FarFadeDistance(farFade), FarClipDistance(farClip) { }
};

int main()
{
    CEngineMeshEffectDistanceFade e(1.0f, 2.5f, 100.0f, 120.0f);
    if (e.RefCount != 1 || e.MeshEffect != 2) { std::printf("0077c210_TEST FAIL\n"); return 1; }
    if (e.NearClipDistance != 1.0f || e.NearFadeDistance != 2.5f || e.FarFadeDistance != 100.0f || e.FarClipDistance != 120.0f) { std::printf("0077c210_TEST FAIL\n"); return 1; }
    std::printf("0077c210_TEST PASS\n");
    return 0;
}

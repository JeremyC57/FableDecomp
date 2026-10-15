// CEngineMeshEffectDistanceFade::CEngineMeshEffectDistanceFade(float, float, float, float) -- a distance-fade mesh effect (effect type 2) with its near-clip, near-fade, far-fade and far-clip distances
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C210, length 53 bytes. No callees.
// The manifest label "CQuestInfoElementCounterList::CQuestInfoElementCounterList" is propagated and rejected: the
// vtable stored is 0x0126ADE0, CEngineMeshEffectDistanceFade's (rtti-vtables.tsv). PDB: CEngineMeshEffect is 12
// bytes (a counted root's word at +4, MeshEffect +8), CEngineMeshEffectDistanceFade adds NearClipDistance +0xc,
// NearFadeDistance +0x10, FarFadeDistance +0x14, FarClipDistance +0x18 (28 bytes).
// Shape: the root and base constructors are inline (their vtable stores are dead): +4 = 1 (the reference count)
// and +8 = 2 (the effect type) sit before the derived vtable store; the four float arguments are copied with
// integer moves, interleaved with those stores by the scheduler; `ret 10h`.
struct CEngineMeshEffectRoot {
    virtual ~CEngineMeshEffectRoot();
    long RefCount;                          // +0x04
    CEngineMeshEffectRoot() : RefCount(1) { }
};

struct CEngineMeshEffect : CEngineMeshEffectRoot {
    long MeshEffect;                        // +0x08 EEngineMeshEffect
    CEngineMeshEffect(long effect) : MeshEffect(effect) { }
};

struct CEngineMeshEffectDistanceFade : CEngineMeshEffect {
    float NearClipDistance;                 // +0x0c
    float NearFadeDistance;                 // +0x10
    float FarFadeDistance;                  // +0x14
    float FarClipDistance;                  // +0x18
    CEngineMeshEffectDistanceFade(float nearClip, float nearFade, float farFade, float farClip);
};

CEngineMeshEffectDistanceFade::CEngineMeshEffectDistanceFade(float nearClip, float nearFade, float farFade, float farClip)
    : CEngineMeshEffect(2)
    , NearClipDistance(nearClip)
    , NearFadeDistance(nearFade)
    , FarFadeDistance(farFade)
    , FarClipDistance(farClip)
{
}

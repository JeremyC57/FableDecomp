// Behaviour test for 0x0081DFE0: vector<CParticleMorphs::CEntry>::resize(n) forwards a cleared entry (LocationName -1)
#include <cstdio>
#include <string.h>
enum EHeroMorphType { MORPH_TYPE_NONE, MORPH_TYPE_OTHER = 5 };
enum EMeshLocationPoint { MESH_LOCATION_NONE, MESH_LOCATION_OTHER = 3 };
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};
class CParticleMorphs {
public:
    class CEntry {
    public:
        EHeroMorphType MorphType;
        bool Blending;
        EMeshLocationPoint Location;
        CDefString LocationName;
        long LocationIndex;
        long ParticleIndex;
        long SecondParticleIndex;
        float StartAtMorphStrength;
        float EndAtMorphStrength;
        CEntry() { memset(this, 0, sizeof(*this)); LocationName = CDefString(); }
    };
};

static bool FloatNear(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

template <class T> class TestVector {               // records what the out-of-line resize(n, x) receives
public:
    unsigned int lastN;
    int calls;
    T lastX;
    TestVector() : lastN(0), calls(0) { }
    void resize(unsigned int n, const T& x) { lastN = n; lastX = x; ++calls; }
    void resize(unsigned int n) { resize(n, T()); }  // the reconstructed forwarder
};

int main()
{
    TestVector<CParticleMorphs::CEntry > v;
    CParticleMorphs::CEntry& x = v.lastX;
    x.MorphType = MORPH_TYPE_OTHER; x.Blending = true; x.Location = MESH_LOCATION_OTHER; x.LocationName.Index = 4; x.LocationIndex = 5; x.ParticleIndex = 6; x.SecondParticleIndex = 7; x.StartAtMorphStrength = 0.5f; x.EndAtMorphStrength = 0.75f;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.MorphType == MORPH_TYPE_NONE && !x.Blending && x.Location == MESH_LOCATION_NONE && x.LocationName.Index == -1 && x.LocationIndex == 0 && x.ParticleIndex == 0 && x.SecondParticleIndex == 0 && FloatNear(x.StartAtMorphStrength, 0.0f) && FloatNear(x.EndAtMorphStrength, 0.0f))) {
        std::printf("0081dfe0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.MorphType == MORPH_TYPE_NONE && !x.Blending && x.Location == MESH_LOCATION_NONE && x.LocationName.Index == -1 && x.LocationIndex == 0 && x.ParticleIndex == 0 && x.SecondParticleIndex == 0 && FloatNear(x.StartAtMorphStrength, 0.0f) && FloatNear(x.EndAtMorphStrength, 0.0f))) {
        std::printf("0081dfe0_TEST FAIL\n");
        return 1;
    }
    std::printf("0081dfe0_TEST PASS\n");
    return 0;
}
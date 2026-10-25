// Behaviour test for 0x0081DED0: vector<pair<EHeroMorphType, CParticleMorphs::CEntry> >::resize(n) forwards a cleared pair (LocationName -1)
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
template <class _T1, class _T2>
struct TestPair {                                   // STLport pair: pair() : first(_T1()), second(_T2())
    _T1 first;
    _T2 second;
    TestPair() : first(_T1()), second(_T2()) { }
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
    TestVector<TestPair<EHeroMorphType, CParticleMorphs::CEntry> > v;
    TestPair<EHeroMorphType, CParticleMorphs::CEntry>& x = v.lastX;
    x.first = MORPH_TYPE_OTHER; x.second.MorphType = MORPH_TYPE_OTHER; x.second.Blending = true; x.second.Location = MESH_LOCATION_OTHER; x.second.LocationName.Index = 4; x.second.LocationIndex = 5; x.second.ParticleIndex = 6; x.second.SecondParticleIndex = 7; x.second.StartAtMorphStrength = 0.5f; x.second.EndAtMorphStrength = 0.75f;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.first == MORPH_TYPE_NONE && x.second.MorphType == MORPH_TYPE_NONE && !x.second.Blending && x.second.Location == MESH_LOCATION_NONE && x.second.LocationName.Index == -1 && x.second.LocationIndex == 0 && x.second.ParticleIndex == 0 && x.second.SecondParticleIndex == 0 && FloatNear(x.second.StartAtMorphStrength, 0.0f) && FloatNear(x.second.EndAtMorphStrength, 0.0f))) {
        std::printf("0081ded0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.first == MORPH_TYPE_NONE && x.second.MorphType == MORPH_TYPE_NONE && !x.second.Blending && x.second.Location == MESH_LOCATION_NONE && x.second.LocationName.Index == -1 && x.second.LocationIndex == 0 && x.second.ParticleIndex == 0 && x.second.SecondParticleIndex == 0 && FloatNear(x.second.StartAtMorphStrength, 0.0f) && FloatNear(x.second.EndAtMorphStrength, 0.0f))) {
        std::printf("0081ded0_TEST FAIL\n");
        return 1;
    }
    std::printf("0081ded0_TEST PASS\n");
    return 0;
}
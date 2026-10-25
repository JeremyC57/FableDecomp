// Behaviour test for 0x00759770: vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::resize(n) forwards a zeroed pair
#include <cstdio>
enum EBodyLocation { BODY_LOCATION_NONE, BODY_LOCATION_OTHER = 4 };
class CWoundMorphs {
public:
    class CEntry {
    public:
        EBodyLocation Location;
        long ReplacingTextureIndex, ReplacingBumpMapIndex, WoundTextureIndex, WoundBumpMapIndex, ScarTextureIndex, ScarBumpMapIndex;
        long FadeToScarDuration;
        long TotalDuration;
        float PermanentScarStrength;
        CEntry() : Location(BODY_LOCATION_NONE), ReplacingTextureIndex(0), ReplacingBumpMapIndex(0), WoundTextureIndex(0),
                   WoundBumpMapIndex(0), ScarTextureIndex(0), ScarBumpMapIndex(0), FadeToScarDuration(0), TotalDuration(0),
                   PermanentScarStrength(0.0f) { }
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
    TestVector<TestPair<EBodyLocation, CWoundMorphs::CEntry> > v;
    TestPair<EBodyLocation, CWoundMorphs::CEntry>& x = v.lastX;
    x.first = BODY_LOCATION_OTHER; x.second.Location = BODY_LOCATION_OTHER; x.second.ReplacingTextureIndex = 1; x.second.ScarBumpMapIndex = 2; x.second.TotalDuration = 3; x.second.PermanentScarStrength = 0.5f;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.first == BODY_LOCATION_NONE && x.second.Location == BODY_LOCATION_NONE && x.second.ReplacingTextureIndex == 0 && x.second.ReplacingBumpMapIndex == 0 && x.second.WoundTextureIndex == 0 && x.second.WoundBumpMapIndex == 0 && x.second.ScarTextureIndex == 0 && x.second.ScarBumpMapIndex == 0 && x.second.FadeToScarDuration == 0 && x.second.TotalDuration == 0 && FloatNear(x.second.PermanentScarStrength, 0.0f))) {
        std::printf("00759770_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.first == BODY_LOCATION_NONE && x.second.Location == BODY_LOCATION_NONE && x.second.ReplacingTextureIndex == 0 && x.second.ReplacingBumpMapIndex == 0 && x.second.WoundTextureIndex == 0 && x.second.WoundBumpMapIndex == 0 && x.second.ScarTextureIndex == 0 && x.second.ScarBumpMapIndex == 0 && x.second.FadeToScarDuration == 0 && x.second.TotalDuration == 0 && FloatNear(x.second.PermanentScarStrength, 0.0f))) {
        std::printf("00759770_TEST FAIL\n");
        return 1;
    }
    std::printf("00759770_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x0081DE90: vector<pair<EHeroMorphType, CTextureMorphEntry> >::resize(n) forwards a zeroed pair
#include <cstdio>
enum EHeroMorphType { MORPH_TYPE_NONE, MORPH_TYPE_OTHER = 5 };
class CTextureMorphEntry {                          // POD
public:
    EHeroMorphType MorphType;
    long TextureLayer;
    long ReplacingTextureIndex;
    long BankIndex;
    long Blend;
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
    TestVector<TestPair<EHeroMorphType, CTextureMorphEntry> > v;
    TestPair<EHeroMorphType, CTextureMorphEntry>& x = v.lastX;
    x.first = MORPH_TYPE_OTHER; x.second.MorphType = MORPH_TYPE_OTHER; x.second.TextureLayer = 1; x.second.ReplacingTextureIndex = 2; x.second.BankIndex = 3; x.second.Blend = 4;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.first == MORPH_TYPE_NONE && x.second.MorphType == MORPH_TYPE_NONE && x.second.TextureLayer == 0 && x.second.ReplacingTextureIndex == 0 && x.second.BankIndex == 0 && x.second.Blend == 0)) {
        std::printf("0081de90_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.first == MORPH_TYPE_NONE && x.second.MorphType == MORPH_TYPE_NONE && x.second.TextureLayer == 0 && x.second.ReplacingTextureIndex == 0 && x.second.BankIndex == 0 && x.second.Blend == 0)) {
        std::printf("0081de90_TEST FAIL\n");
        return 1;
    }
    std::printf("0081de90_TEST PASS\n");
    return 0;
}
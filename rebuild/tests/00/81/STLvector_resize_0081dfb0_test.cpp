// Behaviour test for 0x0081DFB0: vector<CTextureMorphEntry>::resize(n) forwards a zeroed POD entry
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
    TestVector<CTextureMorphEntry > v;
    CTextureMorphEntry& x = v.lastX;
    x.MorphType = MORPH_TYPE_OTHER; x.TextureLayer = 1; x.ReplacingTextureIndex = 2; x.BankIndex = 3; x.Blend = 4;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.MorphType == MORPH_TYPE_NONE && x.TextureLayer == 0 && x.ReplacingTextureIndex == 0 && x.BankIndex == 0 && x.Blend == 0)) {
        std::printf("0081dfb0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.MorphType == MORPH_TYPE_NONE && x.TextureLayer == 0 && x.ReplacingTextureIndex == 0 && x.BankIndex == 0 && x.Blend == 0)) {
        std::printf("0081dfb0_TEST FAIL\n");
        return 1;
    }
    std::printf("0081dfb0_TEST PASS\n");
    return 0;
}
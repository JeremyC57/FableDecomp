// Behaviour test for 0x004EA398: vector<CEngineGraphic>::resize(n) forwards CEngineGraphic()
#include <cstdio>
#pragma pack(push, 2)
class CEngineGraphic {                              // 14 bytes in retail (2-packed)
public:
    unsigned int BankIndex;
    float AnimStep;
    float RenderSizeX;
    bool AdditiveAlpha;
    unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
};
#pragma pack(pop)

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
    TestVector<CEngineGraphic > v;
    CEngineGraphic& x = v.lastX;
    x.BankIndex = 9; x.AnimStep = 1.5f; x.RenderSizeX = 2.5f; x.AdditiveAlpha = true; x.Type = 3;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.BankIndex == 0 && FloatNear(x.AnimStep, 0.0f) && FloatNear(x.RenderSizeX, 0.0f) && !x.AdditiveAlpha && x.Type == 0)) {
        std::printf("004ea398_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.BankIndex == 0 && FloatNear(x.AnimStep, 0.0f) && FloatNear(x.RenderSizeX, 0.0f) && !x.AdditiveAlpha && x.Type == 0)) {
        std::printf("004ea398_TEST FAIL\n");
        return 1;
    }
    std::printf("004ea398_TEST PASS\n");
    return 0;
}
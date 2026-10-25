// Behaviour test for 0x0067F3A0: vector<CDegradableInfo>::resize(n) forwards CDegradableInfo() (only the engine graphic is initialised)
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
class CDegradableInfo {
public:
    float HealthPercentage;
    CEngineGraphic EngineGraphic;
    long SmashParticleEmitter;
    bool BlocksNavigation;
    CDegradableInfo() { }
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
    TestVector<CDegradableInfo > v;
    CDegradableInfo& x = v.lastX;
    x.EngineGraphic.BankIndex = 9; x.EngineGraphic.AnimStep = 1.5f; x.EngineGraphic.RenderSizeX = 2.5f; x.EngineGraphic.AdditiveAlpha = true; x.EngineGraphic.Type = 3;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.EngineGraphic.BankIndex == 0 && FloatNear(x.EngineGraphic.AnimStep, 0.0f) && FloatNear(x.EngineGraphic.RenderSizeX, 0.0f) && !x.EngineGraphic.AdditiveAlpha && x.EngineGraphic.Type == 0)) {
        std::printf("0067f3a0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.EngineGraphic.BankIndex == 0 && FloatNear(x.EngineGraphic.AnimStep, 0.0f) && FloatNear(x.EngineGraphic.RenderSizeX, 0.0f) && !x.EngineGraphic.AdditiveAlpha && x.EngineGraphic.Type == 0)) {
        std::printf("0067f3a0_TEST FAIL\n");
        return 1;
    }
    std::printf("0067f3a0_TEST PASS\n");
    return 0;
}
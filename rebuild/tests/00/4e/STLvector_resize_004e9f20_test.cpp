// Behaviour test for 0x004E9F20: vector<CMusicEntry>::resize(n) forwards CMusicEntry() (second TU copy)
#include <cstdio>
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};
class CMusicEntry {
public:
    unsigned int ID;
    CDefString Filename;
    float Gain;
    bool CacheToHD;
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
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
    TestVector<CMusicEntry > v;
    CMusicEntry& x = v.lastX;
    x.ID = 5; x.Filename.Index = 2; x.Gain = 3.0f; x.CacheToHD = true;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.ID == 0 && x.Filename.Index == -1 && FloatNear(x.Gain, 0.0f) && !x.CacheToHD)) {
        std::printf("004e9f20_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.ID == 0 && x.Filename.Index == -1 && FloatNear(x.Gain, 0.0f) && !x.CacheToHD)) {
        std::printf("004e9f20_TEST FAIL\n");
        return 1;
    }
    std::printf("004e9f20_TEST PASS\n");
    return 0;
}
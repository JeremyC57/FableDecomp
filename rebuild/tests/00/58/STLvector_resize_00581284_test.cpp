// Behaviour test for 0x00581284: vector<CHeroMarriageInfo>::resize(n) forwards CHeroMarriageInfo()
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 TestU64;
#else
typedef unsigned long long TestU64;
#endif
class CHeroMarriageInfo {
public:
    TestU64 WifeUID;
    TestU64 MaritalHomeUID;
    long State;
    bool GayMarriage;
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State(0), GayMarriage(false) { }
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
    TestVector<CHeroMarriageInfo > v;
    CHeroMarriageInfo& x = v.lastX;
    x.WifeUID = 11; x.MaritalHomeUID = 12; x.State = 2; x.GayMarriage = true;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.WifeUID == 0 && x.MaritalHomeUID == 0 && x.State == 0 && !x.GayMarriage)) {
        std::printf("00581284_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.WifeUID == 0 && x.MaritalHomeUID == 0 && x.State == 0 && !x.GayMarriage)) {
        std::printf("00581284_TEST FAIL\n");
        return 1;
    }
    std::printf("00581284_TEST PASS\n");
    return 0;
}
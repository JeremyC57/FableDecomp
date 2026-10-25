// Behaviour test for 0x00657069: vector<C2DCoordI>::resize(n) forwards an (uninitialised) C2DCoordI()
#include <cstdio>
class C2DCoordI {
public:
    long X;
    long Y;
    C2DCoordI() { }
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
    TestVector<C2DCoordI > v;
    C2DCoordI& x = v.lastX;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(true)) {
        std::printf("00657069_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(true)) {
        std::printf("00657069_TEST FAIL\n");
        return 1;
    }
    std::printf("00657069_TEST PASS\n");
    return 0;
}
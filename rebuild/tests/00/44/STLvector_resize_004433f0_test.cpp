// Behaviour test for 0x004433F0: vector<long>::resize(n) forwards a zero long
#include <cstdio>

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
    TestVector<long > v;
    long& x = v.lastX;
    x = 77;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x == 0)) {
        std::printf("004433f0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x == 0)) {
        std::printf("004433f0_TEST FAIL\n");
        return 1;
    }
    std::printf("004433f0_TEST PASS\n");
    return 0;
}
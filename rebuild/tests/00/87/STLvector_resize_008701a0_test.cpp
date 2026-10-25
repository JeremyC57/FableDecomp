// Behaviour test for 0x008701A0: vector<pair<EExpressionType, CExpressionSet::CEntry> >::resize(n) forwards a zeroed pair
#include <cstdio>
enum EExpressionType { EXPRESSION_NONE, EXPRESSION_OTHER = 2 };
class CExpressionSet {
public:
    class CEntry {                                  // POD
    public:
        EExpressionType Type;
        long AnimIndex;
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
    TestVector<TestPair<EExpressionType, CExpressionSet::CEntry> > v;
    TestPair<EExpressionType, CExpressionSet::CEntry>& x = v.lastX;
    x.first = EXPRESSION_OTHER; x.second.Type = EXPRESSION_OTHER; x.second.AnimIndex = 9;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.first == EXPRESSION_NONE && x.second.Type == EXPRESSION_NONE && x.second.AnimIndex == 0)) {
        std::printf("008701a0_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.first == EXPRESSION_NONE && x.second.Type == EXPRESSION_NONE && x.second.AnimIndex == 0)) {
        std::printf("008701a0_TEST FAIL\n");
        return 1;
    }
    std::printf("008701a0_TEST PASS\n");
    return 0;
}
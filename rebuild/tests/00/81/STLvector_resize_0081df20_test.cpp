// Behaviour test for 0x0081DF20: vector<pair<unsigned long, CSkeletalMorphs::CEntry> >::resize(n) forwards key 0 and an empty morph name
#include <cstdio>
class CDefString {
public:
    long Index;
    CDefString() : Index(-1) { }
};
class CSkeletalMorphs {
public:
    class CEntry {
    public:
        CDefString MorphName;
        short Priority;
        bool SupressesLowerMorphs;
        CEntry() { }
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
    TestVector<TestPair<unsigned int, CSkeletalMorphs::CEntry> > v;
    TestPair<unsigned int, CSkeletalMorphs::CEntry>& x = v.lastX;
    x.first = 8; x.second.MorphName.Index = 6;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.first == 0 && x.second.MorphName.Index == -1)) {
        std::printf("0081df20_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.first == 0 && x.second.MorphName.Index == -1)) {
        std::printf("0081df20_TEST FAIL\n");
        return 1;
    }
    std::printf("0081df20_TEST PASS\n");
    return 0;
}
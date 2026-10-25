// Behaviour test for 0x009D4300: vector<CBankFile::CRuntimeData>::resize(n) forwards a zeroed runtime record
#include <cstdio>
class CBankFile {
public:
    class CRuntimeData {
    public:
        unsigned int DataOffset;
        unsigned int DataSize;
        unsigned char DataType;
        bool Valid;
        CRuntimeData() : DataOffset(0), DataSize(0), DataType(0), Valid(false) { }
    };
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
    TestVector<CBankFile::CRuntimeData > v;
    CBankFile::CRuntimeData& x = v.lastX;
    x.DataOffset = 100; x.DataSize = 200; x.DataType = 3; x.Valid = true;
    v.resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.DataOffset == 0 && x.DataSize == 0 && x.DataType == 0 && !x.Valid)) {
        std::printf("009d4300_TEST FAIL\n");
        return 1;
    }
    v.resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.DataOffset == 0 && x.DataSize == 0 && x.DataType == 0 && !x.Valid)) {
        std::printf("009d4300_TEST FAIL\n");
        return 1;
    }
    std::printf("009d4300_TEST PASS\n");
    return 0;
}
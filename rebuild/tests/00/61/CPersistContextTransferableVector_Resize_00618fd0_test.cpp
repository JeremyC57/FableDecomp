// Behaviour test for 0x00618FD0: CPersistContext_TransferableVector<unsigned __int64>::Resize(size) resizes the vector with a zero 64-bit value
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 TestU64;
#else
typedef unsigned long long TestU64;
#endif

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
template <class T> class TestTransferableVector { // CPersistContext_TransferableVector<T> model
public:
    TestTransferableVector(TestVector<T>& v) : TransVector(&v) { }
    virtual long Size() { return (long)TransVector->lastN; }
    virtual void Resize(long size) { TransVector->resize(size); }   // the reconstructed slot-1 body
    TestVector<T>* TransVector;
};

int main()
{
    TestVector<TestU64 > v;
    TestU64& x = v.lastX;
    x = 99;
    TestTransferableVector<TestU64 > tv(v);
    TestTransferableVector<TestU64 >* p = &tv;
    p->Resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x == 0)) {
        std::printf("00618fd0_TEST FAIL\n");
        return 1;
    }
    p->Resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x == 0)) {
        std::printf("00618fd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00618fd0_TEST PASS\n");
    return 0;
}
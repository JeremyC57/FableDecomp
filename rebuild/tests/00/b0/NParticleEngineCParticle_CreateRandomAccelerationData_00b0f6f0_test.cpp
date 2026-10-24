#include <cstdio>

// Model of NParticleEngine::CParticle::CreateRandomAccelerationData (retail 0x00B0F6F0): scoped_ptr reset to a new block.
static int g_live = 0;
static int g_deleted = 0;
struct CRandomAcceleration {
    unsigned long Seed;
    CRandomAcceleration(unsigned long& seed) : Seed(seed) { seed = seed * 0x24A1 + 0x24DF; ++g_live; }
    virtual ~CRandomAcceleration() { --g_live; ++g_deleted; }
};

template <class T> class scoped_ptr {
    T* ptr;
public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { delete ptr; }
    void reset(T* p = 0) { scoped_ptr<T>(p).swap(*this); }
    void swap(scoped_ptr& b) { T* tmp = b.ptr; b.ptr = ptr; ptr = tmp; }
    T* get() const { return ptr; }
};

struct CParticle {
    scoped_ptr<CRandomAcceleration> RandomAcceleration;
    void CreateRandomAccelerationData(unsigned long& seed) { RandomAcceleration.reset(new CRandomAcceleration(seed)); }
};

int main()
{
    {
        CParticle p;
        unsigned long seed = 5;
        p.CreateRandomAccelerationData(seed);
        if (g_live != 1 || g_deleted != 0 || p.RandomAcceleration.get() == 0 || p.RandomAcceleration.get()->Seed != 5) { std::printf("00b0f6f0_TEST FAIL\n"); return 1; }
        if (seed != 5u * 0x24A1u + 0x24DFu) { std::printf("00b0f6f0_TEST FAIL\n"); return 1; }
        CRandomAcceleration* first = p.RandomAcceleration.get();
        p.CreateRandomAccelerationData(seed);
        if (g_live != 1 || g_deleted != 1 || p.RandomAcceleration.get() == first) { std::printf("00b0f6f0_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0 || g_deleted != 2) { std::printf("00b0f6f0_TEST FAIL\n"); return 1; }
    std::printf("00b0f6f0_TEST PASS\n");
    return 0;
}
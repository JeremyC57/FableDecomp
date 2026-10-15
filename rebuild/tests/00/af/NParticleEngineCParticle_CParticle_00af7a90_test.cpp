// Behaviour test for the NParticleEngine::CParticle constructor (0x00AF7A90): base constructed,
// pointer and arrays zeroed, the reset deletes nothing on a fresh particle (and deletes a held
// object when one is present).
#include <cstdio>

static int g_baseCtors = 0;
static int g_deletes = 0;

struct CBase {
    virtual void v00() {}
    CBase() { ++g_baseCtors; }
};

namespace NParticleEngine {

struct CRandomAcceleration {
    virtual ~CRandomAcceleration() { ++g_deletes; }
};

struct CRandomAccelerationScopedPtr {
    CRandomAcceleration* m_p;
    CRandomAccelerationScopedPtr() : m_p(0) {}
    void reset(CRandomAcceleration* p = 0)
    {
        CRandomAcceleration* old = m_p;
        m_p = p;
        if (old)
            delete old;
    }
};

struct CLongArrayArray {
    void* m_pData; long m_size; long m_capacity;
    CLongArrayArray() : m_pData(0), m_size(0), m_capacity(0) {}
};

struct CPrimitiveHandleArray {
    void* m_pData; long m_size; long m_capacity;
    CPrimitiveHandleArray() : m_pData(0), m_size(0), m_capacity(0) {}
};

struct CParticle : CBase {
    unsigned char                _state[0x7c];
    CRandomAccelerationScopedPtr RandomAcceleration;
    CLongArrayArray              PrimIDList;
    CPrimitiveHandleArray        ParentHandleList;
    CParticle();
};

CParticle::CParticle()
{
    RandomAcceleration.reset();
}

} // namespace NParticleEngine

int main()
{
    NParticleEngine::CParticle* p = new NParticleEngine::CParticle();
    bool ok = g_baseCtors == 1 && g_deletes == 0 && p->RandomAcceleration.m_p == 0
        && p->PrimIDList.m_pData == 0 && p->PrimIDList.m_size == 0 && p->PrimIDList.m_capacity == 0
        && p->ParentHandleList.m_pData == 0 && p->ParentHandleList.m_size == 0 && p->ParentHandleList.m_capacity == 0;
    p->RandomAcceleration.m_p = new NParticleEngine::CRandomAcceleration();
    p->RandomAcceleration.reset();
    ok = ok && g_deletes == 1 && p->RandomAcceleration.m_p == 0;
    delete p;
    if (!ok) {
        std::printf("00af7a90_TEST FAIL\n");
        return 1;
    }
    std::printf("00af7a90_TEST PASS\n");
    return 0;
}
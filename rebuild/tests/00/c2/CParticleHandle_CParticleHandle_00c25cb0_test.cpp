// Behaviour test for CParticleHandle::~CParticleHandle (0x00C25CB0): drops one particle reference and clears the record.
#include <cstdio>
#include <new>

struct CParticle {
    unsigned short m_refCount;
    void DecRef() { --m_refCount; }
};

struct CParticleHandle {
    CParticle* m_pParticle;
    long       m_a;
    long       m_b;
    long       m_c;
    ~CParticleHandle();
};

CParticleHandle::~CParticleHandle()
{
    m_a = 0;
    m_b = 0;
    m_c = 0;
    m_pParticle->DecRef();
    m_pParticle = 0;
}

static CParticleHandle* volatile g_handle;

int main()
{
    CParticle particle; particle.m_refCount = 3;
    static double storage[4];
    CParticleHandle* h = new (storage) CParticleHandle;
    h->m_pParticle = &particle; h->m_a = 1; h->m_b = 2; h->m_c = 3;
    g_handle = h;
    h->~CParticleHandle();
    CParticleHandle* seen = g_handle;
    if (particle.m_refCount != 2 || seen->m_pParticle != 0 || seen->m_a != 0 || seen->m_b != 0 || seen->m_c != 0) {
        std::printf("00c25cb0_TEST FAIL\n"); return 1;
    }
    std::printf("00c25cb0_TEST PASS\n");
    return 0;
}
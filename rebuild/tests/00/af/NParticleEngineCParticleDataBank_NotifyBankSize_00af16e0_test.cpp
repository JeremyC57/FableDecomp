// Behaviour test for CParticleDataBank::NotifyBankSize (0x00AF16E0): the array is resized to
// `count` null entries and every entry gets the bank pointer and its own index.
#include <cstdio>

namespace NParticleEngine {

struct CParticleEmitterInfo {
    void*         Data;
    void*         Bank;
    unsigned long Index;
    CParticleEmitterInfo() : Data(0), Bank(0), Index(0) {}
};

} // namespace NParticleEngine

static NParticleEngine::CParticleEmitterInfo g_storage[16];
static unsigned long g_resizedTo = 999;

struct CParticleEmitterInfoArray {
    NParticleEngine::CParticleEmitterInfo* m_pData;
    long                                   m_size;
    long                                   m_capacity;
    void Resize(unsigned long count, const NParticleEngine::CParticleEmitterInfo& value)
    {
        m_pData = g_storage;
        for (unsigned long i = 0; i < count; ++i) g_storage[i] = value;
        m_size = (long)count;
        m_capacity = 16;
        g_resizedTo = count;
    }
    NParticleEngine::CParticleEmitterInfo& operator[](unsigned long i) { return m_pData[i]; }
};

struct CParticleDataBank {
    virtual void v00() {}
    unsigned char             _base[0x1dc];
    CParticleEmitterInfoArray ParticleEmitters;
    void NotifyBankSize(unsigned long count, void* typeCounts);
};

void CParticleDataBank::NotifyBankSize(unsigned long count, void* typeCounts)
{
    ParticleEmitters.Resize(count, NParticleEngine::CParticleEmitterInfo());
    for (unsigned long i = 0; i < count; ++i) {
        NParticleEngine::CParticleEmitterInfo& info = ParticleEmitters[i];
        info.Index = i;
        info.Bank = this;
    }
}

int main()
{
    static int dummy;
    CParticleDataBank bank;
    for (int i = 0; i < 16; ++i) { g_storage[i].Data = &dummy; g_storage[i].Bank = 0; g_storage[i].Index = 77; }
    bank.NotifyBankSize(5, 0);
    bool ok = g_resizedTo == 5 && bank.ParticleEmitters.m_size == 5;
    for (unsigned long i = 0; i < 5; ++i)
        ok = ok && g_storage[i].Data == 0 && g_storage[i].Bank == &bank && g_storage[i].Index == i;
    ok = ok && g_storage[5].Index == 77 && g_storage[5].Data == &dummy;
    bank.NotifyBankSize(0, 0);
    ok = ok && g_resizedTo == 0;
    if (!ok) {
        std::printf("00af16e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00af16e0_TEST PASS\n");
    return 0;
}
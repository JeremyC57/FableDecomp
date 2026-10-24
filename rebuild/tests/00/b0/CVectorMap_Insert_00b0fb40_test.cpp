#include <cstdio>

// Model of the particle update-parameter descriptor map (CVectorMap<K, CParticleUpdateParamDesc<K,K>>) and its Insert.
static int g_baseLive = 0;
struct CBaseClass {
    CBaseClass() { ++g_baseLive; }
    CBaseClass(const CBaseClass&) { ++g_baseLive; }
    virtual ~CBaseClass() { --g_baseLive; }
};
struct CCallback { int id; };
struct CParamDesc : CBaseClass {
    CCallback* Callback;
    unsigned long Data;
    CParamDesc(CCallback* callback, unsigned long data) : Callback(callback), Data(data) { }
};
struct Entry {
    unsigned long first;
    CCallback* callback;
    unsigned long data;
};
static int g_overflow = 0;
struct CParamDescMap {
    Entry storage[4];
    unsigned int count;
    unsigned int capacity;
    unsigned char Compare;
    bool Dirty;
    void Insert(const unsigned long& key, const CParamDesc& value)
    {
        CParamDesc copy(value);                      // the pair temporary's descriptor
        if (count != capacity) {
            storage[count].first = key; storage[count].callback = copy.Callback; storage[count].data = copy.Data;
            ++count;
        }
        else
            ++g_overflow;
        Dirty = true;
    }
};
struct CParams {
    CParamDescMap UpdateParams;
};
static void InitParams(CParams& p, unsigned int capacity) { p.UpdateParams.count = 0; p.UpdateParams.capacity = capacity; p.UpdateParams.Dirty = false; }
static unsigned long Crc(unsigned long seed, const char* data, unsigned long length)
{
    unsigned long crc = seed;
    for (unsigned long i = 0; i < length; ++i) {
        crc ^= (unsigned char)data[i];
        for (int b = 0; b < 8; ++b)
            crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320u : (crc >> 1);
    }
    return crc;
}

int main()
{
    CParams p; InitParams(p, 1);
    CCallback cb; cb.id = 3;
    CParamDesc d(&cb, 7);
    unsigned long key = 0x1234;
    p.UpdateParams.Insert(key, d);
    if (p.UpdateParams.count != 1 || p.UpdateParams.storage[0].first != 0x1234 || p.UpdateParams.storage[0].callback != &cb || p.UpdateParams.storage[0].data != 7 || !p.UpdateParams.Dirty) { std::printf("00b0fb40_TEST FAIL\n"); return 1; }
    p.UpdateParams.Dirty = false;
    p.UpdateParams.Insert(key, d);
    if (g_overflow != 1 || !p.UpdateParams.Dirty || g_baseLive != 1) { std::printf("00b0fb40_TEST FAIL\n"); return 1; }
    std::printf("00b0fb40_TEST PASS\n");
    return 0;
}
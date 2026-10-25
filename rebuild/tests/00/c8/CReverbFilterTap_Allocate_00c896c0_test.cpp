// Behaviour test for 00c896c0: CReverbFilterTap::Allocate -- size the ring as (length + 131) & ~3 floats, take/clear it from the cursor, reset the gain, total the bytes
#include <cstdio>
#include <cstring>

struct CDspObject {
    virtual ~CDspObject() {}
    CDspObject() {}
};
static int g_resets = 0;
struct CDspGain : CDspObject {
    float m_gain[4];
    void Reset() { ++g_resets; m_gain[0] = m_gain[1] = m_gain[2] = m_gain[3] = 1.0f; }
};
struct CRingBuffer {
    float* m_data;
    unsigned int m_size;
    unsigned int m_pos;
    void Clear() { std::memset(m_data, 0, m_size * sizeof(float)); }
};

struct CReverbFilterTap : CDspObject {
    CDspGain m_gain;
    CRingBuffer m_ring;
    unsigned int m_index;
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);
};

void CReverbFilterTap::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    unsigned int size = (length + 131) & ~3u;
    if (*pMemory) {
        m_ring.m_data = *pMemory;
        m_ring.m_size = size;
        m_ring.m_pos = 0;
        m_ring.Clear();
        *pMemory += size;
        m_index = index;
        m_gain.Reset();
    }
    *pBytes += size * sizeof(float);
}

int main()
{
    CReverbFilterTap a, b;
    unsigned int bytes = 0;
    float* none = 0;
    a.m_ring.m_data = 0; a.m_index = 99;
    a.Allocate(5, &none, &bytes, 3);          // (5 + 131) & ~3 = 136 floats
    b.Allocate(10, &none, &bytes, 4);         // (10 + 131) & ~3 = 140 floats
    int ok = bytes == (136 + 140) * 4 && none == 0 && a.m_index == 99 && a.m_ring.m_data == 0 && g_resets == 0;
    static float block[276 + 8];
    for (int i = 0; i < 284; i++) block[i] = 7.0f;
    float* cursor = block;
    unsigned int bytes2 = 0;
    a.m_ring.m_pos = 55;
    a.Allocate(5, &cursor, &bytes2, 3);
    b.Allocate(10, &cursor, &bytes2, 4);
    ok = ok && bytes2 == bytes && cursor == block + 276 && g_resets == 2
        && a.m_ring.m_data == block && a.m_ring.m_size == 136 && a.m_ring.m_pos == 0 && a.m_index == 3
        && b.m_ring.m_data == block + 136 && b.m_ring.m_size == 140 && b.m_index == 4 && a.m_gain.m_gain[2] == 1.0f;
    for (int i = 0; ok && i < 276; i++) if (block[i] != 0.0f) ok = 0;
    ok = ok && block[276] == 7.0f;
    if (!ok) { std::printf("00c896c0_TEST FAIL\n"); return 1; }
    std::printf("00c896c0_TEST PASS\n");
    return 0;
}
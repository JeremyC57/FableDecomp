// Behaviour test for 00c89aa0: CReverbFilterTap::Feedback -- ring[pos] = feedback[i] + sum_j blocks[j][i] * mixRow[j], wrapping the ring position
#include <cstdio>

struct CRingBuffer {
    float* m_data;
    unsigned int m_size;
    unsigned int m_pos;
};

struct CReverbFilterTap {
    CRingBuffer m_ring;
    void Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count);
};

void CReverbFilterTap::Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    for (unsigned int i = 0; i < count; i++) {
        float sum = feedback[i];
        for (unsigned int j = 0; j < 4; j++)
            sum += blocks[j][i] * mixRow[j];
        data[pos] = sum;
        if (pos + 1 < m_ring.m_size)
            pos = pos + 1;
        else
            pos = pos + 1 - m_ring.m_size;
    }
    m_ring.m_pos = pos;
}

static float blocks[4][128];
static float fb[128];
static float ring[8];

static int near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 128; i++)
            blocks[j][i] = (float)(j + 1) * (float)(i + 1);
    for (int i = 0; i < 128; i++) fb[i] = 100.0f * (float)i;
    const float row[4] = { 0.5f, -0.5f, -0.5f, -0.5f };
    for (int i = 0; i < 8; i++) ring[i] = -7.0f;
    CReverbFilterTap t;
    t.m_ring.m_data = ring; t.m_ring.m_size = 8; t.m_ring.m_pos = 5;
    t.Feedback(blocks, fb, row, 5);                  // writes ring[5], [6], [7], [0], [1]
    int ok = t.m_ring.m_pos == 2;
    unsigned int p = 5;
    for (int i = 0; ok && i < 5; i++) {
        float want = 100.0f * i + (i + 1) * (0.5f - 0.5f * 2 - 0.5f * 3 - 0.5f * 4);
        if (!near(ring[p], want)) ok = 0;
        p = (p + 1) % 8;
    }
    ok = ok && ring[2] == -7.0f && ring[4] == -7.0f;
    t.Feedback(blocks, fb, row, 0);
    ok = ok && t.m_ring.m_pos == 2;
    if (!ok) { std::printf("00c89aa0_TEST FAIL\n"); return 1; }
    std::printf("00c89aa0_TEST PASS\n");
    return 0;
}
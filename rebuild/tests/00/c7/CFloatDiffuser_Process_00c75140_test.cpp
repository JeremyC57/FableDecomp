// Test for CStereoDelayLine::Process 0x00C75140: two chained allpass lines with wrap, checked against a scalar reference.
#include <stdio.h>
#include <math.h>
struct CDelayLine {
    virtual ~CDelayLine() {}
    float* m_data;
    unsigned int m_size;
    unsigned int m_pos;
    float Allpass(float input)
    {
        float delayed = m_data[m_pos];
        float value = delayed * 0.7f + input;
        m_data[m_pos] = value;
        if (++m_pos == m_size)
            m_pos = 0;
        return delayed - value * 0.7f;
    }
};
struct CStereoDelayLine {
    virtual ~CStereoDelayLine() {}
    CDelayLine m_lines[2];
    void Process(float* buffer, unsigned long count);
};
void CStereoDelayLine::Process(float* buffer, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        *buffer = m_lines[1].Allpass(m_lines[0].Allpass(*buffer));
        buffer++;
    }
}
int main()
{
    float ring0[3] = { 1.0f, 2.0f, 3.0f }, ring1[2] = { 0.5f, -0.5f };
    float ref0[3] = { 1.0f, 2.0f, 3.0f }, ref1[2] = { 0.5f, -0.5f };
    CStereoDelayLine line;
    line.m_lines[0].m_data = ring0; line.m_lines[0].m_size = 3; line.m_lines[0].m_pos = 1;
    line.m_lines[1].m_data = ring1; line.m_lines[1].m_size = 2; line.m_lines[1].m_pos = 0;
    float block[7] = { 1, 2, 3, 4, 5, 6, 7 }, expect[7];
    unsigned p0 = 1, p1 = 0;
    for (int i = 0; i < 7; ++i) {
        float x = (float)(i + 1);
        float d = ref0[p0]; float v = d * 0.7f + x; ref0[p0] = v; p0 = (p0 + 1) % 3; x = d - v * 0.7f;
        d = ref1[p1]; v = d * 0.7f + x; ref1[p1] = v; p1 = (p1 + 1) % 2; expect[i] = d - v * 0.7f;
    }
    line.Process(block, 7);
    int ok = line.m_lines[0].m_pos == p0 && line.m_lines[1].m_pos == p1;
    for (int i = 0; i < 7; ++i) ok = ok && fabs(block[i] - expect[i]) < 1e-5f;
    for (int i = 0; i < 3; ++i) ok = ok && ring0[i] == ref0[i];
    for (int i = 0; i < 2; ++i) ok = ok && ring1[i] == ref1[i];
    printf(ok ? "00c75140_TEST PASS\n" : "00c75140_TEST FAIL\n");
    return ok ? 0 : 1;
}
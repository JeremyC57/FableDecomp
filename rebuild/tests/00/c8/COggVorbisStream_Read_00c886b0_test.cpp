// Behaviour test for 0x00C886B0 COggVorbisStream::Read (standalone copy of the logic; ov_read stubbed)
#include <cstdio>
struct SAudioBuffer { short* m_data; unsigned int m_channels; };
static int g_chunk, g_fail, g_calls;
static int ov_read_stub(char* buffer, int length)
{
    ++g_calls;
    if (g_fail && g_calls == 2) return -3;
    int n = length < g_chunk ? length : g_chunk;
    for (int i = 0; i < n; ++i) buffer[i] = 1;
    return n;
}
static int Read(int valid, SAudioBuffer* buffer, unsigned int frames)
{
    if (!valid) return -1;
    if (frames == 0) return 0;
    char* out = (char*)buffer->m_data;
    int remaining = buffer->m_channels * frames * 2;
    unsigned int done = 0;
    for (;;) {
        int got = ov_read_stub(out, remaining);
        if (got <= 0) { if (got < 0) return got; break; }
        done += got; out += got; remaining -= got;
        if (remaining == 0) break;
    }
    return done / (buffer->m_channels * 2);
}
int main()
{
    short data[64] = {0};
    SAudioBuffer b = { data, 2 };
    if (Read(0, &b, 4) != -1 || Read(1, &b, 0) != 0) { std::printf("00c886b0_TEST FAIL\n"); return 1; }
    g_chunk = 6; g_calls = 0;
    if (Read(1, &b, 10) != 10 || g_calls != 7) { std::printf("00c886b0_TEST FAIL\n"); return 1; }
    g_fail = 1; g_calls = 0;
    if (Read(1, &b, 10) != -3) { std::printf("00c886b0_TEST FAIL\n"); return 1; }
    std::printf("00c886b0_TEST PASS\n");
    return 0;
}
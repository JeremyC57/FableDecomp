#include <cstdio>
// COggVorbisStream::GetLength: -1 when invalid, else ov_pcm_total(&vf, -1) truncated to long.
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
static int g_link = 99;
static i64 ov_pcm_total(int* vf, int i) { g_link = i; return (i64)*vf; }
struct Stream {
    int m_valid; int m_vf;
    int GetLength() { if (!m_valid) return -1; return (int)ov_pcm_total(&m_vf, -1); }
};
int main()
{
    Stream s; s.m_valid = 0; s.m_vf = 1234;
    if (s.GetLength() != -1 || g_link != 99) { std::printf("00c88620_TEST FAIL\n"); return 1; }
    s.m_valid = 1;
    if (s.GetLength() != 1234 || g_link != -1) { std::printf("00c88620_TEST FAIL\n"); return 1; }
    std::printf("00c88620_TEST PASS\n");
    return 0;
}
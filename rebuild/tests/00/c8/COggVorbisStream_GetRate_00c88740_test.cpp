#include <cstdio>
// COggVorbisStream::GetRate: ov_info(-1)->rate, -1 when invalid or no info.
struct vorbis_info { int version; int channels; int rate; };
static vorbis_info* g_vi = 0;
static vorbis_info* ov_info(int* vf, int link) { return link == -1 ? g_vi : 0; }
struct Stream {
    int m_valid; int m_vf;
    int GetRate()
    {
        if (m_valid) {
            vorbis_info* vi = ov_info(&m_vf, -1);
            if (vi) return vi->rate;
        }
        return -1;
    }
};
int main()
{
    vorbis_info vi = { 0, 2, 44100 };
    Stream s; s.m_valid = 1; s.m_vf = 0;
    if (s.GetRate() != -1) { std::printf("00c88740_TEST FAIL\n"); return 1; }
    g_vi = &vi;
    if (s.GetRate() != 44100) { std::printf("00c88740_TEST FAIL\n"); return 1; }
    s.m_valid = 0;
    if (s.GetRate() != -1) { std::printf("00c88740_TEST FAIL\n"); return 1; }
    std::printf("00c88740_TEST PASS\n");
    return 0;
}
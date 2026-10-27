#include <cstdio>
// COggVorbisStream::Seek: no-op when already there; exact -> ov_pcm_seek, else ov_pcm_seek_page; -1 on failure.
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct VF { i64 pos; int fail; int exactCalls; int pageCalls; };
static i64 ov_pcm_tell(VF* vf) { return vf->pos; }
static int ov_pcm_seek(VF* vf, i64 p) { ++vf->exactCalls; if (vf->fail) return -1; vf->pos = p; return 0; }
static int ov_pcm_seek_page(VF* vf, i64 p) { ++vf->pageCalls; if (vf->fail) return -1; vf->pos = p - (p % 64); return 0; }
struct Stream {
    int m_valid; VF m_vf;
    int Seek(unsigned int position, bool exact)
    {
        if (!m_valid) return -1;
        if (ov_pcm_tell(&m_vf) == position) return (int)position;
        int result;
        if (exact) result = ov_pcm_seek(&m_vf, position);
        else result = ov_pcm_seek_page(&m_vf, position);
        if (result < 0) return -1;
        return (int)ov_pcm_tell(&m_vf);
    }
};
int main()
{
    Stream s; s.m_valid = 1; s.m_vf.pos = 100; s.m_vf.fail = 0; s.m_vf.exactCalls = 0; s.m_vf.pageCalls = 0;
    if (s.Seek(100, true) != 100 || s.m_vf.exactCalls != 0) { std::printf("00c88640_TEST FAIL\n"); return 1; }
    if (s.Seek(1000, true) != 1000 || s.m_vf.exactCalls != 1) { std::printf("00c88640_TEST FAIL\n"); return 1; }
    if (s.Seek(1000 + 70, false) != 1024 || s.m_vf.pageCalls != 1) { std::printf("00c88640_TEST FAIL\n"); return 1; }
    s.m_vf.fail = 1;
    if (s.Seek(5, true) != -1) { std::printf("00c88640_TEST FAIL\n"); return 1; }
    s.m_valid = 0;
    if (s.Seek(1024, true) != -1) { std::printf("00c88640_TEST FAIL\n"); return 1; }
    std::printf("00c88640_TEST PASS\n");
    return 0;
}
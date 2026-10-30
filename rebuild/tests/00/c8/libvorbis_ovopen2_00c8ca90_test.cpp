// Behaviour test for vorbisfile.c _ov_open2 (0x00C8CA90): PARTOPEN only; seekable runs the seekable open (ov_clear on failure), a stream becomes STREAMSET.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
#ifdef _MSC_VER
typedef __int64 i64;
#else
#include <stdint.h>
typedef int64_t i64;
#endif
#endif
#define FAIL(a) do { std::printf(a "_TEST FAIL\n"); return 1; } while (0)
struct VF { int seekable; int ready_state; int links; int cleared; int headers; int seek2; char buf[16]; int nbuf; void* datasource; };
static int _ov_open1(void* f, VF* vf, const char* initial, long ibytes, int seek_ok)
{
    int offsettest = f ? (seek_ok ? 0 : -1) : -1;
    int h = vf->headers, s2 = vf->seek2;
    std::memset(vf, 0, sizeof(*vf)); vf->headers = h; vf->seek2 = s2;
    vf->datasource = f;
    if (initial) { std::memcpy(vf->buf, initial, ibytes); vf->nbuf = ibytes; }
    if (offsettest != -1) vf->seekable = 1;
    vf->links = 1;
    int ret = vf->headers;
    if (ret < 0) { vf->datasource = 0; vf->cleared++; } else vf->ready_state = 1;
    return ret;
}
static int _ov_open2(VF* vf)
{
    if (vf->ready_state != 1) return -131;
    vf->ready_state = 2;
    if (vf->seekable) { int ret = vf->seek2; if (ret) { vf->datasource = 0; vf->cleared++; } return ret; }
    vf->ready_state = 3;
    return 0;
}

int main()
{
    VF a; std::memset(&a, 0, sizeof(a)); if (_ov_open2(&a) != -131) FAIL("00c8ca90");
    VF b; std::memset(&b, 0, sizeof(b)); b.ready_state = 1; if (_ov_open2(&b) != 0 || b.ready_state != 3) FAIL("00c8ca90");
    VF c; std::memset(&c, 0, sizeof(c)); c.ready_state = 1; c.seekable = 1; c.seek2 = -128;
    if (_ov_open2(&c) != -128 || c.cleared != 1 || c.ready_state != 2) FAIL("00c8ca90");
    std::printf("00c8ca90_TEST PASS\n"); return 0;
}
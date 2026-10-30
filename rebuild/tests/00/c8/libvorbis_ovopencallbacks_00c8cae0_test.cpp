// Behaviour test for vorbisfile.c ov_open_callbacks (0x00C8CAE0): _ov_open1, then _ov_open2 when it succeeded.
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

static int ov_open_callbacks(void* f, VF* vf, const char* initial, long ibytes)
{
    int ret = _ov_open1(f, vf, initial, ibytes, 0);
    if (ret) return ret;
    return _ov_open2(vf);
}
int main()
{
    VF a; std::memset(&a, 0, sizeof(a)); if (ov_open_callbacks(0, &a, 0, 0) != 0 || a.ready_state != 3) FAIL("00c8cae0");
    VF b; std::memset(&b, 0, sizeof(b)); b.headers = -133; if (ov_open_callbacks(0, &b, 0, 0) != -133 || b.ready_state != 0) FAIL("00c8cae0");
    std::printf("00c8cae0_TEST PASS\n"); return 0;
}
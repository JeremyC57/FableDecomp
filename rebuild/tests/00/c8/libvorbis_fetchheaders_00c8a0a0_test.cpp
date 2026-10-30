// Behaviour test for vorbisfile.c _fetch_headers (0x00C8A0A0): read the first page unless given, then the three header packets; OV_EBADHEADER on a hole, OV_ENOTVORBIS without a page.
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
struct VF { int ready_state; long serialno; int packets[6]; int np; int pages; };
static int packetout(VF* vf) { return vf->np < 6 ? vf->packets[vf->np++] : 0; }
static int _fetch_headers(VF* vf, long* serialno, int have_page)
{
    if (!have_page) { if (vf->pages-- <= 0) return -132; }
    if (serialno) *serialno = 1234;
    vf->ready_state = 3;
    int i = 0, ret;
    while (i < 3) {
        while (i < 3) {
            int result = packetout(vf);
            if (result == 0) break;
            if (result == -1) { ret = -133; goto bail; }
            i++;
        }
        if (i < 3) if (vf->pages-- <= 0) { ret = -133; goto bail; }
    }
    return 0;
bail:
    vf->ready_state = 2;
    return ret;
}
int main()
{
    long sn = 0;
    VF ok = { 0, 0, { 1, 0, 1, 1, 0, 0 }, 0, 2 };
    if (_fetch_headers(&ok, &sn, 0) != 0 || sn != 1234 || ok.ready_state != 3) FAIL("00c8a0a0");
    VF hole = { 0, 0, { 1, -1, 0, 0, 0, 0 }, 0, 2 };
    if (_fetch_headers(&hole, 0, 1) != -133 || hole.ready_state != 2) FAIL("00c8a0a0");
    VF none = { 0, 0, { 0 }, 0, 0 };
    if (_fetch_headers(&none, 0, 0) != -132) FAIL("00c8a0a0");
    std::printf("00c8a0a0_TEST PASS\n"); return 0;
}
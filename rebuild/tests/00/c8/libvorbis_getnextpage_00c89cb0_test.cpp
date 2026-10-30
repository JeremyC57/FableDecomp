// Behaviour test for vorbisfile.c _get_next_page (0x00C89CB0): skip garbage (negative pageseek), fetch more data at need, stop at the boundary; returns the page offset.
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
struct VF { i64 offset; int seq[8]; int n; int data; };
static long pageseek(VF* vf) { return vf->n < 8 ? vf->seq[vf->n++] : 0; }
static long get_data(VF* vf) { return vf->data-- > 0 ? 100 : 0; }
static i64 _get_next_page(VF* vf, i64 boundary)
{
    if (boundary > 0) boundary += vf->offset;
    while (1) {
        long more;
        if (boundary > 0 && vf->offset >= boundary) return -1;
        more = pageseek(vf);
        if (more < 0) vf->offset -= more;
        else if (more == 0) {
            if (!boundary) return -1;
            long ret = get_data(vf);
            if (ret == 0) return -2;
            if (ret < 0) return -128;
        } else { i64 ret = vf->offset; vf->offset += more; return ret; }
    }
}
int main()
{
    VF vf = { 1000, { -7, 0, 50, 0, 0, 0, 0, 0 }, 0, 1 };
    if (_get_next_page(&vf, -1) != 1007 || vf.offset != 1057) FAIL("00c89cb0");
    if (_get_next_page(&vf, -1) != -2) FAIL("00c89cb0");          /* pageseek 0, no more data -> OV_EOF */
    VF b = { 0, { -10, 0, 0, 0, 0, 0, 0, 0 }, 0, 0 };
    if (_get_next_page(&b, 5) != -1) FAIL("00c89cb0");            /* boundary passed -> OV_FALSE */
    VF z = { 0, { 0, 0, 0, 0, 0, 0, 0, 0 }, 0, 5 };
    if (_get_next_page(&z, 0) != -1) FAIL("00c89cb0");            /* no boundary -> OV_FALSE without reading */
    std::printf("00c89cb0_TEST PASS\n"); return 0;
}
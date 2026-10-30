// Behaviour test for vorbisfile.c _get_prev_page (0x00C89D70): step back 8500 bytes at a time until a page starts before the current offset; returns the last such page.
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
/* a file of pages: page k starts at off[k]; _get_next_page finds the first page starting in [offset, offset+boundary) */
struct VF { i64 offset; i64 end; int npages; i64 off[8]; int serial[8]; i64 gran[8]; int hit; };
static void _seek_helper(VF* vf, i64 o) { vf->offset = o; }
static i64 _get_next_page(VF* vf, int* page, i64 boundary)
{
    for (int k = 0; k < vf->npages; k++)
        if (vf->off[k] >= vf->offset) {
            if (boundary > 0 && vf->off[k] >= vf->offset + boundary) return -1;
            *page = k; vf->offset = (k + 1 < vf->npages) ? vf->off[k + 1] : vf->end; return vf->off[k];
        }
    return -1;
}

static i64 _get_prev_page(VF* vf, int* page)
{
    i64 begin = vf->offset, end = begin, ret, offset = -1;
    while (offset == -1) {
        begin -= 8500; if (begin < 0) begin = 0;
        _seek_helper(vf, begin);
        while (vf->offset < end) {
            ret = _get_next_page(vf, page, end - vf->offset);
            if (ret == -128) return -128;
            if (ret < 0) break; else offset = ret;
        }
    }
    _seek_helper(vf, offset);
    ret = _get_next_page(vf, page, 8500);
    if (ret < 0) return -129;
    return offset;
}
int main()
{
    VF vf = { 20000, 20000, 4, { 0, 4000, 9000, 15000 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 }, 0 };
    int page = -1;
    if (_get_prev_page(&vf, &page) != 15000 || page != 3) FAIL("00c89d70");
    VF w = { 30000, 30000, 2, { 0, 1000 }, { 1, 1 }, { 0, 0 }, 0 };     /* needs several 8500-byte steps back */
    if (_get_prev_page(&w, &page) != 1000 || page != 1) FAIL("00c89d70");
    std::printf("00c89d70_TEST PASS\n"); return 0;
}
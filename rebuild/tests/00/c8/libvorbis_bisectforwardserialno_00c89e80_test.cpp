// Behaviour test for vorbisfile.c _bisect_forward_serialno (0x00C89E80): split a chained file into links by serial number (offsets/serialnos per link).
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

struct Links { int links; i64 offsets[8]; long serialnos[8]; };
static Links L;
static int _bisect_forward_serialno(VF* vf, i64 begin, i64 searched, i64 end, long currentno, long m)
{
    i64 endsearched = end, next = end, ret; int pg = 0;
    while (searched < endsearched) {
        i64 bisect = (endsearched - searched < 8500) ? searched : (searched + endsearched) / 2;
        _seek_helper(vf, bisect);
        ret = _get_next_page(vf, &pg, -1);
        if (ret == -128) return -128;
        if (ret < 0 || vf->serial[pg] != currentno) { endsearched = bisect; if (ret >= 0) next = ret; }
        else searched = vf->offset;     /* ret + header_len + body_len: the page end */
    }
    _seek_helper(vf, next);
    ret = _get_next_page(vf, &pg, -1);
    if (ret == -128) return -128;
    if (searched >= end || ret < 0) { L.links = m + 1; L.offsets[m + 1] = searched; }
    else if (_bisect_forward_serialno(vf, next, vf->offset, end, vf->serial[pg], m + 1) == -128) return -128;
    L.offsets[m] = begin; L.serialnos[m] = currentno;
    return 0;
}
int main()
{
    /* two links: serial 7 pages at 0/3000/6000, serial 9 pages at 9000/12000; file end 15000 */
    VF vf = { 0, 15000, 5, { 0, 3000, 6000, 9000, 12000 }, { 7, 7, 7, 9, 9 }, { 0, 0, 0, 0, 0 }, 0 };
    if (_bisect_forward_serialno(&vf, 0, 0, 15001, 7, 0) != 0) FAIL("00c89e80");
    if (L.links != 2 || L.offsets[0] != 0 || L.offsets[1] != 9000 || L.serialnos[0] != 7 || L.serialnos[1] != 9
        || L.offsets[2] != 15000) FAIL("00c89e80");
    std::printf("00c89e80_TEST PASS\n"); return 0;
}
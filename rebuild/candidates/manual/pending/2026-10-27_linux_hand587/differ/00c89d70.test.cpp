// Behaviour test for libvorbis _get_prev_page (0x00C89D70): stepping back CHUNKSIZE at a time finds the last page that
// starts before the current offset, re-reads it, and reports OV_EREAD / OV_EFAULT, over a mock page scanner.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
#define CHUNKSIZE 8500
#define OV_FALSE -1
#define OV_EREAD -128
#define OV_EFAULT -129
struct File { i64 offset; i64 pages[16]; int npages; i64 length; bool readError; int seeks; };
static void _seek_helper(File* vf, i64 offset) { vf->offset = offset; ++vf->seeks; }
// next page starting at or after vf->offset, below vf->offset + boundary (boundary > 0)
static i64 _get_next_page(File* vf, i64 boundary)
{
    if (vf->readError) return OV_EREAD;
    i64 limit = vf->offset + boundary;
    for (int i = 0; i < vf->npages; i++) {
        if (vf->pages[i] >= vf->offset) {
            if (vf->pages[i] >= limit) { vf->offset = limit; return OV_FALSE; }
            i64 ret = vf->pages[i];
            vf->offset = (i + 1 < vf->npages) ? vf->pages[i + 1] : vf->length;
            return ret;
        }
    }
    vf->offset = vf->length;
    return OV_FALSE;
}
static i64 _get_prev_page(File* vf)
{
    i64 begin = vf->offset, end = begin, ret, offset = -1;
    while (offset == -1) {
        begin -= CHUNKSIZE;
        if (begin < 0) begin = 0;
        _seek_helper(vf, begin);
        while (vf->offset < end) {
            ret = _get_next_page(vf, end - vf->offset);
            if (ret == OV_EREAD) return OV_EREAD;
            if (ret < 0) break; else offset = ret;
        }
    }
    _seek_helper(vf, offset);
    ret = _get_next_page(vf, CHUNKSIZE);
    if (ret < 0) return OV_EFAULT;
    return offset;
}
int main()
{
    File f; f.npages = 0; f.readError = false; f.seeks = 0; f.length = 40000;
    for (i64 p = 0; p < 40000; p += 4000) f.pages[f.npages++] = p;      // pages every 4000 bytes
    f.offset = 40000;
    bool ok = _get_prev_page(&f) == 36000 && f.offset == 40000;
    f.offset = 22000; f.seeks = 0;
    ok = ok && _get_prev_page(&f) == 20000;
    File g = f; g.npages = 2; g.pages[0] = 0; g.pages[1] = 100; g.offset = 30000; g.length = 30000;   // far back: several chunks
    ok = ok && _get_prev_page(&g) == 100 && g.seeks >= 4;
    File h = f; h.offset = 30000; h.readError = true;
    ok = ok && _get_prev_page(&h) == OV_EREAD;
    if (!ok) { std::printf("00c89d70_TEST FAIL\n"); return 1; }
    std::printf("00c89d70_TEST PASS\n");
    return 0;
}

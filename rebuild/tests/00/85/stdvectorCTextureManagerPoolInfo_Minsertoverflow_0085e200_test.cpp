// Behaviour test for 0x0085E200 vector<CTextureManagerPoolInfo>::_M_insert_overflow: the new block holds max(2*size, size+fill_len) 20-byte records; head, fill_len copies of x and (unless atend) the tail are copied in order; the old block is freed.
#include <cstdio>
#include <cstdlib>
#include <new>
struct Info {
    unsigned int Width, Height, PixelFormat, UseMipmaps, Count;
    Info(unsigned int w) : Width(w), Height(w + 1), PixelFormat(3), UseMipmaps(1), Count(0) { }
    Info(const Info& o) : Width(o.Width), Height(o.Height), PixelFormat(o.PixelFormat), UseMipmaps(o.UseMipmaps), Count(o.Count) { }
};
struct Vec {
    Info* start; Info* finish; Info* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Info* pos, const Info& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Info* ns = len ? (Info*)std::malloc(len * sizeof(Info)) : 0;
        Info* nf = ns;
        for (Info* p = start; p != pos; ++p, ++nf) new (nf) Info(*p);
        for (unsigned int i = 0; i < fill_len; ++i, ++nf) new (nf) Info(x);
        if (!atend) for (Info* p = pos; p != finish; ++p, ++nf) new (nf) Info(*p);
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
static int fail() { std::printf("0085e200_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(Info) != 20) return fail();
    Vec v; v.start = 0; v.finish = 0; v.eos = 0;
    Info a(10), b(20), c(30);
    v.insert_overflow(v.finish, a, 1, true);           // empty: len = 0 + max(0, 1) = 1
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].Width != 10) return fail();
    v.insert_overflow(v.finish, b, 1, true);           // len 2
    v.insert_overflow(v.start, c, 1, false);           // front insert: {30, 10, 20}, len 4
    if (v.size() != 3 || v.eos - v.start != 4 || v.start[0].Width != 30 || v.start[1].Width != 10 ||
        v.start[2].Width != 20 || v.start[2].Height != 21) return fail();
    std::free(v.start);
    std::printf("0085e200_TEST PASS\n");
    return 0;
}
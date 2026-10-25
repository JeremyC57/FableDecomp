// Behaviour test for 0x008657E0 vector<CFlash>::_M_insert_overflow: max(2*size, size+fill_len) slots; head, fill_len copies of x through the copy constructor and (unless atend) the tail; every old element destroyed and the old block freed.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0, g_destroyed = 0;
struct Flash {
    void* geometry; long CreationFrame;
    Flash(long f) : geometry(0), CreationFrame(f) { }
    Flash(const Flash& o) : geometry(o.geometry), CreationFrame(o.CreationFrame) { ++g_copies; }
    ~Flash() { ++g_destroyed; }
};
struct Vec {
    Flash* start; Flash* finish; Flash* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Flash* pos, const Flash& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Flash* ns = len ? (Flash*)std::malloc(len * sizeof(Flash)) : 0;
        Flash* nf = ns;
        for (Flash* p = start; p != pos; ++p, ++nf) new (nf) Flash(*p);
        if (fill_len == 1) { new (nf) Flash(x); ++nf; }
        else for (unsigned int i = 0; i < fill_len; ++i, ++nf) new (nf) Flash(x);
        if (!atend) for (Flash* p = pos; p != finish; ++p, ++nf) new (nf) Flash(*p);
        for (Flash* p = start; p != finish; ++p) p->~Flash();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
static int fail() { std::printf("008657e0_TEST FAIL\n"); return 1; }
int main()
{
    Vec v; v.start = 0; v.finish = 0; v.eos = 0;
    Flash a(1), b(2);
    v.insert_overflow(v.finish, a, 1, true);
    if (v.size() != 1 || g_copies != 1 || g_destroyed != 0) return fail();
    v.insert_overflow(v.start, b, 2, false);           // len 1 + 2 = 3: {2, 2, 1}
    if (v.size() != 3 || v.eos - v.start != 3 || g_copies != 4 || g_destroyed != 1 ||
        v.start[0].CreationFrame != 2 || v.start[1].CreationFrame != 2 || v.start[2].CreationFrame != 1) return fail();
    for (Flash* p = v.start; p != v.finish; ++p) p->~Flash();
    std::free(v.start);
    std::printf("008657e0_TEST PASS\n");
    return 0;
}
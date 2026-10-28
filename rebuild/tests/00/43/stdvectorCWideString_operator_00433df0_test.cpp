// behaviour test for 0x00433DF0 std::vector<CWideString>::operator= (STLport 4.6 copy assignment, all three arms)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
class CWideString {                                      // model: a counted string id
public:
    int Id;
    CWideString(int id) : Id(id) { ++g_live; }
    CWideString(const CWideString& o) : Id(o.Id) { ++g_live; }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const CWideString& o) { Id = o.Id; return *this; }
};
struct Vec {
    CWideString* Start; CWideString* Finish; CWideString* End;
    Vec() : Start(0), Finish(0), End(0) { }
    unsigned size() const { return (unsigned)(Finish - Start); }
    unsigned capacity() const { return (unsigned)(End - Start); }
    void reserve_ids(const int* ids, unsigned n, unsigned cap)
    {
        Start = (CWideString*)std::malloc(cap * sizeof(CWideString));
        for (unsigned i = 0; i < n; ++i) new (Start + i) CWideString(ids[i]);
        Finish = Start + n; End = Start + cap;
    }
    void clear_all() { for (CWideString* p = Start; p != Finish; ++p) p->~CWideString(); std::free(Start); Start = Finish = End = 0; }
    Vec& assign(const Vec& x)                            // the retail algorithm
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                CWideString* tmp = xlen ? (CWideString*)std::malloc(xlen * sizeof(CWideString)) : 0;
                CWideString* cur = tmp;
                for (const CWideString* p = x.Start; p != x.Finish; ++p, ++cur) new (cur) CWideString(*p);
                for (CWideString* p = Start; p != Finish; ++p) p->~CWideString();
                if (Start) std::free(Start);
                Start = tmp; End = Start + xlen;
            } else if (size() >= xlen) {
                CWideString* d = Start;
                for (const CWideString* p = x.Start; p != x.Finish; ++p, ++d) *d = *p;
                for (CWideString* p = d; p != Finish; ++p) p->~CWideString();
            } else {
                unsigned n = size();
                for (unsigned i = 0; i < n; ++i) Start[i] = x.Start[i];
                CWideString* cur = Finish;
                for (const CWideString* p = x.Start + n; p != x.Finish; ++p, ++cur) new (cur) CWideString(*p);
            }
            Finish = Start + xlen;
        }
        return *this;
    }
};
static bool same(const Vec& v, const int* ids, unsigned n)
{
    if (v.size() != n) return false;
    for (unsigned i = 0; i < n; ++i) if (v.Start[i].Id != ids[i]) return false;
    return true;
}
int main()
{
    const int a[] = { 1, 2, 3, 4, 5 }, b[] = { 7, 8 }, c[] = { 9, 10, 11 };
    Vec src, dst;
    src.reserve_ids(a, 5, 5); dst.reserve_ids(b, 2, 2);
    dst.assign(src);                                    // realloc arm
    bool ok = same(dst, a, 5) && dst.capacity() == 5;
    Vec small; small.reserve_ids(b, 2, 2);
    dst.assign(small);                                  // shrink arm
    ok = ok && same(dst, b, 2) && dst.capacity() == 5;
    Vec mid; mid.reserve_ids(c, 3, 3);
    dst.assign(mid);                                    // grow-within-capacity arm
    ok = ok && same(dst, c, 3);
    dst.assign(dst);                                    // self-assignment is a no-op
    ok = ok && same(dst, c, 3);
    src.clear_all(); dst.clear_all(); small.clear_all(); mid.clear_all();
    ok = ok && g_live == 0;
    std::printf(ok ? "00433df0_TEST PASS\n" : "00433df0_TEST FAIL\n");
    return ok ? 0 : 1;
}
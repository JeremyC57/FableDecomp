// Behaviour test for 0x006DCA50 vector<16-byte record>::_M_insert_overflow: the new block holds size+max(size,n)
// elements, the prefix, n copies of x (one copy-construct when n == 1), and the suffix unless atend.
#include <cstdio>
#include <cstdlib>
#include <new>

struct SPair8 { int first; int second; };
struct Entry {
    int Pairs[3];
    int Value;
};

struct Vec {
    Entry* s; Entry* f; Entry* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    void clear() { std::free(s); }
    void InsertOverflow(Entry* pos, const Entry& x, unsigned int n, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < n ? n : old);
        Entry* ns = len ? (Entry*)std::malloc(len * sizeof(Entry)) : 0;
        Entry* nf = ns;
        for (Entry* p = s; p != pos; ++p, ++nf) new (nf) Entry(*p);
        if (n == 1) { new (nf) Entry(x); ++nf; }
        else for (unsigned int k = n; k > 0; --k, ++nf) new (nf) Entry(x);
        if (!atend) for (Entry* p = pos; p != f; ++p, ++nf) new (nf) Entry(*p);
        clear();
        s = ns; f = nf; e = ns + len;
    }
};

static Entry Make(int v) { Entry r; r.Pairs[0] = r.Pairs[1] = r.Pairs[2] = 0; r.Value = v; return r; }

int main()
{
    Vec v; v.s = (Entry*)std::malloc(2 * sizeof(Entry)); v.f = v.s + 2; v.e = v.f;
    v.s[0] = Make(1); v.s[1] = Make(2);
    Entry x = Make(9);
    v.InsertOverflow(v.s + 1, x, 3, false);                 // len = 2 + max(2,3) = 5
    if (v.size() != 5 || v.e - v.s != 5 || v.s[0].Value != 1 || v.s[1].Value != 9 || v.s[3].Value != 9 || v.s[4].Value != 2) {
        std::printf("006dca50_TEST FAIL\n"); return 1;
    }
    v.InsertOverflow(v.f, Make(7), 1, true);                // len = 5 + 5 = 10, append one
    if (v.size() != 6 || v.e - v.s != 10 || v.s[5].Value != 7 || v.s[4].Value != 2) { std::printf("006dca50_TEST FAIL\n"); return 1; }
    v.clear();
    std::printf("006dca50_TEST PASS\n");
    return 0;
}
// Behaviour test for vector<CSwitchableBlockage>::operator= (0x0051BCD0): the three STLport arms (grow, shrink,
// partial overlap) through a copy of the model with counted element copies / assignments / destructions.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0, g_assigns = 0, g_dtors = 0;

struct Blockage {                       // stands in for CSwitchableBlockage (key + list)
    unsigned int key;
    int payload;
    Blockage(unsigned int k, int p) : key(k), payload(p) {}
    Blockage(const Blockage& o) : key(o.key), payload(o.payload) { ++g_copies; }
    Blockage& operator=(const Blockage& o) { key = o.key; payload = o.payload; ++g_assigns; return *this; }
    ~Blockage() { ++g_dtors; }
};

static Blockage* UninitCopy(const Blockage* first, const Blockage* last, Blockage* result)
{
    for (; first != last; ++first, ++result) new (result) Blockage(*first);
    return result;
}
static Blockage* Copy(const Blockage* first, const Blockage* last, Blockage* result)
{
    for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
static void Destroy(Blockage* first, Blockage* last) { for (; first != last; ++first) first->~Blockage(); }

struct BlockageVector {
    Blockage* start; Blockage* finish; Blockage* eos;
    BlockageVector() : start(0), finish(0), eos(0) {}
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    BlockageVector& operator=(const BlockageVector& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                Blockage* tmp = xlen != 0 ? (Blockage*)std::malloc(xlen * sizeof(Blockage)) : 0;
                UninitCopy(x.start, x.finish, tmp);
                Destroy(start, finish);
                if (start) std::free(start);
                start = tmp;
                eos = start + xlen;
            } else if (size() >= xlen) {
                Blockage* i = Copy(x.start, x.finish, start);
                Destroy(i, finish);
            } else {
                Copy(x.start, x.start + size(), start);
                UninitCopy(x.start + size(), x.finish, finish);
            }
            finish = start + xlen;
        }
        return *this;
    }
};

static void Fill(BlockageVector& v, unsigned int n, unsigned int cap, int base)
{
    v.start = (Blockage*)std::malloc(cap * sizeof(Blockage));
    for (unsigned int i = 0; i < n; ++i) new (v.start + i) Blockage(i, base + int(i));
    v.finish = v.start + n;
    v.eos = v.start + cap;
}
static bool Same(const BlockageVector& a, const BlockageVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i)
        if (a.start[i].key != b.start[i].key || a.start[i].payload != b.start[i].payload) return false;
    return true;
}

int main()
{
    BlockageVector src, a, b, c;
    Fill(src, 5, 5, 100);
    Fill(a, 2, 3, 0);                   // capacity 3 < 5: grow arm
    g_copies = g_assigns = g_dtors = 0;
    a = src;
    if (!Same(a, src) || g_copies != 5 || g_assigns != 0 || g_dtors != 2 || a.capacity() != 5) {
        std::printf("0051bcd0_TEST FAIL\n"); return 1;
    }
    Fill(b, 7, 8, 50);                  // size 7 >= 5: assign 5, destroy 2
    g_copies = g_assigns = g_dtors = 0;
    b = src;
    if (!Same(b, src) || g_copies != 0 || g_assigns != 5 || g_dtors != 2 || b.capacity() != 8) {
        std::printf("0051bcd0_TEST FAIL\n"); return 1;
    }
    Fill(c, 3, 6, 9);                   // size 3 < 5 <= capacity 6: assign 3, construct 2
    g_copies = g_assigns = g_dtors = 0;
    c = src;
    if (!Same(c, src) || g_copies != 2 || g_assigns != 3 || g_dtors != 0 || c.capacity() != 6) {
        std::printf("0051bcd0_TEST FAIL\n"); return 1;
    }
    g_copies = g_assigns = g_dtors = 0;
    c = c;                              // self-assignment is a no-op
    if (g_copies || g_assigns || g_dtors || !Same(c, src)) { std::printf("0051bcd0_TEST FAIL\n"); return 1; }
    std::printf("0051bcd0_TEST PASS\n");
    return 0;
}
// Behaviour test for std::vector<CCombatWheel::SSlot>::operator= (0x006F3C20): self-assignment is a no-op; a larger
// source reallocates and copies; a smaller one assigns and destroys the tail; an in-between one assigns then
// copy-constructs the rest; the size always ends equal to the source's.
#include <cstdio>
#include <cstdlib>

static int g_live = 0;
struct SSlot { long Value; };
static void Construct(SSlot* p, const SSlot& s) { p->Value = s.Value; ++g_live; }
static void Destroy(SSlot* p) { p->Value = -1; --g_live; }
struct CSlotVector {
    SSlot* Start; SSlot* Finish; SSlot* End;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    unsigned int capacity() const { return (unsigned int)(End - Start); }
    void Clear() { for (SSlot* p = Start; p != Finish; ++p) Destroy(p); std::free(Start); Start = Finish = End = 0; }
    CSlotVector& operator=(const CSlotVector& x)
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                SSlot* tmp = xlen ? (SSlot*)std::malloc(xlen * sizeof(SSlot)) : 0;
                for (unsigned int i = 0; i < xlen; ++i) Construct(tmp + i, x.Start[i]);
                Clear();
                Start = tmp; End = Start + xlen;
            } else if (size() >= xlen) {
                for (unsigned int i = 0; i < xlen; ++i) Start[i] = x.Start[i];
                for (SSlot* p = Start + xlen; p != Finish; ++p) Destroy(p);
            } else {
                unsigned int n = size();
                for (unsigned int i = 0; i < n; ++i) Start[i] = x.Start[i];
                for (unsigned int i = n; i < xlen; ++i) Construct(Finish + (i - n), x.Start[i]);
            }
            Finish = Start + xlen;
        }
        return *this;
    }
};
static void Make(CSlotVector& v, unsigned int cap, unsigned int n, long base)
{
    v.Start = (SSlot*)std::malloc(cap * sizeof(SSlot)); v.Finish = v.Start; v.End = v.Start + cap;
    for (unsigned int i = 0; i < n; ++i) { SSlot s; s.Value = base + (long)i; Construct(v.Finish++, s); }
}

int main()
{
    CSlotVector a, b, c;
    Make(a, 2, 2, 10); Make(b, 5, 4, 20); Make(c, 8, 1, 30);
    a = a;
    bool ok = a.size() == 2 && a.Start[1].Value == 11;
    a = b;                                       // grow: reallocate
    ok = ok && a.size() == 4 && a.capacity() == 4 && a.Start[3].Value == 23;
    b = c;                                       // shrink: assign + destroy tail
    ok = ok && b.size() == 1 && b.Start[0].Value == 30 && b.Start[1].Value == -1;
    c = a;                                       // fits: assign 1, construct 3
    ok = ok && c.size() == 4 && c.Start[0].Value == 20 && c.Start[3].Value == 23;
    a.Clear(); b.Clear(); c.Clear();
    ok = ok && g_live == 0;
    if (!ok) { std::printf("006f3c20_TEST FAIL\n"); return 1; }
    std::printf("006f3c20_TEST PASS\n");
    return 0;
}
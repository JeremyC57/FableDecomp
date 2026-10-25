// Behaviour test for 0x0075F860 std::vector<CKeyCamera>::_M_insert_overflow: growth (size + max(size, n)), single and multi-copy insertion in the middle, copy/destroy balance
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

// CKeyCamera: delta index, position, look direction, lens/timing floats, an owned event-name string (CCharString,
// copied by the out-of-line copy constructor 0x0075F1F0) and the speed fraction.
int g_live = 0;
struct C3DVector { float X, Y, Z; };
struct Elem {
    float     DeltaIndex;
    C3DVector Pos;
    C3DVector LookDirection;
    float     FOV, Roll, ShuttleSpeed, Duration, PauseTime;
    char*     EventName;
    float     SpeedFraction;
    explicit Elem(long k) : DeltaIndex((float)k), FOV(60.0f), Roll(0.0f), ShuttleSpeed(1.0f), Duration((float)k), PauseTime(0.0f),
        EventName(new char[8]), SpeedFraction(0.5f)
    {
        Pos.X = (float)k; Pos.Y = (float)(k + 1); Pos.Z = (float)(k + 2);
        LookDirection.X = 0.0f; LookDirection.Y = 1.0f; LookDirection.Z = 0.0f;
        std::sprintf(EventName, "cam%ld", k);
        ++g_live;
    }
    Elem(const Elem& o) : DeltaIndex(o.DeltaIndex), Pos(o.Pos), LookDirection(o.LookDirection), FOV(o.FOV), Roll(o.Roll),
        ShuttleSpeed(o.ShuttleSpeed), Duration(o.Duration), PauseTime(o.PauseTime), EventName(new char[8]), SpeedFraction(o.SpeedFraction)
    {
        for (int i = 0; i < 8; ++i) EventName[i] = o.EventName[i];
        ++g_live;
    }
    ~Elem() { delete[] EventName; --g_live; }
};
Elem make(long k) { return Elem(k); }
long key(const Elem& x) { return (long)x.DeltaIndex; }
bool check(const Elem* p, unsigned int n, const Elem&, const Elem&, const Elem& c)
{
    for (unsigned int i = 0; i < n; ++i) {
        long k = (long)p[i].DeltaIndex;
        if (p[i].EventName == c.EventName) return false;          // deep copies
        if (p[i].EventName[0] != 'c' || p[i].EventName[3] != (char)('0' + k)) return false;
        if (p[i].Pos.Z != (float)(k + 2) || p[i].LookDirection.Y != 1.0f || p[i].SpeedFraction != 0.5f) return false;
    }
    return g_live == 8;
}
bool final_check() { return g_live == 0; }

struct Vec {
    Elem* s;
    Elem* f;
    Elem* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    // STLport _M_insert_overflow: len = size + max(size, n); copy head, construct x (once or n times), copy tail
    // unless atend, destroy + free the old block, adopt the new one.
    void insert_overflow(Elem* pos, const Elem& x, unsigned int n, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < n ? n : old);
        Elem* ns = len != 0 ? (Elem*)std::malloc(len * sizeof(Elem)) : 0;
        Elem* nf = ns;
        for (Elem* p = s; p != pos; ++p, ++nf)
            new (nf) Elem(*p);
        if (n == 1) {
            new (nf) Elem(x);
            ++nf;
        }
        else {
            for (unsigned int i = 0; i < n; ++i, ++nf)
                new (nf) Elem(x);
        }
        if (!atend)
            for (Elem* p = pos; p != f; ++p, ++nf)
                new (nf) Elem(*p);
        for (Elem* p = s; p != f; ++p)
            p->~Elem();
        if (s)
            std::free(s);
        s = ns;
        f = nf;
        e = ns + len;
    }
    void destroy()
    {
        for (Elem* p = s; p != f; ++p)
            p->~Elem();
        if (s)
            std::free(s);
        s = f = e = 0;
    }
};

}

int main()
{
    using namespace model;
    int bad = 0;
    Vec v;
    v.s = v.f = v.e = 0;
    {
        Elem a = make(1), b = make(2), c = make(3);
        v.insert_overflow(v.f, a, 1, true);                 // empty -> capacity 1
        if (v.size() != 1 || v.capacity() != 1 || key(v.s[0]) != 1) bad = 1;
        v.insert_overflow(v.f, b, 1, true);                 // capacity 2
        if (v.size() != 2 || v.capacity() != 2 || key(v.s[1]) != 2) bad = 2;
        v.insert_overflow(v.s + 1, c, 3, false);            // 3 copies in the middle: capacity 2 + max(2, 3)
        if (v.size() != 5 || v.capacity() != 5) bad = 3;
        long want[5] = { 1, 3, 3, 3, 2 };
        for (int i = 0; i < 5; ++i)
            if (key(v.s[i]) != want[i]) bad = 4;
        if (!check(v.s, v.size(), a, b, c)) bad = 5;
        v.destroy();
    }
    if (!final_check()) bad = 6;
    if (bad) { std::printf("0075f860_TEST FAIL\n"); return 1; }
    std::printf("0075f860_TEST PASS\n");
    return 0;
}
// Behaviour test for 006f40f0 (std::__uninitialized_fill_n<CCombatRing*, unsigned int, CCombatRing>: n deep copies of one ring).
#include <cstdio>
#include <cstdlib>
#include <new>

struct __false_type { __false_type() {} };
template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};

class CProng {                                  // opaque 0x1c-byte record (copied bitwise by the stand-in below)
public:
    long Words[7];
};

template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector<T>& x)                  // stand-in for vector<CProng>::vector(const vector&) 0x006F20A0
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = allocator<T>().allocate(n);
        for (unsigned int i = 0; i < n; ++i)
            _M_start[i] = x._M_start[i];
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
};

class CCombatRing {
public:
    float RotationAngle;
    float DeltaRotation;
    long Index;
    float RingThickness;
    vector<CProng> Prongs;
};

static void MakeRing(CCombatRing& ring, long index, int prongs)
{
    new (&ring) CCombatRing;
    ring.RotationAngle = index * 0.5f;
    ring.DeltaRotation = -index * 0.25f;
    ring.Index = index;
    ring.RingThickness = 2.0f + index;
    ring.Prongs._M_start = (CProng*)std::malloc((prongs ? prongs : 1) * sizeof(CProng));
    for (int i = 0; i < prongs; ++i)
        for (int w = 0; w < 7; ++w)
            ring.Prongs._M_start[i].Words[w] = index * 100 + i * 10 + w;
    ring.Prongs._M_finish = ring.Prongs._M_start + prongs;
    ring.Prongs._M_end_of_storage = ring.Prongs._M_finish;
}

static int SameRing(const CCombatRing& a, const CCombatRing& b)
{
    if (a.RotationAngle != b.RotationAngle || a.DeltaRotation != b.DeltaRotation || a.Index != b.Index ||
        a.RingThickness != b.RingThickness)
        return 0;
    long n = (long)(a.Prongs._M_finish - a.Prongs._M_start);
    if (n != (long)(b.Prongs._M_finish - b.Prongs._M_start))
        return 0;
    if (n && a.Prongs._M_start == b.Prongs._M_start)   // must be a deep copy
        return 0;
    for (long i = 0; i < n; ++i)
        for (int w = 0; w < 7; ++w)
            if (a.Prongs._M_start[i].Words[w] != b.Prongs._M_start[i].Words[w])
                return 0;
    return 1;
}

template<class ForwardIter, class Size, class T>
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

int main()
{
    CCombatRing ring;
    MakeRing(ring, 7, 2);
    CCombatRing* raw = (CCombatRing*)std::malloc(4 * sizeof(CCombatRing));
    CCombatRing* end = __uninitialized_fill_n(raw, 4u, ring, __false_type());
    int ok = end == raw + 4;
    for (int i = 0; i < 4; ++i)
        if (!SameRing(ring, raw[i]))
            ok = 0;
    if (raw[0].Prongs._M_start == raw[1].Prongs._M_start)
        ok = 0;
    if (__uninitialized_fill_n(raw, 0u, ring, __false_type()) != raw)
        ok = 0;
    if (!ok) {
        std::printf("006f40f0_TEST FAIL\n");
        return 1;
    }
    std::printf("006f40f0_TEST PASS\n");
    return 0;
}
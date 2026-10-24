// Behaviour test for 006f4930 (STLport vector<CCombatRing>::push_back: construct at finish when there is room, else grow through _M_insert_overflow).
#include <cstdio>
#include <cstdlib>
#include <new>
#include <cstring>

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

class CRingVector {
public:
    CCombatRing* _M_start;
    CCombatRing* _M_finish;
    CCombatRing* _M_end_of_storage;
    int Overflows;
    void _M_insert_overflow(CCombatRing* position, const CCombatRing& x, const __false_type&, unsigned int fill_len, bool atend)
    {   // the real grow (0x006F4750) reduced to the push_back case: double, copy, construct at the end
        ++Overflows;
        if (!atend || fill_len != 1 || position != _M_finish)
            return;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = size ? 2 * size : 1;
        CCombatRing* fresh = (CCombatRing*)std::malloc(len * sizeof(CCombatRing));
        for (unsigned int i = 0; i < size; ++i)
            std::memcpy((void*)(fresh + i), (const void*)(_M_start + i), sizeof(CCombatRing));
        _Construct(fresh + size, x);
        std::free(_M_start);
        _M_start = fresh;
        _M_finish = fresh + size + 1;
        _M_end_of_storage = fresh + len;
    }
    void push_back(const CCombatRing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

int main()
{
    CCombatRing rings[3];
    MakeRing(rings[0], 4, 2);
    MakeRing(rings[1], 5, 1);
    MakeRing(rings[2], 6, 3);
    CRingVector v;
    v._M_start = (CCombatRing*)std::malloc(2 * sizeof(CCombatRing));
    v._M_finish = v._M_start;
    v._M_end_of_storage = v._M_start + 2;
    v.Overflows = 0;
    v.push_back(rings[0]);
    v.push_back(rings[1]);
    int ok = v.Overflows == 0 && v._M_finish == v._M_start + 2;
    v.push_back(rings[2]);
    if (v.Overflows != 1 || v._M_finish - v._M_start != 3)
        ok = 0;
    for (int i = 0; ok && i < 3; ++i)
        if (!SameRing(rings[i], v._M_start[i]))
            ok = 0;
    if (!ok) {
        std::printf("006f4930_TEST FAIL\n");
        return 1;
    }
    std::printf("006f4930_TEST PASS\n");
    return 0;
}
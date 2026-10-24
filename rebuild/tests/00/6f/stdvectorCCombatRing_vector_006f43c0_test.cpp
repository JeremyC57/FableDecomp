// Behaviour test for 006f43c0 (STLport vector<CCombatRing>::vector(const vector&): allocate size() rings and copy-construct them).
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

template<class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template<class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = allocator<T>(a).allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};

class CRingVector : public _Vector_base<CCombatRing> {
public:
    CRingVector(CCombatRing* s, CCombatRing* f) : _Vector_base<CCombatRing>(0, allocator<CCombatRing>())
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = f;
    }
    CRingVector(const CRingVector& x) : _Vector_base<CCombatRing>(x.size(), x.get_allocator())
    {
        _M_finish = __uninitialized_copy((const CCombatRing*)x._M_start, (const CCombatRing*)x._M_finish, _M_start, __false_type());
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    allocator<CCombatRing> get_allocator() const { return allocator<CCombatRing>(); }
};

int main()
{
    CCombatRing rings[3];
    MakeRing(rings[0], 1, 1);
    MakeRing(rings[1], 2, 3);
    MakeRing(rings[2], 3, 0);
    CRingVector original(rings, rings + 3);
    CRingVector copy(original);
    int ok = copy.size() == 3 && copy._M_end_of_storage == copy._M_start + 3 && copy._M_start != original._M_start;
    for (int i = 0; ok && i < 3; ++i)
        if (!SameRing(rings[i], copy._M_start[i]))
            ok = 0;
    CRingVector empty(rings, rings);
    CRingVector emptyCopy(empty);
    if (emptyCopy._M_start != 0 || emptyCopy._M_finish != 0 || emptyCopy._M_end_of_storage != 0)
        ok = 0;
    if (!ok) {
        std::printf("006f43c0_TEST FAIL\n");
        return 1;
    }
    std::printf("006f43c0_TEST PASS\n");
    return 0;
}
// Behaviour test for _STL::_Destroy(ParticleAndEndPoints*, ParticleAndEndPoints*) (0x0076AE90): each 20-byte record in
// the range is destroyed in order, its end-point name before its start-point name; an empty range does nothing.
#include <cstdio>
#include <new>

static int gLog[32];
static int gCount = 0;

class CCharString {
public:
    int Id;
    explicit CCharString(int id) : Id(id) { }
    ~CCharString() { gLog[gCount++] = Id; }
};

struct ParticleAndEndPoints {
    CCharString StartPoint;
    CCharString EndPoint;
    long        ParticleData[2];
    long        PeriodicFrames;
    explicit ParticleAndEndPoints(int b) : StartPoint(b), EndPoint(b + 1), PeriodicFrames(b) { }
};

namespace _STL {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIterator>
void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}
}

int main()
{
    static double storage[32];
    ParticleAndEndPoints* p = (ParticleAndEndPoints*)storage;
    for (int i = 0; i < 3; ++i)
        new (&p[i]) ParticleAndEndPoints(10 * (i + 1));
    _STL::_Destroy(p, p);
    int fails = 0;
    if (gCount != 0) ++fails;
    _STL::_Destroy(p, p + 3);
    static const int expect[6] = { 11, 10, 21, 20, 31, 30 };
    if (gCount != 6) ++fails;
    for (int i = 0; i < 6 && i < gCount; ++i)
        if (gLog[i] != expect[i]) ++fails;
    if (fails) { std::printf("0076ae90_TEST FAIL\n"); return 1; }
    std::printf("0076ae90_TEST PASS\n");
    return 0;
}
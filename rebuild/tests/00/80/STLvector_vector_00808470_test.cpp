// Behaviour test for 0x00808470 STLport vector<CTargetPoint> copy constructor: capacity == size == x.size(), elements copied.
#include <cstdio>
#include <cstdlib>
#include <new>
struct CThing { int id; };
struct CTargetPoint { CThing* Thing; long Index; float X, Y, Z; long FrameRejected; };
struct CVector {
    CTargetPoint* _M_start; CTargetPoint* _M_finish; CTargetPoint* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CVector(const CVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n != 0 ? (CTargetPoint*)std::malloc(n * sizeof(CTargetPoint)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        CTargetPoint* cur = _M_start;
        for (const CTargetPoint* p = x._M_start; p != x._M_finish; ++p, ++cur) new (cur) CTargetPoint(*p);
        _M_finish = cur;
    }
};
int main()
{
    CThing things[3] = { { 1 }, { 2 }, { 3 } };
    CTargetPoint src[3];
    for (int i = 0; i < 3; ++i) { src[i].Thing = &things[i]; src[i].Index = 5 + i; src[i].X = (float)i; src[i].Y = 1.0f; src[i].Z = 2.0f; src[i].FrameRejected = -i; }
    CVector a; a._M_start = src; a._M_finish = src + 3; a._M_end_of_storage = src + 3;
    CVector b(a);
    bool ok = b._M_start != 0 && b._M_start != a._M_start && b._M_finish - b._M_start == 3 && b._M_end_of_storage == b._M_finish;
    for (int i = 0; ok && i < 3; ++i)
        ok = b._M_start[i].Thing == &things[i] && b._M_start[i].Index == 5 + i && b._M_start[i].X == (float)i && b._M_start[i].FrameRejected == -i;
    std::free(b._M_start);
    CVector e;
    CVector c(e);                                          // empty source: no allocation
    ok = ok && c._M_start == 0 && c._M_finish == 0 && c._M_end_of_storage == 0;
    if (!ok) { std::printf("00808470_TEST FAIL\n"); return 1; }
    std::printf("00808470_TEST PASS\n");
    return 0;
}
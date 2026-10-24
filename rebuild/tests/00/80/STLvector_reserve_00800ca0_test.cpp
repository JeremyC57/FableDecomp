// Behaviour test for 0x00800CA0 vector<pair<C2DVector, unsigned char> >::reserve: grow only when capacity < n, keep size and elements.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#pragma pack(push, 1)
struct SPoint { float X, Y; unsigned char Type; };
#pragma pack(pop)
struct CRoute {
    SPoint* _M_start; SPoint* _M_finish; SPoint* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    void reserve(unsigned long n)
    {
        if (capacity() < n) {
            unsigned long old = size();
            SPoint* tmp;
            if (_M_start) {
                tmp = (SPoint*)std::malloc(n * sizeof(SPoint));
                for (unsigned long i = 0; i < old; ++i) tmp[i] = _M_start[i];
                std::free(_M_start);
            } else
                tmp = n != 0 ? (SPoint*)std::malloc(n * sizeof(SPoint)) : 0;
            _M_start = tmp; _M_finish = tmp + old; _M_end_of_storage = tmp + n;
        }
    }
};
int main()
{
    bool ok = sizeof(SPoint) == 9;
    CRoute r = { 0, 0, 0 };
    r.reserve(0);                                            // empty and n == 0: untouched
    ok = ok && r._M_start == 0 && r.capacity() == 0;
    r.reserve(2);                                            // empty vector: plain allocate
    ok = ok && r._M_start != 0 && r.capacity() == 2 && r.size() == 0;
    r._M_finish->X = 1.0f; r._M_finish->Y = 2.0f; r._M_finish->Type = 3; ++r._M_finish;
    r._M_finish->X = 4.0f; r._M_finish->Y = 5.0f; r._M_finish->Type = 6; ++r._M_finish;
    SPoint* before = r._M_start;
    r.reserve(2);                                            // already enough: no reallocation
    ok = ok && r._M_start == before && r.capacity() == 2;
    r.reserve(7);                                            // grow: copy the two points, size kept
    ok = ok && r.capacity() == 7 && r.size() == 2;
    ok = ok && r._M_start[0].X == 1.0f && r._M_start[0].Type == 3 && r._M_start[1].Y == 5.0f && r._M_start[1].Type == 6;
    std::free(r._M_start);
    if (!ok) { std::printf("00800ca0_TEST FAIL\n"); return 1; }
    std::printf("00800ca0_TEST PASS\n");
    return 0;
}
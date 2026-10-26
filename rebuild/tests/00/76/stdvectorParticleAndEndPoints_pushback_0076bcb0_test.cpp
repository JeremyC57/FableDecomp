// Behaviour test for 0x0076BCB0 vector<ParticleAndEndPoints>::push_back
#include <cstdio>
#include <cstdlib>
#include <new>

class CCharString { public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { }
};
struct __false_type { __false_type() { } };
struct ParticleAndEndPoints {
    CCharString ParticleName;
    CCharString HelperName;
    int StartPoint;
    int EndPoint;
    int PeriodicFrames;
};
struct CParticleVector {
    ParticleAndEndPoints* _M_start;
    ParticleAndEndPoints* _M_finish;
    ParticleAndEndPoints* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(ParticleAndEndPoints* pos, const ParticleAndEndPoints& x, const __false_type&, unsigned int, bool)
    {   // stand-in for 0x0076B850 (append, doubling)
        ++overflows;
        unsigned int oldSize = (unsigned int)(_M_finish - _M_start);
        unsigned int len = oldSize + (oldSize > 1 ? oldSize : 1);
        ParticleAndEndPoints* s = (ParticleAndEndPoints*)std::malloc(len * sizeof(ParticleAndEndPoints));
        ParticleAndEndPoints* f = s;
        for (ParticleAndEndPoints* p = _M_start; p != pos; ++p, ++f) new (f) ParticleAndEndPoints(*p);
        new (f) ParticleAndEndPoints(x); ++f;
        std::free(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
    void push_back(const ParticleAndEndPoints& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) ParticleAndEndPoints(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

int main()
{
    CParticleVector v = { 0, 0, 0, 0 };
    ParticleAndEndPoints a = { CCharString("spark"), CCharString("hilt"), 1, 2, 30 };
    ParticleAndEndPoints b = { CCharString("glow"), CCharString("tip"), 3, 4, 60 };
    v.push_back(a);
    v.push_back(b);
    v.push_back(a);
    if (v.overflows != 3 || v._M_finish - v._M_start != 3) { std::printf("0076bcb0_TEST FAIL\n"); return 1; }
    v.push_back(b);           // capacity 4: in place
    if (v.overflows != 3 || v._M_finish - v._M_start != 4) { std::printf("0076bcb0_TEST FAIL\n"); return 1; }
    if (v._M_start[1].ParticleName.PStringData != b.ParticleName.PStringData || v._M_start[3].PeriodicFrames != 60
        || v._M_start[2].HelperName.PStringData != a.HelperName.PStringData || v._M_start[0].EndPoint != 2) { std::printf("0076bcb0_TEST FAIL\n"); return 1; }
    std::printf("0076bcb0_TEST PASS\n");
    return 0;
}
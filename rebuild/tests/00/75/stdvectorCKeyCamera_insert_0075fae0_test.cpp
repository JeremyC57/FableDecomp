// Behaviour test for 0x0075FAE0 vector<CKeyCamera>::insert(position, x): inserting at the end, in the middle (tail
// shifted up, x assigned into position) and at the front all keep order and return the iterator at the insert index;
// a full vector takes the growth path.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CCharString {
public:
    char* PStringData;
    CCharString(const char* s) { PStringData = new char[std::strlen(s) + 1]; std::strcpy(PStringData, s); }
    CCharString(const CCharString& o) { PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); }
    const CCharString& operator=(const CCharString& o) { if (this != &o) { delete[] PStringData; PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); } return *this; }
    ~CCharString() { delete[] PStringData; }
};

struct CKeyCamera {
    float DeltaIndex;
    CCharString EventName;
    CKeyCamera(float d, const char* n) : DeltaIndex(d), EventName(n) { }
};

struct Vec {
    CKeyCamera* _M_start;
    CKeyCamera* _M_finish;
    CKeyCamera* _M_end_of_storage;
    int m_overflows;
    void _M_insert_overflow(CKeyCamera* pos, const CKeyCamera& x)
    {
        ++m_overflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start), cap = n ? 2 * n : 1;
        CKeyCamera* s = (CKeyCamera*)std::malloc(cap * sizeof(CKeyCamera)), *d = s;
        for (CKeyCamera* p = _M_start; p != pos; ++p, ++d) new (d) CKeyCamera(*p);
        new (d) CKeyCamera(x); ++d;
        for (CKeyCamera* p = pos; p != _M_finish; ++p, ++d) new (d) CKeyCamera(*p);
        for (CKeyCamera* p = _M_start; p != _M_finish; ++p) p->~CKeyCamera();
        std::free(_M_start);
        _M_start = s; _M_finish = d; _M_end_of_storage = s + cap;
    }
    CKeyCamera* insert(CKeyCamera* position, const CKeyCamera& x)
    {
        unsigned int n = (unsigned int)(position - _M_start);
        if (_M_finish != _M_end_of_storage) {
            if (position == _M_finish) { new (_M_finish) CKeyCamera(x); ++_M_finish; }
            else {
                new (_M_finish) CKeyCamera(*(_M_finish - 1));
                ++_M_finish;
                CKeyCamera x_copy = x;
                CKeyCamera* last = _M_finish - 2; CKeyCamera* result = _M_finish - 1;
                for (int k = (int)(last - position); k > 0; --k) *--result = *--last;
                *position = x_copy;
            }
        }
        else _M_insert_overflow(position, x);
        return _M_start + n;
    }
};

int main()
{
    Vec v = { 0, 0, 0, 0 };
    v._M_start = (CKeyCamera*)std::malloc(8 * sizeof(CKeyCamera)); v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 8;
    CKeyCamera* it = v.insert(v._M_finish, CKeyCamera(1.0f, "one"));
    int ok = it == v._M_start;
    it = v.insert(v._M_finish, CKeyCamera(3.0f, "three"));
    if (it != v._M_start + 1) ok = 0;
    it = v.insert(v._M_start + 1, CKeyCamera(2.0f, "two"));
    if (it != v._M_start + 1) ok = 0;
    it = v.insert(v._M_start, CKeyCamera(0.0f, "zero"));
    if (it != v._M_start || v._M_finish - v._M_start != 4) ok = 0;
    const char* want[4] = { "zero", "one", "two", "three" };
    for (int i = 0; i < 4; ++i) if (v._M_start[i].DeltaIndex != (float)i || std::strcmp(v._M_start[i].EventName.PStringData, want[i])) ok = 0;
    v._M_end_of_storage = v._M_finish;                        // full: growth path
    it = v.insert(v._M_start + 2, CKeyCamera(1.5f, "mid"));
    if (v.m_overflows != 1 || it != v._M_start + 2 || v._M_start[2].DeltaIndex != 1.5f || v._M_start[4].DeltaIndex != 3.0f) ok = 0;
    for (CKeyCamera* p = v._M_start; p != v._M_finish; ++p) p->~CKeyCamera();
    std::free(v._M_start);
    if (!ok) { std::printf("0075fae0_TEST FAIL\n"); return 1; }
    std::printf("0075fae0_TEST PASS\n");
    return 0;
}
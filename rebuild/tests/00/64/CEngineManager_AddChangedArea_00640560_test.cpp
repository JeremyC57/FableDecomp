#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test: CEngineManager::AddChangedArea -- STLport push_back of a C2DBoxI: in place while capacity remains, else
// _M_insert_overflow grows to max(2*size, size+1) and copies the old boxes and the new one.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
struct BoxVector {
    C2DBoxI* _M_start; C2DBoxI* _M_finish; C2DBoxI* _M_end_of_storage;
    void _M_insert_overflow(C2DBoxI* pos, const C2DBoxI& x, unsigned int n, bool atend)
    {
        unsigned int oldSize = _M_finish - _M_start;
        unsigned int len = oldSize + (oldSize < n ? n : oldSize);
        C2DBoxI* ns = len ? (C2DBoxI*)std::malloc(len * sizeof(C2DBoxI)) : 0;
        C2DBoxI* nf = ns;
        C2DBoxI* p;
        for (p = _M_start; p != pos; ++p) { *nf++ = *p; }
        for (; n != 0; --n) { *nf++ = x; }
        if (!atend)
            for (p = pos; p != _M_finish; ++p) { *nf++ = *p; }
        if (_M_start)
            std::free(_M_start);
        _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    void push_back(const C2DBoxI& x)
    {
        if (_M_finish != _M_end_of_storage) { *_M_finish = x; ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};
class CEngineManager { public:
    void* Engines;
    BoxVector ChangedAreas;
    void AddChangedArea(const C2DBoxI& area) { ChangedAreas.push_back(area); }
};
int main()
{
    CEngineManager m;
    m.Engines = 0; m.ChangedAreas._M_start = m.ChangedAreas._M_finish = m.ChangedAreas._M_end_of_storage = 0;
    for (long i = 0; i < 5; ++i) {
        C2DBoxI b = { i, i + 1, i + 2, i + 3 };
        m.AddChangedArea(b);
    }
    if (m.ChangedAreas._M_finish - m.ChangedAreas._M_start != 5) { std::printf("00640560_TEST FAIL\n"); return 1; }
    if (m.ChangedAreas._M_end_of_storage - m.ChangedAreas._M_start != 8) { std::printf("00640560_TEST FAIL\n"); return 1; }
    for (long k = 0; k < 5; ++k)
        if (m.ChangedAreas._M_start[k].TLX != k || m.ChangedAreas._M_start[k].BRY != k + 3) { std::printf("00640560_TEST FAIL\n"); return 1; }
    std::free(m.ChangedAreas._M_start);
    std::printf("00640560_TEST PASS\n");
    return 0;
}
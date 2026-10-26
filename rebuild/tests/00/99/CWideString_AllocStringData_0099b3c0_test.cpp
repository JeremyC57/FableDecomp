#include <cstdio>
#define FAIL { std::printf("0099b3c0_TEST FAIL\n"); return 1; }
struct forward_iterator_tag { forward_iterator_tag() { } };
struct CBasicStringW {
    unsigned short* _M_start; unsigned short* _M_finish; unsigned short* _M_end_of_storage;
    static unsigned long length(const unsigned short* s) { unsigned long i = 0; while (s[i] != 0) ++i; return i; }
    void _M_range_initialize(const unsigned short* first, const unsigned short* last, const forward_iterator_tag&)
    {
        unsigned long n = (unsigned long)(last - first);
        _M_start = new unsigned short[n + 1]; _M_end_of_storage = _M_start + n + 1;
        for (unsigned long i = 0; i < n; ++i) _M_start[i] = first[i];
        _M_finish = _M_start + n; *_M_finish = 0;
    }
    CBasicStringW(const unsigned short* s) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { _M_range_initialize(s, s + length(s), forward_iterator_tag()); }
    CBasicStringW(const unsigned short* s, unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { _M_range_initialize(s, s + n, forward_iterator_tag()); }
};
class CWideStringData { public:
    CBasicStringW Data; long NoRefs;
    CWideStringData(const unsigned short* t) : Data(t), NoRefs(1) { }
    CWideStringData(const unsigned short* t, unsigned long n) : Data(t, n), NoRefs(1) { }
};
class CWideString { public:
    CWideStringData* AllocStringData(const unsigned short* text, long length)
    {
        return length == -1 ? new CWideStringData(text) : new CWideStringData(text, length);
    }
};
int main()
{
    unsigned short text[] = { 'a', 'b', 'c', 'd', 0 };
    CWideString s;
    CWideStringData* a = s.AllocStringData(text, -1);
    if (!a || a->NoRefs != 1 || a->Data._M_finish - a->Data._M_start != 4 || a->Data._M_start[3] != 'd' || *a->Data._M_finish != 0) FAIL;
    CWideStringData* b = s.AllocStringData(text, 2);
    if (!b || b->NoRefs != 1 || b->Data._M_finish - b->Data._M_start != 2 || b->Data._M_start[1] != 'b' || *b->Data._M_finish != 0) FAIL;
    std::printf("0099b3c0_TEST PASS\n");
    return 0;
}
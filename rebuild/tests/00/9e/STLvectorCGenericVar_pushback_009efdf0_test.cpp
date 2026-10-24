// Behaviour test for 0x009EFDF0 vector<CGenericVar>::push_back: copies are appended in order, the overflow path grows
// the storage and keeps the existing elements.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CGenericVar
{
public:
    long Type;
    const char* ValString;
    long ValSLONG;
    CGenericVar(long t, const char* s, long v) : Type(t), ValString(s), ValSLONG(v) {}
    virtual ~CGenericVar() {}
};

struct FalseType { FalseType() {} };

template <class _Tp> class vector
{
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    int Overflows;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Overflows(0) {}
    void _M_insert_overflow(_Tp* __pos, const _Tp& __x, const FalseType&, unsigned int __fill_len, bool __atend)
    {
        ++Overflows;
        unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        _Tp* __new_start = (_Tp*)std::malloc(__len * sizeof(_Tp));
        _Tp* __new_finish = __new_start;
        for (_Tp* __p = _M_start; __p != __pos; ++__p, ++__new_finish) new (__new_finish) _Tp(*__p);
        for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish) new (__new_finish) _Tp(__x);
        if (!__atend)
            for (_Tp* __q = __pos; __q != _M_finish; ++__q, ++__new_finish) new (__new_finish) _Tp(*__q);
        for (_Tp* __d = _M_start; __d != _M_finish; ++__d) __d->~_Tp();
        std::free(_M_start);
        _M_start = __new_start; _M_finish = __new_finish; _M_end_of_storage = __new_start + __len;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) _Tp(__x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, __x, FalseType(), 1, true);
    }
};

int main()
{
    vector<CGenericVar> v;
    const char* names[5] = { "a", "b", "c", "d", "e" };
    for (long i = 0; i < 5; ++i) v.push_back(CGenericVar(i + 1, names[i], i * 10));
    bool ok = v._M_finish - v._M_start == 5 && v.Overflows == 4 && v._M_end_of_storage - v._M_start == 8;
    for (long i = 0; i < 5; ++i)
        ok = ok && v._M_start[i].Type == i + 1 && std::strcmp(v._M_start[i].ValString, names[i]) == 0
                && v._M_start[i].ValSLONG == i * 10;
    for (CGenericVar* p = v._M_start; p != v._M_finish; ++p) p->~CGenericVar();
    std::free(v._M_start);
    if (!ok) { std::printf("009efdf0_TEST FAIL\n"); return 1; }
    std::printf("009efdf0_TEST PASS\n");
    return 0;
}
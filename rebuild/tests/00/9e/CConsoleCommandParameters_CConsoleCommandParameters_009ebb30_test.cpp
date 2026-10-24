// Behaviour test for 0x009EBB30 CConsoleCommandParameters(type): an empty vector grown through push_back holds the
// one type.
#include <cstdio>
#include <cstdlib>
#include <new>

enum EGenericVarType { GVT_NONE, GVT_SLONG, GVT_ULONG, GVT_FLOAT, GVT_BOOL, GVT_STRING };

struct TrueType { TrueType() {} };

template <class _Tp> class vector
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) std::free(_M_start); }
    void _M_insert_overflow(_Tp* __pos, const _Tp& __x, const TrueType&, unsigned int __fill_len, bool __atend)
    {
        unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
        unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        _Tp* __new_start = (_Tp*)std::malloc(__len * sizeof(_Tp));
        _Tp* __new_finish = __new_start;
        for (_Tp* __p = _M_start; __p != __pos; ++__p, ++__new_finish) new (__new_finish) _Tp(*__p);
        for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish) new (__new_finish) _Tp(__x);
        if (!__atend)
            for (_Tp* __q = __pos; __q != _M_finish; ++__q, ++__new_finish) new (__new_finish) _Tp(*__q);
        if (_M_start) std::free(_M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) _Tp(__x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, __x, TrueType(), 1, true);
    }
};

class CConsoleCommandParameters
{
public:
    vector<EGenericVarType> ParamTypes;
    CConsoleCommandParameters(EGenericVarType type)
    {
        AddParameterType(type);
    }
    void AddParameterType(const EGenericVarType& type) { ParamTypes.push_back(type); }
    long GetSize() const { return (long)(ParamTypes._M_finish - ParamTypes._M_start); }
};

int main()
{
    CConsoleCommandParameters p(GVT_BOOL);
    bool ok = p.GetSize() == 1 && p.ParamTypes._M_start[0] == GVT_BOOL
           && p.ParamTypes._M_end_of_storage - p.ParamTypes._M_start == 1;
    p.AddParameterType(GVT_ULONG);
    ok = ok && p.GetSize() == 2 && p.ParamTypes._M_start[0] == GVT_BOOL && p.ParamTypes._M_start[1] == GVT_ULONG;
    if (!ok) { std::printf("009ebb30_TEST FAIL\n"); return 1; }
    std::printf("009ebb30_TEST PASS\n");
    return 0;
}
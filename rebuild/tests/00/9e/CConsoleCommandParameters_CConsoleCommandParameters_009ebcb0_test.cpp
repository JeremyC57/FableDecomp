// Behaviour test for 0x009EBCB0 CConsoleCommandParameters(type1, type2, type3, type4): an empty vector grown through
// push_back holds the four types in order.
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
    CConsoleCommandParameters(EGenericVarType type1, EGenericVarType type2, EGenericVarType type3, EGenericVarType type4)
    {
        AddParameterType(type1);
        AddParameterType(type2);
        AddParameterType(type3);
        AddParameterType(type4);
    }
    void AddParameterType(const EGenericVarType& type) { ParamTypes.push_back(type); }
    long GetSize() const { return (long)(ParamTypes._M_finish - ParamTypes._M_start); }
};

int main()
{
    CConsoleCommandParameters p(GVT_STRING, GVT_FLOAT, GVT_BOOL, GVT_SLONG);
    bool ok = p.GetSize() == 4 && p.ParamTypes._M_start[0] == GVT_STRING && p.ParamTypes._M_start[1] == GVT_FLOAT
           && p.ParamTypes._M_start[2] == GVT_BOOL && p.ParamTypes._M_start[3] == GVT_SLONG
           && p.ParamTypes._M_end_of_storage - p.ParamTypes._M_start == 4;           // capacity 1, 2, 4
    if (!ok) { std::printf("009ebcb0_TEST FAIL\n"); return 1; }
    std::printf("009ebcb0_TEST PASS\n");
    return 0;
}
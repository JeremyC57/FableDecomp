// Behaviour test for 0x0067F220 _STL::vector<CDegradableInfo>::resize(unsigned int new_size, const CDegradableInfo& x).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0067f220_TEST FAIL\n"); return 1; } } while (0)

struct CDegradableInfo {
    float         HealthPercentage;
    unsigned char EngineGraphic[0x10];
    long          SmashParticleEmitter;
    bool          BlocksNavigation;
};

#include <new>
template <class _Tp>
class vector_model {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector_model(unsigned int capacity)
    {
        _M_start = (_Tp*)::operator new(sizeof(_Tp) * capacity);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + capacity;
    }
    ~vector_model() { _Destroy(_M_start, _M_finish); ::operator delete(_M_start); }
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) __first->~_Tp(); }
    static _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result)
    {
        int __n;
        for (__n = (int)(__last - __first); __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
        return __result;
    }
    void push_back(const _Tp& __x) { new (_M_finish) _Tp(__x); ++_M_finish; }
    // STLport erase(first, last)
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy(__last, _M_finish, __first);
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    // STLport erase(position)
    _Tp* erase(_Tp* __position)
    {
        if (__position + 1 != end())
            copy(__position + 1, _M_finish, __position);
        --_M_finish;
        _M_finish->~_Tp();
        return __position;
    }
    // _M_fill_insert at the end with enough capacity (what resize needs here)
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x)
    {
        if (__pos == _M_finish && _M_finish + __n <= _M_end_of_storage)
            for (; __n != 0; --__n) { new (_M_finish) _Tp(__x); ++_M_finish; }
    }
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    // STLport resize(n, x) and resize(n)
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

int main()
{
    int i;
    vector_model<CDegradableInfo> v(16);
    CDegradableInfo x;
    x.HealthPercentage = 0.5f; std::memset(x.EngineGraphic, 0xAB, sizeof(x.EngineGraphic)); x.SmashParticleEmitter = 12; x.BlocksNavigation = true;
    v.resize(5, x);
    CHECK(v.size() == 5);
    for (i = 0; i < 5; ++i) CHECK(v._M_start[i].SmashParticleEmitter == 12 && v._M_start[i].BlocksNavigation && v._M_start[i].EngineGraphic[3] == 0xAB);
    v._M_start[1].SmashParticleEmitter = 99;
    v.resize(2, x);
    CHECK(v.size() == 2 && v._M_start[1].SmashParticleEmitter == 99);
    x.SmashParticleEmitter = 3;
    v.resize(4, x);
    CHECK(v.size() == 4 && v._M_start[0].SmashParticleEmitter == 12 && v._M_start[2].SmashParticleEmitter == 3 && v._M_start[3].SmashParticleEmitter == 3);
    v.resize(4, x);
    CHECK(v.size() == 4);
    std::printf("0067f220_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00A55670 _STL::vector<C3DAnimationSequenceData::C3DVectorWord>::resize(unsigned int new_size, const C3DVectorWord& x).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00a55670_TEST FAIL\n"); return 1; } } while (0)
class C3DAnimationSequenceData {
public:
    class C3DVectorWord { public: short X, Y, Z; };
};
typedef C3DAnimationSequenceData::C3DVectorWord TElem;

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
    vector_model<TElem> v(16);
    TElem x;
    x.X = 1; x.Y = 2; x.Z = 3;
    v.resize(6, x);
    CHECK(v.size() == 6 && v._M_start[5].X == 1 && v._M_start[5].Y == 2 && v._M_start[5].Z == 3);
    v._M_start[2].Z = 77;
    v.resize(3, x);
    CHECK(v.size() == 3 && v._M_start[2].Z == 77);
    x.X = -5;
    v.resize(5, x);
    CHECK(v.size() == 5 && v._M_start[3].X == -5 && v._M_start[4].X == -5 && v._M_start[0].X == 1);
    v.resize(0, x);
    CHECK(v.size() == 0);
    std::printf("00a55670_TEST PASS\n");
    return 0;
}
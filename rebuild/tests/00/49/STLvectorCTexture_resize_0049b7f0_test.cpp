// Behaviour test for 0x0049B7F0 _STL::vector<CTexture>::resize(unsigned int new_size).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0049b7f0_TEST FAIL\n"); return 1; } } while (0)
static int g_elemCtors = 0, g_elemDtors = 0;

struct IDirect3DTexture9 { int RefCount; };
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;
    unsigned long      ByteLength : 28;
    unsigned long      AllocationSource : 4;
    CTexture() : PD3DTexture(0) { ByteLength = 0; AllocationSource = 0; ++g_elemCtors; }
    CTexture(const CTexture& o) : PD3DTexture(o.PD3DTexture) { ByteLength = o.ByteLength; AllocationSource = o.AllocationSource; }
    CTexture& operator=(const CTexture& o) { PD3DTexture = o.PD3DTexture; ByteLength = o.ByteLength; AllocationSource = o.AllocationSource; return *this; }
    ~CTexture() { PD3DTexture = 0; ++g_elemDtors; }
};
static IDirect3DTexture9 g_tex;

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
    {
        vector_model<CTexture> v(16);
        g_elemCtors = g_elemDtors = 0;
        v.resize(4);
        CHECK(v.size() == 4 && g_elemCtors == 1 && g_elemDtors == 1);
        for (i = 0; i < 4; ++i) CHECK(v._M_start[i].PD3DTexture == 0 && v._M_start[i].ByteLength == 0 && v._M_start[i].AllocationSource == 0);
        v._M_start[1].PD3DTexture = &g_tex; v._M_start[1].ByteLength = 0x123456; v._M_start[1].AllocationSource = 5;
        v.resize(2);
        CHECK(v.size() == 2 && g_elemCtors == 2 && g_elemDtors == 4);
        CHECK(v._M_start[1].PD3DTexture == &g_tex && v._M_start[1].ByteLength == 0x123456 && v._M_start[1].AllocationSource == 5);
        v.resize(5);
        CHECK(v.size() == 5 && g_elemCtors == 3 && g_elemDtors == 5);
        i = 4;
        CHECK(v._M_start[i].PD3DTexture == 0 && v._M_start[i].ByteLength == 0 && v._M_start[i].AllocationSource == 0);
        v.resize(5);
        CHECK(v.size() == 5 && g_elemCtors == 4 && g_elemDtors == 6);
    }
    CHECK(g_elemDtors == 11);
    std::printf("0049b7f0_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x008E3890 _STL::vector<CIcicleData>::erase(CIcicleData* position).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("008e3890_TEST FAIL\n"); return 1; } } while (0)

class CBaseObject { public: int Id; };
static int g_ptrSets = 0, g_ptrDtors = 0;
class CBaseIntelligentPointer {                         // test model: vptr + item
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_ptrDtors; PItem = 0; }
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* item) { PItem = item; ++g_ptrSets; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing;
struct C3DVector { float X, Y, Z; };
static int g_icicleDtors = 0;
class CIcicleData {
public:
    CIntelligentPointer<CThing> PEffect;
    CIntelligentPointer<CThing> PTrap;
    long                        EffectTimer;
    long                        TrapTimer;
    C3DVector                   Position;
    CIcicleData(CBaseObject* e, long t) : PEffect(e), PTrap(e), EffectTimer(t), TrapTimer(t * 2) { Position.X = (float)t; Position.Y = 0.0f; Position.Z = 0.0f; }
    ~CIcicleData() { ++g_icicleDtors; }
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
    CBaseObject effects[4];
    {
        vector_model<CIcicleData> v(8);
        for (i = 0; i < 4; ++i) { effects[i].Id = i; v.push_back(CIcicleData(&effects[i], 10 * i)); }
        g_icicleDtors = 0;
        CIcicleData* r = v.erase(v.begin() + 1);
        CHECK(r == v.begin() + 1 && v.size() == 3 && g_icicleDtors == 1);
        CHECK(v._M_start[0].EffectTimer == 0 && v._M_start[1].EffectTimer == 20 && v._M_start[2].EffectTimer == 30);
        CHECK(v._M_start[1].PEffect.GetPItem() == &effects[2] && v._M_start[2].PTrap.GetPItem() == &effects[3] && v._M_start[2].TrapTimer == 60);
        r = v.erase(v.begin() + 2);                     // the last element: no copy
        CHECK(r == v.end() && v.size() == 2 && g_icicleDtors == 2);
        CHECK(v._M_start[1].EffectTimer == 20);
    }
    CHECK(g_icicleDtors == 4);
    std::printf("008e3890_TEST PASS\n");
    return 0;
}
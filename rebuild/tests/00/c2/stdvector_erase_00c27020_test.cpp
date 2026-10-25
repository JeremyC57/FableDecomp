// behaviour test for 0x00C27020 std::vector<PKCStreamRequest*>::erase: close the gap, shrink by one, return the position
#include <cstdio>
#include <string.h>

struct PKCStreamRequest { int m_id; };

namespace pkstd {
template<class _Ty> class _Ptrit {
public:
    _Ptrit() { }
    explicit _Ptrit(_Ty* _Ptr) : current(_Ptr) { }
    _Ty& operator*() const { return (*current); }
    _Ty* base() const { return (current); }
    _Ty* current;
};

template<class _Ty> inline
_Ty* copy(_Ty* _First, _Ty* _Last, _Ty* _Dest)
{
    int _Off = _Last - _First;
    return ((_Ty*)memmove(_Dest, _First, _Off * sizeof(*_First)) + _Off);
}

template<class _Ty> inline
void _Destroy(_Ty*, _Ty*)
{
}

template<class _Ty> class vector {
public:
    typedef _Ptrit<_Ty> iterator;
    iterator begin() { return (iterator(_Myfirst)); }
    iterator erase(iterator _Where)
    {
        copy(_Where.base() + 1, _Mylast, _Where.base());
        _Destroy(_Mylast - 1, _Mylast);
        --_Mylast;
        return (_Where);
    }
    int  _Alval;
    _Ty* _Myfirst;
    _Ty* _Mylast;
    _Ty* _Myend;
};
}

int main()
{
    PKCStreamRequest a = { 1 }, b = { 2 }, c = { 3 }, d = { 4 };
    PKCStreamRequest* slots[4] = { &a, &b, &c, &d };
    pkstd::vector<PKCStreamRequest*> v; v._Myfirst = slots; v._Mylast = slots + 4; v._Myend = slots + 4;
    pkstd::vector<PKCStreamRequest*>::iterator it = v.erase(pkstd::vector<PKCStreamRequest*>::iterator(slots + 1));
    bool ok = it.base() == slots + 1 && v._Mylast == slots + 3 && slots[0] == &a && slots[1] == &c && slots[2] == &d;
    it = v.erase(pkstd::vector<PKCStreamRequest*>::iterator(slots + 2));      // the last element
    ok = ok && it.base() == slots + 2 && v._Mylast == slots + 2 && slots[1] == &c;
    it = v.erase(v.begin());
    ok = ok && v._Mylast == slots + 1 && slots[0] == &c;
    if (!ok) { std::printf("00c27020_TEST FAIL\n"); return 1; }
    std::printf("00c27020_TEST PASS\n");
    return 0;
}
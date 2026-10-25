// Behaviour test for 0x0051CAA0 _STL::fill(CNavigationAreaUpdate::CNavigationLayerUpdate*, ...): every navigation-layer update of the range becomes a copy of the value (line list then switchable blockages, each through its vector operator=)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
template <class T> class CVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector& operator=(const CVector& other)          // stand-in
    {
        _M_start = other._M_start; _M_finish = other._M_finish; _M_end_of_storage = other._M_end_of_storage;
        ++gAssigns;
        return *this;
    }
};
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Start, End; };
class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
        unsigned __int64 UniqueID;
        void*            LineList;
    };
    class CNavigationLayerUpdate {
    public:
        CVector<C2DLineF>            LineList;
        CVector<CSwitchableBlockage> SwitchableBlockages;
    };
};
typedef CNavigationAreaUpdate::CNavigationLayerUpdate CNavigationLayerUpdate;
static C2DLineF gLines[2];
static CNavigationAreaUpdate::CSwitchableBlockage gBlockages[1];
static CNavigationLayerUpdate gArr[4];
namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
int main()
{
    CNavigationLayerUpdate v;
    v.LineList._M_start = gLines; v.LineList._M_finish = gLines + 2; v.LineList._M_end_of_storage = gLines + 2;
    v.SwitchableBlockages._M_start = gBlockages; v.SwitchableBlockages._M_finish = gBlockages + 1; v.SwitchableBlockages._M_end_of_storage = gBlockages + 1;
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].LineList._M_start = 0; gArr[i].SwitchableBlockages._M_start = 0; }
    bool ok = true;
    _STL::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].LineList._M_start == gLines && gArr[i].LineList._M_finish == gLines + 2 &&
             gArr[i].SwitchableBlockages._M_start == gBlockages && gArr[i].SwitchableBlockages._M_finish == gBlockages + 1;
    ok = ok && gArr[3].LineList._M_start == 0 && gAssigns == 6;     // two vector assignments per element
    _STL::fill(gArr, gArr, v);
    ok = ok && gAssigns == 6;
    if (ok) { printf("0051caa0_TEST PASS\n"); return 0; }
    printf("0051caa0_TEST FAIL\n");
    return 1;
}
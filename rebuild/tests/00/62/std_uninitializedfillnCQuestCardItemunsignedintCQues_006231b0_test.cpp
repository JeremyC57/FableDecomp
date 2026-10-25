// Behaviour test for std::__uninitialized_fill_n<CQuestCardItem*, unsigned int, CQuestCardItem> (0x006231B0).
#include <stdio.h>
inline void* operator new(unsigned int, void* p) { return p; }
static int g_copies = 0;
class CWideString { public: void* PStringData; CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_copies; } };
class CQuestCardItem { public: long ItemIndex; CWideString ItemText; };
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}
int main()
{
    static long xb[2]; CQuestCardItem& x = *(CQuestCardItem*)xb; x.ItemIndex = 5; x.ItemText.PStringData = (void*)0x77;
    static long buf[8];
    CQuestCardItem* first = (CQuestCardItem*)buf;
    CQuestCardItem* e = std::__uninitialized_fill_n(first, 3u, x, std::__false_type());
    int ok = e == first + 3 && g_copies == 3 && first[2].ItemIndex == 5 && first[2].ItemText.PStringData == (void*)0x77
        && buf[6] == 0;
    CQuestCardItem* z = std::__uninitialized_fill_n(first, 0u, x, std::__false_type());
    ok = ok && z == first && g_copies == 3;
    printf(ok ? "006231b0_TEST PASS\n" : "006231b0_TEST FAIL\n");
    return ok ? 0 : 1;
}

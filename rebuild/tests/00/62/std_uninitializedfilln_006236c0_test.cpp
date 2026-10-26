// Behaviour test for 0x006236C0 std::uninitialized_fill_n(CQuestCardItem* first, unsigned int n, const CQuestCardItem& x):
// placement-construct n copies of x (the index, then the wide-string copy 0x0099B720, modelled as a counting copy) and
// return the end of the filled range.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_stringCopies = 0;
class CWideString { public:
    const unsigned short* PStringData;
    explicit CWideString(const unsigned short* s) : PStringData(s) { }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_stringCopies; }
};

class CQuestCardItem { public:
    long        ItemIndex;
    CWideString ItemText;
    CQuestCardItem(long i, const unsigned short* t) : ItemIndex(i), ItemText(t) { }
};

namespace stl {
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}
}

int main()
{
    int ok = 1;
    static const unsigned short text[] = { 'Q', 'U', 'E', 'S', 'T', 0 };
    CQuestCardItem x(42, text);
    CQuestCardItem* buf = (CQuestCardItem*)std::malloc(4 * sizeof(CQuestCardItem));
    CQuestCardItem* end = stl::uninitialized_fill_n(buf, 3u, x);
    if (end != buf + 3 || g_stringCopies != 3) ok = 0;
    for (int i = 0; i < 3; ++i)
        if (buf[i].ItemIndex != 42 || buf[i].ItemText.PStringData != text) ok = 0;
    if (stl::uninitialized_fill_n(buf + 3, 0u, x) != buf + 3 || g_stringCopies != 3) ok = 0;
    std::free(buf);
    if (!ok) { std::printf("006236c0_TEST FAIL\n"); return 1; }
    std::printf("006236c0_TEST PASS\n");
    return 0;
}
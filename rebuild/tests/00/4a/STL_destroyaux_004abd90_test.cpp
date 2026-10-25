// Behaviour test for std::__destroy_aux(CTestQuest*, CTestQuest*, const __false_type&) (0x004ABD90): every record in
// [first, last) is destroyed in order, each record's six strings last member first; nothing outside the range is touched.
#include <cstdio>
#include <new>

static int gLog[64];
static int gCount = 0;

class CCharString {
public:
    int Id;
    explicit CCharString(int id) : Id(id) { }
    ~CCharString() { gLog[gCount++] = Id; }
};

class CTestQuest {
public:
    CCharString QuestName;
    CCharString HSPName;
    long        ColourIndex;
    CCharString DisplayName;
    CCharString IniFile;
    CCharString OutiFile;
    CCharString QuestCardDefName;
    explicit CTestQuest(int b)
        : QuestName(b + 0), HSPName(b + 1), ColourIndex(b), DisplayName(b + 2), IniFile(b + 3), OutiFile(b + 4),
          QuestCardDefName(b + 5) { }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

int main()
{
    static double storage[64];
    CTestQuest* q = (CTestQuest*)storage;
    for (int i = 0; i < 3; ++i)
        new (&q[i]) CTestQuest(10 * (i + 1));
    std::__destroy_aux(q, q + 2, std::__false_type());
    int fails = 0;
    if (gCount != 12) ++fails;
    static const int expect[12] = { 15, 14, 13, 12, 11, 10, 25, 24, 23, 22, 21, 20 };
    for (int i = 0; i < 12 && i < gCount; ++i)
        if (gLog[i] != expect[i]) ++fails;
    std::__destroy_aux(q + 2, q + 2, std::__false_type());       // empty range
    if (gCount != 12) ++fails;
    if (q[2].ColourIndex != 30) ++fails;
    if (fails) { std::printf("004abd90_TEST FAIL\n"); return 1; }
    std::printf("004abd90_TEST PASS\n");
    return 0;
}
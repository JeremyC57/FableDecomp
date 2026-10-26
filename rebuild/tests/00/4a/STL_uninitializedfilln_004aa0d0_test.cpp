#include <cstdio>
#include <new>

// Behaviour model of __uninitialized_fill_n<CTestQuest*> (0x004AA0D0): copy-construct x into n slots, return the end.
static int g_copies = 0;
class CCharString { public:
    const char* p;
    CCharString(const char* s) : p(s) { }
    CCharString(const CCharString& o) : p(o.p) { ++g_copies; }
};
struct __false_type { __false_type() { } };
class CTestQuest { public:
    CCharString QuestName, HSPName;
    long        ColourIndex;
    CCharString DisplayName, IniFile, OutiFile, QuestCardDefName;
    CTestQuest(const char* n) : QuestName(n), HSPName("hsp"), ColourIndex(7), DisplayName("d"), IniFile("i"), OutiFile("o"), QuestCardDefName("c") { }
};
inline void _Construct(CTestQuest* p, const CTestQuest& v) { new (p) CTestQuest(v); }
CTestQuest* __uninitialized_fill_n(CTestQuest* first, unsigned int n, const CTestQuest& x, const __false_type&)
{
    CTestQuest* cur = first;
    for ( ; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

static unsigned char g_buf[sizeof(CTestQuest) * 4];

int main()
{
    CTestQuest x("Q_Arena");
    CTestQuest* first = (CTestQuest*)g_buf;
    CTestQuest* end = __uninitialized_fill_n(first, 3u, x, __false_type());
    if (end != first + 3 || g_copies != 3 * 6) { std::printf("004aa0d0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (first[i].QuestName.p != x.QuestName.p || first[i].ColourIndex != 7 || first[i].QuestCardDefName.p != x.QuestCardDefName.p) { std::printf("004aa0d0_TEST FAIL\n"); return 1; }
    if (__uninitialized_fill_n(first + 3, 0u, x, __false_type()) != first + 3 || g_copies != 18) { std::printf("004aa0d0_TEST FAIL\n"); return 1; }
    std::printf("004aa0d0_TEST PASS\n");
    return 0;
}
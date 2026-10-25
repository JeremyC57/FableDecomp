// Behaviour test for std::__destroy_aux<CHeroLogBook::CHeroLogBookEntry*> (0x0047BAF0): every entry in [first, last)
// destroys its three CWideString members in reverse order (Content, AbbreviatedName, Name); an empty range does nothing.
#include <stdio.h>

static int g_dtorCalls;
static int g_order[16];
static int g_orderIdx;

class CWideString {
public:
    ~CWideString();
    int Tag;
};
CWideString::~CWideString()
{
    ++g_dtorCalls;
    if (g_orderIdx < 16) g_order[g_orderIdx++] = Tag;
}

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        CWideString Name;
        CWideString AbbreviatedName;
        CWideString Content;
    };
};

namespace std {
struct __false_type {};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
}

int main()
{
    typedef CHeroLogBook::CHeroLogBookEntry Entry;
    const int N = 3;
    static char buf[sizeof(Entry) * N];
    Entry* first = (Entry*)buf;
    for (int i = 0; i < N; ++i) {
        first[i].Name.Tag = i * 3 + 0;
        first[i].AbbreviatedName.Tag = i * 3 + 1;
        first[i].Content.Tag = i * 3 + 2;
    }
    int ok = sizeof(Entry) == 12;
    std::__false_type tag;
    g_dtorCalls = 0; g_orderIdx = 0;
    std::__destroy_aux(first, first + N, tag);
    if (g_dtorCalls != N * 3) ok = 0;
    for (int i = 0; i < N && ok; ++i)
        if (g_order[i * 3] != i * 3 + 2 || g_order[i * 3 + 1] != i * 3 + 1 || g_order[i * 3 + 2] != i * 3) ok = 0;
    g_dtorCalls = 0;
    std::__destroy_aux(first, first, tag);
    if (g_dtorCalls != 0) ok = 0;
    printf(ok ? "0047baf0_TEST PASS\n" : "0047baf0_TEST FAIL\n");
    return ok ? 0 : 1;
}

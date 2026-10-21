#include <cstdio>
#include <new>
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);               // 0x0099B720
};
class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        CWideString Name;                                // +0x00
        CWideString AbbreviatedName;                     // +0x04
        CWideString Content;                             // +0x08
    };
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template CHeroLogBook::CHeroLogBookEntry* __fastcall __uninitialized_fill_n<CHeroLogBook::CHeroLogBookEntry*, unsigned int, CHeroLogBook::CHeroLogBookEntry>(CHeroLogBook::CHeroLogBookEntry*, unsigned int, const CHeroLogBook::CHeroLogBookEntry&, const __false_type&);
}


static int g_copies = 0;
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_copies; }
typedef CHeroLogBook::CHeroLogBookEntry Entry;
int main()
{
    static unsigned long raw[3] = { 7, 8, 9 };
    static unsigned long out[12];
    Entry* d = (Entry*)out;
    Entry* e = std::__uninitialized_fill_n(d, 3u, *(Entry*)raw, std::__false_type());
    bool ok = e == d + 3 && g_copies == 9 && out[0] == 7 && out[4] == 8 && out[8] == 9 && out[9] == 0;
    ok = ok && std::__uninitialized_fill_n(d, 0u, *(Entry*)raw, std::__false_type()) == d && g_copies == 9;
    if (ok) std::printf("0047b6c0_TEST PASS\n");
    return 0;
}
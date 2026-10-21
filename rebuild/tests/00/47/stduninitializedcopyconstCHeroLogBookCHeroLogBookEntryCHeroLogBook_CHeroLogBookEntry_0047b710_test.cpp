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
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CHeroLogBook::CHeroLogBookEntry* __fastcall __uninitialized_copy<const CHeroLogBook::CHeroLogBookEntry*, CHeroLogBook::CHeroLogBookEntry*>(const CHeroLogBook::CHeroLogBookEntry*, const CHeroLogBook::CHeroLogBookEntry*, CHeroLogBook::CHeroLogBookEntry*, const __false_type&);
}


static int g_copies = 0;
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_copies; }
typedef CHeroLogBook::CHeroLogBookEntry Entry;
int main()
{
    static unsigned long raw[6] = { 1, 2, 3, 4, 5, 6 };
    static unsigned long out[9];
    const Entry* s = (const Entry*)raw;
    Entry* d = (Entry*)out;
    Entry* e = std::__uninitialized_copy(s, s + 2, d, std::__false_type());
    bool ok = e == d + 2 && g_copies == 6 && out[0] == 1 && out[4] == 5 && out[5] == 6 && out[6] == 0;
    ok = ok && std::__uninitialized_copy(s, s, d, std::__false_type()) == d && g_copies == 6;
    if (ok) std::printf("0047b710_TEST PASS\n");
    return 0;
}
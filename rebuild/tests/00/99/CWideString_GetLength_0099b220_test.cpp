// Link test for 0x0099B220: the source object's CWideString::GetLength counts wide characters (0 for no data).
#include <cstdio>
namespace std {
template <class _CharT> struct char_traits { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string {
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
};
}
class CWideStringData {
public:
    std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > Data;
    long NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
    long GetLength() const;                        // from the source object
};
int main()
{
    static wchar_t text[] = L"hello";
    CWideStringData d;
    d.Data._M_start = text; d.Data._M_finish = text + 5; d.Data._M_end_of_storage = text + 6; d.NoRefs = 1;
    CWideString s, n, e;
    s.PStringData = &d;
    n.PStringData = 0;
    CWideStringData de;
    de.Data._M_start = de.Data._M_finish = de.Data._M_end_of_storage = text; de.NoRefs = 1;
    e.PStringData = &de;
    int ok = s.GetLength() == 5 && n.GetLength() == 0 && e.GetLength() == 0 && sizeof(CWideStringData) == 16;
    if (!ok) { std::printf("0099b220_TEST FAIL\n"); return 1; }
    std::printf("0099b220_TEST PASS\n");
    return 0;
}

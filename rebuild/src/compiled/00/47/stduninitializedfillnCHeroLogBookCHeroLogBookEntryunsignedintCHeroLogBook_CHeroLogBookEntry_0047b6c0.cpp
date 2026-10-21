// std::__uninitialized_fill_n<CHeroLogBook::CHeroLogBookEntry*, unsigned int, CHeroLogBook::CHeroLogBookEntry>(CHeroLogBook::CHeroLogBookEntry* first, unsigned int n, const CHeroLogBook::CHeroLogBookEntry& x, const __false_type&) -- copy-construct n log entries into raw storage, return the end
// Leaf: ??$__uninitialized_fill_n
// Retail: Fable.exe @ 0x0047B6C0, 69 bytes, __fastcall (first ecx, n edx, x + the __false_type tag on the stack, `ret 8`),
// returns the end in eax; one caller 0x0047C1EA inside the vector _M_insert_overflow 0x0047C140 (STLport).
// Catalog label std::_Fill_n<unsigned long*, unsigned int, unsigned long> is wrong: the 12-byte
// element is CHeroLogBook::CHeroLogBookEntry (Ego_r.pdb, 12 bytes: CWideString Name +0, AbbreviatedName +4, Content +8),
// built by its implicit copy constructor (three CWideString copies, 0x0099B720) behind a placement-new null test; the
// vector lives in CHeroLogBook::LogEntries (map<ECategory, vector<CHeroLogBookEntry>>).
// Shape: /O2; n is unsigned (`test edx,edx / jbe`), counted down with `dec / jne`.
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
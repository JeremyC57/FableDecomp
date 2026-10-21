// std::__uninitialized_copy<CHeroLogBook::CHeroLogBookEntry*, CHeroLogBook::CHeroLogBookEntry*>(CHeroLogBook::CHeroLogBookEntry* first, CHeroLogBook::CHeroLogBookEntry* last, CHeroLogBook::CHeroLogBookEntry* result, const __false_type&) -- copy-construct log entries into raw storage, return the end
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe @ 0x0047B670, 73 bytes, __fastcall (first ecx, last edx, result + the __false_type tag on the stack,
// `ret 8`), returns the end in eax; one caller 0x0047C199 inside the vector _M_insert_overflow 0x0047C140 (STLport).
// Catalog label std::_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int>>, unsigned int*> is wrong: the 12-byte
// element is CHeroLogBook::CHeroLogBookEntry (Ego_r.pdb, 12 bytes: CWideString Name +0, AbbreviatedName +4, Content +8),
// built by its implicit copy constructor (three CWideString copies, 0x0099B720) behind a placement-new null test; the
// vector lives in CHeroLogBook::LogEntries (map<ECategory, vector<CHeroLogBookEntry>>).
// 0x0047B670 and 0x0047B710 are byte-identical; the pointer/const-pointer split follows their callers (insert-overflow copy).
// Shape: /O2; first == last tested before the loop.
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
template CHeroLogBook::CHeroLogBookEntry* __fastcall __uninitialized_copy<CHeroLogBook::CHeroLogBookEntry*, CHeroLogBook::CHeroLogBookEntry*>(CHeroLogBook::CHeroLogBookEntry*, CHeroLogBook::CHeroLogBookEntry*, CHeroLogBook::CHeroLogBookEntry*, const __false_type&);
}
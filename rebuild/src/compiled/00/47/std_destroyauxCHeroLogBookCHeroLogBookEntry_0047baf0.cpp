// std::__destroy_aux<CHeroLogBook::CHeroLogBookEntry*>(first, last, const __false_type&) -- destroy each log-book entry
// in [first, last)
// Leaf: ??$__destroy_aux@PAVCHeroLogBookEntry@CHeroLogBook@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047BAF0, 51 bytes, `ret 4` (/Gr: ecx = first, edx = last, the __false_type tag on
// the stack).
// Identity: STLport's `_Destroy(first, last)` -> `__destroy_aux(first, last, __false_type)`. Each 12-byte element runs
// three calls to CWideString::~CWideString (0x0099B510; ~CCharString is the separate 0x0099EAE0), and Ego_r.pdb
// CHeroLogBook::CHeroLogBookEntry is exactly {CWideString Name +0, AbbreviatedName +4, Content +8} (12 bytes). Callers
// include CHeroLogBook AddLogEntry (0x0047ADEA, 0x0047AE05) and InsertEntry (0x0047CBE7, 0x0047CC02). The old label
// vector<pair<CCharString,CPersistInfo>>::_Destroy was wrong: that element holds CCharStrings and a vector member
// _Destroy is __thiscall.
// Shape: the implicit entry destructor inlines (members in reverse order: +8, +4, +0); the speed default gives the
// aligned loop (`lea ebx,[ebx+0]` pad) with the top `je` guard and bottom `jne` back edge.
class CWideString {
public:
    ~CWideString();                                     // 0x0099B510
private:
    void* PStringData;
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        CWideString Name;                               // +0x00
        CWideString AbbreviatedName;                    // +0x04
        CWideString Content;                            // +0x08
    };
};

namespace std {
struct __false_type {};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux<CHeroLogBook::CHeroLogBookEntry*>(CHeroLogBook::CHeroLogBookEntry*,
    CHeroLogBook::CHeroLogBookEntry*, const __false_type&);
}

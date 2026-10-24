// std::pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, std::vector<CHeroLogBook::CHeroLogBookEntry> >::`scalar deleting destructor'(unsigned int flags) -- destroy the entry vector (elements, then the storage), operator delete when bit 0 of the flags is set
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C350, 57 bytes, `ret 4`; no call/jmp site or absolute reference anywhere in
// the image (an unreferenced COMDAT the link kept).
// Callees: std::__destroy_aux<CHeroLogBookEntry*>(first, last, const __false_type&) @ 0x0047BAF0 (__fastcall, three
// ~CWideString @ 0x0099B510 per element; landed with a Dinkumware-style name), free @ 0x00BFEA14, operator delete
// @ 0x00BFE9BC.
// Layout: a 4-byte key at +0 (untouched) and an STLport vector<CHeroLogBookEntry> {start, finish, end of storage}
// at +4 = the value_type of CHeroLogBook::LogEntries (Ego_r.pdb: map<ECategory, vector<CHeroLogBookEntry>>, whose
// _Pair_base puts `second` at +4). The holder is inferred from that layout and the TU (0x0047C140 / 0x0047C470 /
// 0x0047C500 are the CHeroLogBook LogEntries map and vector members); no caller names it.
// Shape: no vtable store, so the class is non-polymorphic and the `??_G` is the non-virtual scalar deleting destructor
// `delete p` emits (as the landed CTCContainer 0x0074FDE0). STLport `~vector` = `_Destroy(start, finish)` ->
// `__destroy_aux(first, last, __false_type())` out of line (the empty tag homed over the flags slot), then
// `~_Vector_base` frees the storage behind a null test.
// Label correction: catalog "QuestManager_PersistInfo_Destructor" is a guess.
// Leaf: ??_G?$pair
#include <stdlib.h>

void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { CATEGORY_NONE };
        CWideString Name;                                   // +0x00
        CWideString AbbreviatedName;                        // +0x04
        CWideString Content;                                // +0x08
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

// 0x0047BAF0, defined in this TU (retail's copy sits beside this row) and kept out of line: the call site then knows
// the empty tag is never read and homes it over the flags slot.
#pragma auto_inline(off)
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp> _M_end_of_storage;         // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            free(_M_start);                                 // allocator<T>::deallocate -> __malloc_alloc
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};
}

typedef std::pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, std::vector<CHeroLogBook::CHeroLogBookEntry> >
    TLogEntryValue;

void DeleteLogEntryValue(TLogEntryValue* value)
{
    delete value;
}
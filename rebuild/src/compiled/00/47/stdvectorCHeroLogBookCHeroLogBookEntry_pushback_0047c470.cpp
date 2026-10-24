// std::vector<CHeroLogBook::CHeroLogBookEntry>::push_back(const CHeroLogBook::CHeroLogBookEntry& x) -- STLport append: copy-construct at the finish when there is room, else grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C470, 90 bytes, `ret 4`.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (x3: the implicit CHeroLogBookEntry copy
// constructor, inlined), vector<CHeroLogBookEntry>::_M_insert_overflow(position, x, const __false_type&, n, atend)
// @ 0x0047C140 (whose __uninitialized_copy 0x0047B670 is landed with this element).
// Shape: STLport 4.x `push_back`, same source as the CQuestDesc / CQuestObjectiveDesc instantiations 0x0047CDE0 /
// 0x0047C420: placement new with its null test, `++_M_finish` re-read, the `__false_type` temporary over the dead
// `x` slot. Speed TU (the CHeroLogBook module).
// Label correction: the catalog's "Vector_PushBack_DialogueLayerDef" is a guess; the 12-byte element of three
// CWideStrings is Ego_r.pdb CHeroLogBook::CHeroLogBookEntry (Name, AbbreviatedName, Content), the value type of
// CHeroLogBook::LogEntries (map<ECategory, vector<CHeroLogBookEntry>>).
// Leaf: push_back
#include <new>

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);               // 0x0099B720
};

namespace std {
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp> _M_end_of_storage;      // +0x08
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);       // 0x0047C140 for CHeroLogBookEntry
    void push_back(const _Tp& __x);
};
}

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
template <class _Tp> void vector<_Tp>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}

template void vector<CHeroLogBook::CHeroLogBookEntry>::push_back(const CHeroLogBook::CHeroLogBookEntry&);
}
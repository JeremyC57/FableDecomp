// CTCInventoryMap::AddLogBookSaveInfo(const CWideString& name, const CWideString& abbreviatedName, const CWideString& text, CHeroLogBook::CHeroLogBookEntry::ECategory category) -- append one log-book entry to the four parallel save-info vectors (name, abbreviated name, text, category)
// Leaf: ?AddLogBookSaveInfo@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FD9C0, 272 bytes (269 without padding), thiscall `ret 10h`.
// Identity: Ego_r.pdb `?AddLogBookSaveInfo@CTCInventoryMap@@QAEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@
//   CHeroLogBook@@@Z`; the four vectors sit at +0x150/+0x15C/+0x168/+0x174, the retail offsets of LogBookNameTag,
//   LogBookAbbreviatedNameTag, LogBookTextTag and LogBookCategory (landed CTCInventoryMap ctor 0x00606D20). The catalog
//   label "CHeroLogBook_AddLogEntry" is descriptive.
// Callees: CWideString copy constructor 0x0099B720 (push_back's null-tested placement copy),
//   vector<CWideString>::_M_insert_overflow(.., const __false_type&, 1, true) 0x00412330,
//   vector<ECategory>::_M_insert_overflow(.., const __false_type&, 1, true) 0x00618200.
// Model: local retail-layout model of CTCInventoryMap (no engine header; the PDB's 16-byte containers put the vectors
//   0x20 higher); the vectors are STLport 4.6 templates.
// Shape: four STLport `push_back`s inlined; VC7.1 hoists the `++_M_finish` element size into ebx; the growth members are
//   same-TU noinline stand-ins (auto_inline off) that read x through its reference and never touch the empty tag, so
//   the `__false_type()` temporaries are homed in the dead `name` argument slot, as retail.
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);                                           // 0x0099B720
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { ECategory_Force32 = 0x7fffffff };
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                                  size_type __fill_len, bool __atend)
{                                    // stand-in for 0x00412330 / 0x00618200: reads x, never touches the tag
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (*(const char*)&__x != 0);
}
#pragma auto_inline(on)
}

class CTCInventoryMap {
public:
    unsigned char _pad_0x000[0x150];
    std::vector<CWideString>                               LogBookNameTag;            // +0x150
    std::vector<CWideString>                               LogBookAbbreviatedNameTag; // +0x15C
    std::vector<CWideString>                               LogBookTextTag;            // +0x168
    std::vector<CHeroLogBook::CHeroLogBookEntry::ECategory> LogBookCategory;          // +0x174
    void AddLogBookSaveInfo(const CWideString& name, const CWideString& abbreviatedName, const CWideString& text,
                            CHeroLogBook::CHeroLogBookEntry::ECategory category);
};

void CTCInventoryMap::AddLogBookSaveInfo(const CWideString& name, const CWideString& abbreviatedName,
                                         const CWideString& text, CHeroLogBook::CHeroLogBookEntry::ECategory category)
{
    LogBookNameTag.push_back(name);
    LogBookAbbreviatedNameTag.push_back(abbreviatedName);
    LogBookTextTag.push_back(text);
    LogBookCategory.push_back(category);
}
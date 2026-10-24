// std::vector<CHeroLogBook::CQuestDesc>::push_back(const CHeroLogBook::CQuestDesc& x) -- STLport append: copy-construct at the finish when there is room, else grow through _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047CDE0, 96 bytes (+ int3 padding), `ret 4`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 and std::vector<CQuestObjectiveDesc>::vector(const
// vector&) @ 0x0047BA50 (the implicit CQuestDesc copy constructor, inlined), vector<CQuestDesc>::_M_insert_overflow(
// position, x, const __false_type&, n, atend) @ 0x0047CCD0.
// Shape: STLport 4.x `push_back(x) { if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x);
// ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true); }` -- placement new keeps the null
// test, `++_M_finish` re-reads the member (`add dword ptr [edi+4],18h`), the `__false_type` temporary is homed in
// the dead `x` argument slot (`lea eax,[esp+14h]`). Speed TU (the CHeroLogBook module, as the landed 0x0047C290).
// Label correction: the catalog's "Vector_PushBack_DialogueNarrative" is a guess; the 24-byte element built by
// ScriptID / CCharString 0x0099EC30 / QuestStatus / vector copy 0x0047BA50 is CHeroLogBook::CQuestDesc (see the
// landed __uninitialized_copy 0x0047C290 and __uninitialized_fill_n 0x0047C2E0 of the same vector). Caller 0x0047AF1C
// (CHeroLogBook).
// Leaf: push_back
#include <new>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
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
    vector(const vector& other);                         // 0x0047BA50 for CQuestObjectiveDesc
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);       // 0x0047CCD0 for CQuestDesc
    void push_back(const _Tp& __x);
};
}

class CHeroLogBook {
public:
    class CQuestObjectiveDesc;
    enum EQuestStatus { QUEST_STATUS_NONE };
    class CQuestDesc {
    public:
        unsigned long                          ScriptID;             // +0x00
        CCharString                            TextDescription;      // +0x04
        EQuestStatus                           QuestStatus;          // +0x08
        std::vector<CQuestObjectiveDesc>       QuestSubObjectives;   // +0x0c
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

template void vector<CHeroLogBook::CQuestDesc>::push_back(const CHeroLogBook::CQuestDesc&);
}
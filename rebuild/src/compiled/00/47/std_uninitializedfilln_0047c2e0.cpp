// std::__uninitialized_fill_n(CHeroLogBook::CQuestDesc* first, unsigned int n, const CHeroLogBook::CQuestDesc& x, const __false_type&) -- copy-construct n quest descriptions from x into raw storage, return the end
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C2E0, 68 bytes, `ret 8`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, std::vector<CQuestObjectiveDesc>::vector(const
// vector&) @ 0x0047BA50 (both through the implicit CQuestDesc copy constructor).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`: `for (; __n > 0; --__n, ++__cur)
// _Construct(&*__cur, __x);` -- `__fastcall` /Gr algorithm (first in ecx, the unsigned count in edx, x and the tag
// on the stack); placement new gives the null test, the element copy is the implicit memberwise copy constructor
// (ScriptID dword, the CCharString copy, QuestStatus dword, the vector copy). Speed TU (`nop` loop-head pad), the
// count rotated into `dec ebx ; jne`.
// Label correction: the catalog's "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is propagated; the element is
// Ego_r.pdb CHeroLogBook::CQuestDesc (24 bytes in retail with the 12-byte STLport vector), as the landed sibling
// __uninitialized_copy 0x0047C290; the caller 0x0047CD79 is the vector<CQuestDesc>::_M_insert_overflow 0x0047CCD0.
// Leaf: ??$__uninitialized_fill_n
#include <new>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    vector(const vector& other);                         // 0x0047BA50 for CQuestObjectiveDesc
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
struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CHeroLogBook::CQuestDesc* __fastcall __uninitialized_fill_n<CHeroLogBook::CQuestDesc*, unsigned int, CHeroLogBook::CQuestDesc>(
    CHeroLogBook::CQuestDesc*, unsigned int, const CHeroLogBook::CQuestDesc&, const __false_type&);
}
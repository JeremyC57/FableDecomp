// std::__uninitialized_copy<CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*>(CHeroLogBook::CQuestDesc* first, CHeroLogBook::CQuestDesc* last, CHeroLogBook::CQuestDesc* result, const __false_type&) -- copy-construct quest descriptions into raw storage, return the end
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe @ 0x0047C290, 72 bytes, __fastcall (first ecx, last edx, result + the __false_type tag on the stack,
// `ret 8`), returns the end in eax; callers 0x0047CD2A / 0x0047CD95 inside the vector<CQuestDesc> _M_insert_overflow (STLport).
// Catalog label std::_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int>>, unsigned int*> is wrong: the 24-byte
// element is built by an implicit copy constructor (dword +0, CCharString copy 0x0099EC30 at +4, dword +8, the
// vector<CQuestObjectiveDesc> copy constructor 0x0047BA50 at +0xc) behind a placement-new null test, i.e. Ego_r.pdb
// CHeroLogBook::CQuestDesc {ulong ScriptID, CCharString TextDescription, EQuestStatus QuestStatus, vector QuestSubObjectives}
// (28 bytes in the PDB with its 16-byte vector; 12-byte STLport vector in retail).
// Shape: /O2 (`nop` loop-head pad; first == last tested before entry).
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
template CHeroLogBook::CQuestDesc* __fastcall __uninitialized_copy<CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*>(CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*, const __false_type&);
}
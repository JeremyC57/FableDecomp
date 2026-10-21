// std::__uninitialized_copy<CHeroLogBook::CQuestObjectiveDesc*, CHeroLogBook::CQuestObjectiveDesc*>(CHeroLogBook::CQuestObjectiveDesc* first, CHeroLogBook::CQuestObjectiveDesc* last, CHeroLogBook::CQuestObjectiveDesc* result, const __false_type&) -- copy-construct quest objectives into raw storage, return the end
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe @ 0x0047B5F0, 58 bytes, __fastcall (first ecx, last edx, result + the __false_type tag on the stack,
// `ret 8`), returns the end in eax; callers 0x0047C09B / 0x0047C0F2 inside the vector _M_insert_overflow 0x0047C030 (used by the objective push_back 0x0047C420 and by 0x0047A9D6 in the add-objective code at 0x0047A8E0).
// Catalog label std::_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int>>, unsigned int*> is wrong: the 12-byte
// element is built by an implicit copy constructor (dword +0, CCharString copy 0x0099EC30 at +4, byte +8) behind a
// placement-new null test, i.e. Ego_r.pdb CHeroLogBook::CQuestObjectiveDesc {ulong SubObjectiveID, CCharString
// TextDescription, bool QuestObjectiveCompleted}; Ego_r.pdb has the same two instantiations (??$_Uninit_copy@PAV... 0x004881A0
// and the const-iterator one 0x00488100). 0x0047B5F0 and 0x0047B830 are byte-identical; the pointer/const-pointer split follows
// their callers (insert-overflow vs. copy/assign).
// Shape: /O2 (`nop` loop-head pad; first == last tested before entry).
#include <new>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};
class CHeroLogBook {
public:
    class CQuestObjectiveDesc {
    public:
        unsigned long SubObjectiveID;                    // +0x00
        CCharString   TextDescription;                   // +0x04
        bool          QuestObjectiveCompleted;           // +0x08
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
template CHeroLogBook::CQuestObjectiveDesc* __fastcall __uninitialized_copy<CHeroLogBook::CQuestObjectiveDesc*, CHeroLogBook::CQuestObjectiveDesc*>(CHeroLogBook::CQuestObjectiveDesc*, CHeroLogBook::CQuestObjectiveDesc*, CHeroLogBook::CQuestObjectiveDesc*, const __false_type&);
}
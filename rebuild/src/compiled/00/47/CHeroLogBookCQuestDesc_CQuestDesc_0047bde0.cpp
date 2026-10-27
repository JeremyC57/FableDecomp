// CHeroLogBook::CQuestDesc::CQuestDesc(const CQuestDesc& other) -- implicit memberwise copy: ScriptID, the CCharString text, QuestStatus and the sub-objective vector
// Leaf: ??0CQuestDesc@CHeroLogBook@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x0047BDE0, 49 bytes, __fastcall `ret 4`.
// Identity (proven): Ego_r.pdb names ??0CQuestDesc@CHeroLogBook@@QAE@ABV01@@Z; the body copies the PDB layout
// {ScriptID +0, TextDescription +4 (CCharString copy ctor 0x0099EC30), QuestStatus +8, QuestSubObjectives +0x0C (the bound
// vector<CQuestObjectiveDesc> copy ctor 0x0047BA50)}, and the vector<CQuestDesc> helpers 0x0047C250/0x0047C290 copy the same
// 0x18-byte record. The landed default ctor 0x0047BF90 builds the same members.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, vector<CQuestObjectiveDesc>::vector(const vector&) @ 0x0047BA50.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(const vector<_Tp, _Alloc>& __x);                 // 0x0047BA50
};
}
enum EQuestStatus { QUEST_STATUS_UNPROVEN_0 };          // Ego_r.pdb type name; enumerators unproven (4 bytes)
class CHeroLogBook {
public:
    class CQuestObjectiveDesc {                            // 12 bytes (Ego_r.pdb)
    public:
        unsigned long SubObjectiveID;
        CCharString TextDescription;
        bool QuestObjectiveCompleted;
    };
    class CQuestDesc {                                     // 0x18 bytes (TLC, STLport vector)
    public:
        unsigned long ScriptID;                            // +0x00
        CCharString TextDescription;                       // +0x04
        EQuestStatus QuestStatus;                          // +0x08
        std::vector<CQuestObjectiveDesc> QuestSubObjectives;  // +0x0C
        CQuestDesc(const CQuestDesc& other);
    };
};
CHeroLogBook::CQuestDesc::CQuestDesc(const CQuestDesc& other)
    : ScriptID(other.ScriptID)
    , TextDescription(other.TextDescription)
    , QuestStatus(other.QuestStatus)
    , QuestSubObjectives(other.QuestSubObjectives)
{
}
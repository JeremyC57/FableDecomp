// CQuestDesc::CQuestDesc() -- a hero-logbook quest description: script id 999, the text "No description yet", status 0 and an empty sub-objective vector
// Leaf: ??0CQuestDesc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047BF90, length 42 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with the literal @ 0x01236674
// ("No description yet") and -1.
// Label: the catalog name "CUIMiniMapMarker::CUIMiniMapMarker" is propagated; the literal, the 999 id and
// the PDB layout match CQuestDesc (28 bytes in the PDB, 24 in retail: STLport vector = 12 bytes):
// ScriptID +0, TextDescription +4, QuestStatus +8, QuestSubObjectives (vector<CQuestObjectiveDesc>) +0x0c.
// The function right after (0x0047BFC0) walks the same vector at +0x0c/+0x10 destroying its strings.
// Shape: the id store is scheduled after the string constructor's argument pushes; the status and the
// vector's three words share one `xor eax,eax` after the string.
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
};

struct CQuestObjectiveDesc;

template <class T> struct vector {                 // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CQuestDesc {
    unsigned long               ScriptID;          // +0x00
    CCharString                 TextDescription;   // +0x04
    long                        QuestStatus;       // +0x08 EQuestStatus
    vector<CQuestObjectiveDesc> QuestSubObjectives;   // +0x0c
    CQuestDesc();
};

CQuestDesc::CQuestDesc()
    : ScriptID(999)
    , TextDescription("No description yet", -1)
    , QuestStatus(0)
    , QuestSubObjectives()
{
}

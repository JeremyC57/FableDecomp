// CV_TalentlessBardScript::ProcessSongsQuestComplete() -- for each of the 12 quests whose song is available, copy its song tag into the bard's SongItems list; every step and the end are guarded by the script-thread terminate test
// Leaf: ?ProcessSongsQuestComplete@CV_TalentlessBardScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED9B40, length 103 bytes (padded to 112).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (x3), CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// std::vector<CCharString>::push_back(const CCharString&) @ 0x0044BFF0, CCharString::~CCharString @ 0x0099EAE0.
// Layout (retail; PDB offsets -0x14 up to the first vector, -0x18 after it because retail vectors are 12 bytes):
// stBard +0x54, BardWanderTime +0x64, SongItems +0x68, QuestInfo QuestData[12] +0x74 (8 bytes each: name +0, tag +4),
// SongThingAvailable +0xd4 (retail indexes it as one byte per entry: `cmp byte ptr [esi+eax],0`).
// Shape: the tag copy is a named local (`lea ecx ; push ecx`; an unnamed temporary reuses the ctor's eax); the script macros `if (IsActiveThreadTerminating()) return;` around each step and once more at the end.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
struct CCharStringVector {                         // std::vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void push_back(const CCharString& value);      // 0x0044BFF0
};
struct CBoolVector {                               // SongThingAvailable: one byte per entry in retail
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool* begin() { return _M_start; }
    bool& operator[](unsigned int n) { return *(begin() + n); }
};
struct QuestData {
    CCharString name;                              // +0
    CCharString tag;                               // +4
};
class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CQuestScriptObject : CScriptBase {
    void* Game;                                    // +0x40
    void* Data;                                    // +0x44
};
namespace NScript {
class CV_TalentlessBardScript : public CQuestScriptObject {
public:
    unsigned char     _flags48[0x20];              // +0x48 bHurtBard .. BardFollowing, stBard +0x54, BardWanderTime +0x64
    CCharStringVector SongItems;                   // +0x68
    QuestData         QuestInfo[12];               // +0x74
    CBoolVector       SongThingAvailable;          // +0xd4
    void ProcessSongsQuestComplete();
};
} // namespace NScript
using NScript::CV_TalentlessBardScript;
void NScript::CV_TalentlessBardScript::ProcessSongsQuestComplete()
{
    for (long i = 0; i < 12; ++i) {
        if (IsActiveThreadTerminating())
            return;
        if (SongThingAvailable[i]) {
            if (IsActiveThreadTerminating())
                return;
            CCharString tag(QuestInfo[i].tag);
            SongItems.push_back(tag);
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
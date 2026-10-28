// NScript::CQ_FireHeartScript::TriggerProphetComment(const CCharString& comment) -- build the next prophet's comment text key ("TEXT_QST_B01_PROPHET<n>_<comment>") and flag that prophet to speak
// Leaf: ?TriggerProphetComment@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38400, 145 bytes, `__fastcall`, one stack argument (`ret 4`).
//   FableWin symbol ?TriggerProphetComment@CQ_FireHeartScript@NScript@@QAEXABVCCharString@@@Z. Callers: IncNumberOfSuns
//   @ 0x00D38310 (x2), DecNumberOfSuns @ 0x00D38AA0 (x2), 0x00D39E98.
// Label correction: the catalog "CTCGameModeController_UpdateSunCount" is descriptive and names the wrong class.
// Callees: GFIntToCharString(long) @ 0x0099F830, operator+(const char*, const CCharString&) @ 0x0099F690,
//   operator+(const CCharString&, const char*) @ 0x0099F600, operator+(const CCharString&, const CCharString&) @ 0x0099F570
//   (all __fastcall with the hidden result in ecx), CCharString::operator=(const CCharString&) @ 0x0099EFB0,
//   ~CCharString @ 0x0099EAE0 (x4). Strings: "TEXT_QST_B01_PROPHET" @ 0x012CD118, "_" @ 0x01244DB4.
// Layout (retail = Ego_r.pdb CQ_FireHeartScript with the 0x40-byte script base and 12-byte STLport containers):
//   FreedProphets +0x64, KilledProphets +0x68, ShouldProphetSpeak (vector<bool>, a plain byte vector in Fable's
//   STLport) +0xE0, ProphetComment +0x188.
// Shape: speed TU (no pragma). One chained expression evaluated right to left: the right operands of the outer `+`s
//   (comment, then "_") are pushed first, the innermost GFIntToCharString(Freed + Killed + 1) runs first; the four
//   temporaries die in reverse order, the last one homed in the dead argument slot.
class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
}

namespace NScript {
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
class CQ_FireHeartScript : public CScriptBase {
public:
    void*              Game;                            // +0x40
    void*              MasterScriptData;                // +0x44
    unsigned char      _state48[0x64 - 0x48];           // QuestStartScreenShown .. csargs
    long               FreedProphets;                   // +0x64
    long               KilledProphets;                  // +0x68
    unsigned char      _state6c[0xe0 - 0x6c];           // StartNumberOfSuns .. ProphetsReleased
    std::vector<bool> ShouldProphetSpeak;              // +0xe0
    unsigned char      _stateec[0x188 - 0xec];          // ShouldVillagerSpeak .. MissionHistory
    CCharString        ProphetComment;                  // +0x188
    void TriggerProphetComment(const CCharString& comment);
};
void CQ_FireHeartScript::TriggerProphetComment(const CCharString& comment)
{
    ProphetComment = "TEXT_QST_B01_PROPHET" + GFIntToCharString(FreedProphets + KilledProphets + 1) + "_" + comment;
    ShouldProphetSpeak[FreedProphets + KilledProphets] = true;
}
}
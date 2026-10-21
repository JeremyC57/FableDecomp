// CFH_Prisoner::MakeRoundStartComment() -- the prophet's "TEXT_QST_B01_PROPHET<round + 1>_START" line, suffixed GOOD / DEFAULT / BAD by the quest's mood once the quest is under way, said to the hero in a new conversation (bailing out whenever the script is done)
// Leaf: MakeRoundStartComment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3A9B0, length 327 bytes (`ret`).
// Callees: GFIntToCharString(long) @ 0x0099F830, operator+(const char*, const CCharString&) @ 0x0099F690 and
//   operator+(const CCharString&, const char*) @ 0x0099F600 (all `__fastcall` with the hidden result in ecx),
//   CCharString::operator+=(const char*) @ 0x0099F100, ~CCharString @ 0x0099EAE0, the entity script's IsDone @ 0x00F35B30;
//   CGameScriptInterface slots 70 GetHero, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation.
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8, the owning quest
//   script +0x14 (its started counter +0x88, mood +0x184), the round counter +0x20.
// Shape: every mood arm re-tests IsDone before appending; the callee-saved pushes for the conversation sit after the tests.
struct CScriptThing { unsigned char _body[12]; };
struct CCharString {
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {
    VPAD64(pa) VPAD4(pb) VPAD2(pc)                                                   // slots 0..69
    virtual const CScriptThing& GetHero();                                           // slot 70 (+0x118)
    VPAD16(qa) VPAD2(qb)                                                             // slots 71..88
    virtual void SetTimer(long timer, long time);                                    // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;                                         // slot 90 (+0x168)
    VPAD256(ra) VPAD8(rb) VPAD4(rc)                                                  // slots 91..358
    virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing& thing, const CCharString& baseName) const;   // slot 359
    VPAD4(sa)                                                                        // slots 360..363
    virtual long AddNewConversation(const CScriptThing& speaker, bool a, bool b) const;   // slot 364 (+0x5B0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5B4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool b, const CScriptThing& speaker,
                                       const CScriptThing& listener) const;          // slot 366 (+0x5B8)
};
struct CQuestScript_Retail {
    unsigned char _pad000[0x88];
    long          RoundsStarted;                        // +0x88
    unsigned char _pad08c[0xf8];
    long          Mood;                                 // +0x184
};

class CFH_Prisoner {
public:
    void*                   __vftable;                  // +0x00
    CGameScriptInterface*   Game;                       // +0x04
    CScriptThing            Me;                         // +0x08
    CQuestScript_Retail*    Quest;                      // +0x14
    long                    _pad18[2];                  // +0x18
    long                    Round;                      // +0x20
    bool IsDone();                                      // 0x00F35B30
    void MakeRoundStartComment();
};

void CFH_Prisoner::MakeRoundStartComment()
{
    CCharString comment = "TEXT_QST_B01_PROPHET" + GFIntToCharString(Round + 1) + "_START";
    if (Quest->RoundsStarted > 0) {
        if (IsDone())
            return;
        long mood = Quest->Mood;
        if (mood == 1) {
            if (IsDone())
                return;
            comment += "GOOD";
        } else if (mood == 2) {
            if (IsDone())
                return;
            comment += "DEFAULT";
        } else if (mood == 3) {
            if (IsDone())
                return;
            comment += "BAD";
        } else if (IsDone())
            return;
    }
    long conversation = Game->AddNewConversation(Me, true, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, comment, false, Me, Game->GetHero());
}

// CFisherman::IdleChatter() -- when the hero is within 8 units and the idle timer has run out, restart the timer and start a conversation in which the fisherman says his attitude-matched idle line (tutorial or normal, by the quest state); otherwise poll the done state
// Grade: DIFFER(ours v retail) -- the two case blocks share frame words (sub esp,10h) where retail gives each case its own line/result words (14h), and the IsDone exit is laid out at the end instead of after the distance test
// Leaf: IdleChatter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7DE60, length 372 bytes (`ret`).
// Callees: IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE3EA (`__fastcall`), the
//   entity script's IsDone @ 0x00F35B30 (result unused here; the landed CFishermanWasp::Main 0x00E7BB30 name), CCharString
//   ctor/dtor @ 0x0099EBF0 / 0x0099EAE0; CGameScriptInterface slots 70 GetHero (+0x118), 89 SetTimer (+0x164), 90 GetTimer
//   (+0x168), 359 GetValidTextEntryNameWithAttitude (+0x59C), 364 AddNewConversation (+0x5B0), 365 AddPersonToConversation
//   (+0x5B4), 366 AddLineToConversation (+0x5B8) (ghidra_out/cgsi_slots_aligned.tsv); globals g_pGameScriptInterface
//   0x0143E8F8 and the script def 0x0143E90C (idle chatter time +0x594).
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8, the owning quest
//   script +0x14 (its state +0x54), the idle timer +0x1C.
// Shape: each line's Game pointer is read once for the inner GetHero / text lookup and again for the outer call; the
//   state switch lays case 4 first.
struct CScriptThing { unsigned char _body[12]; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA

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
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CScriptDef_Retail {
    unsigned char _pad000[0x594];
    long          FishermanIdleChatterTime;             // +0x594
};
extern CScriptDef_Retail* g_pScriptDef;                 // 0x0143E90C
struct CQuestScript_Retail {
    unsigned char _pad00[0x54];
    long          State;                                // +0x54
};

class CFisherman {
public:
    void*                   __vftable;                  // +0x00
    CGameScriptInterface*   Game;                       // +0x04
    CScriptThing            Me;                         // +0x08
    CQuestScript_Retail*    Quest;                      // +0x14
    long                    _pad18;                     // +0x18
    long                    IdleTimer;                  // +0x1C
    bool IsDone();                                      // 0x00F35B30
    void IdleChatter();
};

void CFisherman::IdleChatter()
{
    if (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f) || g_pGameScriptInterface->GetTimer(IdleTimer) > 0) {
        IsDone();
        return;
    }
    g_pGameScriptInterface->SetTimer(IdleTimer, g_pScriptDef->FishermanIdleChatterTime);
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    switch (Quest->State) {
    case 4: {
        CCharString line("TEXT_QST_023_IDLE");
        Game->AddLineToConversation(conversation, Game->GetValidTextEntryNameWithAttitude(Me, line), false, Me, Game->GetHero());
        break;
    }
    case 2: {
        CCharString line("TEXT_QST_023_TUTORIAL_IDLE");
        Game->AddLineToConversation(conversation, Game->GetValidTextEntryNameWithAttitude(Me, line), false, Me, Game->GetHero());
        break;
    }
    }
}

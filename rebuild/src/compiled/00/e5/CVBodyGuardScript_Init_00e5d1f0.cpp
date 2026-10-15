// CV_BodyGuardScript::Init() -- reset the bodyguard script state: wages, flags, conversation index, the owner's word at +8, the commentary timer (to 0) and the four henchman status words
// Retail: Fable.exe @ 0x00E5D1F0, 70 bytes (+ int3 padding). Reached only through a vtable word (0x012E6E58).
// Callees: CGameScriptInterface::SetTimer (slot 89, +0x164) through the interface global @ 0x0143E8F8.
// Layout: Ego_r.pdb CV_BodyGuardScript - 0x14 (and - 4 more after the 12-byte retail HenchmanList vector): TotalWages
// +0x48, HeroCannotAffordHelp +0x4c, HeroWieldingWeapon +0x4d, CommentaryTimer +0x5c, ConversationIndex +0x70,
// HenchmanGlobalStatus[4] +0x84. +0x44 is the script's owner pointer (the constructor's first argument); the word it
// clears at +8 is unnamed.
// Shape: statement order = store order around the SetTimer call (all zeros from ebx); the four status words are
// written out (retail has no loop).
struct CGameScriptInterface {                        // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long timer, long time);          // slot 89 (+0x164)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CScriptOwner {
    unsigned char _pad[8];
    long m_value08;                                      // +0x08
};

struct CTimer {
    long TimerIndex;
};

struct CV_BodyGuardScript {
    unsigned char _base[0x40];                           // CScriptBase
    void* Game;                                          // +0x40
    CScriptOwner* Owner;                                 // +0x44
    long TotalWages;                                     // +0x48
    bool HeroCannotAffordHelp;                           // +0x4c
    bool HeroWieldingWeapon;                             // +0x4d
    long HeroStoodStillCounter;                          // +0x50 (CCounter)
    CTimer HenchmanLastIdleChatTimer;                    // +0x54
    CTimer HenchmanLastBoredChatTimer;                   // +0x58
    CTimer CommentaryTimer;                              // +0x5c
    CTimer HenchmanFreakedOutThePlayerTimer;             // +0x60
    void* HenchmanList[3];                               // +0x64 (vector<CScriptThing>)
    long ConversationIndex;                              // +0x70
    CTimer HenchmanDeadRespawnTimer[4];                  // +0x74
    long HenchmanGlobalStatus[4];                        // +0x84
    void Init();
};

void CV_BodyGuardScript::Init()
{
    ConversationIndex = 0;
    TotalWages = 0;
    HeroCannotAffordHelp = false;
    Owner->m_value08 = 0;
    g_pGameScriptInterface->SetTimer(CommentaryTimer.TimerIndex, 0);
    HeroWieldingWeapon = false;
    HenchmanGlobalStatus[0] = 0;
    HenchmanGlobalStatus[1] = 0;
    HenchmanGlobalStatus[2] = 0;
    HenchmanGlobalStatus[3] = 0;
}
// CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, interface/owner pair, vtable 0x012E2EE4, two registered timers, ten empty random-speech strings
// Leaf: ??0CV_ArcheryCompetitionScript
// Retail: Fable.exe @ 0x00E32350, 93 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8 (twice),
// CCharString::CCharString() @ 0x0099E4B0 (ten times, inline counted loop).
// Layout: Ego_r.pdb CV_ArcheryCompetitionScript minus 0x14 (the retail script base is 0x14 bytes smaller):
// GameTimeLeft (CTimer) +0x6c, RandomSpeechTimer (CTimer) +0x70, RandomSpeech (CCharString[10]) +0x74.
// Shape (landed 0x00E68F70 model): the arguments are stored before the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); each CTimer member registers
// itself in its inline constructor; the string array is built by an inline `dec ebx ; jne` loop (no EH module).
#include "fable_string.h"

class CGameScriptInterface { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual long RegisterTimer();                    // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

struct CV_ArcheryCompetitionScript : CScriptObject { // vtable 0x012E2EE4
    long Score;                                      // +0x48
    bool IsOwnerDead;                                // +0x4c
    bool PrizeAlreadyGot;                            // +0x4d
    bool QuestActivated;                             // +0x4e
    bool AttackedByHero;                             // +0x4f
    bool GiveAttackWarning;                          // +0x50
    bool QuestFinished;                              // +0x51
    bool WelcomeComplete;                            // +0x52
    bool Pickpocketed;                               // +0x53
    long DummyCounter;                               // +0x54
    long ArcheryHighScoreInfoID;                     // +0x58
    long ArcheryGameScoreInfoID;                     // +0x5c
    long ArcheryGameTimerID;                         // +0x60
    bool SteppedOut;                                 // +0x64
    bool UsedMagic;                                  // +0x65
    bool ShotOwner;                                  // +0x66
    bool GivenReallyCoolPrize;                       // +0x67
    bool GivenFirstHighScorePrize;                   // +0x68
    bool AddedReallyCoolPrizeToOwner;                // +0x69
    bool AddedFirstHighScorePrizeToOwner;            // +0x6a
    CTimer GameTimeLeft;                             // +0x6c
    CTimer RandomSpeechTimer;                        // +0x70
    CCharString RandomSpeech[10];                    // +0x74
    long ScorePowerLevel;                            // +0x9c
    long ContestState;                               // +0xa0
    virtual void v00();
    CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game);
};

CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}
// CTraderToEscort::GetRandomSpeech() -- while the escorted trader follows (quest stage 2) or waits (stage 3), pick one of the 10 follow / 5 wait speech lines at random, never either of the last two picks of that kind, yielding a script frame per rejected pick (or other stage); the empty string when the script is done
// Leaf: GetRandomSpeech
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E23F30, length 347 bytes (`ret 4`, CCharString by hidden pointer).
// Callees: CCharString default ctor @ 0x0099E4B0, copy ctor @ 0x0099EC30, operator= @ 0x0099EFB0, dtor @ 0x0099EAE0,
//   rand @ 0x00BFEB16, the entity script's IsDone @ 0x00F35B30 (manifest label "CCreatureAction_TrollWhackGroundBase::
//   Initialise" is propagated), the game script interface (+4) slot 7 (+0x1c, one script frame).
// Layout: Ego_r.pdb CTraderToEscort (44 bytes): ParentClass +0x14 (CQR_EscortTraderScript*), PMasterData +0x18,
//   lastRandomSpeechFollowIdx1/2 +0x1c/+0x20, lastRandomSpeechWaitIdx1/2 +0x24/+0x28 (as the PDB).
//   CQR_EscortTraderScript (retail, read here): RandomSpeechFollow[10] +0x88, RandomSpeechWait[5] +0xb0, QuestStage +0x11c
//   (Ego_r.pdb +0x9c / +0xc4 / +0x148: the record's earlier members are 0x14 smaller in retail and each of the six
//   std::vector<CCharString> route members is 12 bytes, not 16).
// Shape: the landed CAC_Owner::GetRandomSpeech (0x00E2E440) with two stages: an empty string, a copy of it returned on every
//   IsDone exit, and the chosen line assigned to a third local and copied out (no NRVO).
struct CCharString
{
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)

struct CGameScriptInterface
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual void NewScriptFrame();                  // slot 7 (+0x1c): yield one frame
};

enum EscortTraderQuestStage { ESCORT_STAGE_FOLLOW = 2, ESCORT_STAGE_WAIT = 3 };

struct CQR_EscortTraderScript                       // retail layout of the members read here
{
    unsigned char          _pad00[0x88];
    CCharString            RandomSpeechFollow[10];  // +0x88
    CCharString            RandomSpeechWait[5];     // +0xb0
    unsigned char          _padc4[0x11c - 0xc4];
    EscortTraderQuestStage QuestStage;              // +0x11c
};

struct CTraderToEscort
{
    virtual void v00();
    CGameScriptInterface*   GSI;                    // +0x04
    unsigned char           _pad08[0x14 - 0x08];    // Me (CScriptThing)
    CQR_EscortTraderScript* ParentClass;            // +0x14
    void*                   PMasterData;            // +0x18
    long lastRandomSpeechFollowIdx1;                // +0x1c
    long lastRandomSpeechFollowIdx2;                // +0x20
    long lastRandomSpeechWaitIdx1;                  // +0x24
    long lastRandomSpeechWaitIdx2;                  // +0x28
    bool IsDone();                                  // 0x00F35B30
    CCharString GetRandomSpeech();
};

CCharString CTraderToEscort::GetRandomSpeech()
{
    CCharString empty;
    CCharString none(empty);
    CCharString speech;
    long index;
    for (;;)
    {
        if (IsDone())
            return none;
        if (ParentClass->QuestStage == ESCORT_STAGE_FOLLOW)
        {
            if (IsDone())
                return none;
            index = rand() % 10;
            if (index != lastRandomSpeechFollowIdx1 && index != lastRandomSpeechFollowIdx2)
            {
                if (IsDone())
                    return none;
                lastRandomSpeechFollowIdx2 = lastRandomSpeechFollowIdx1;
                lastRandomSpeechFollowIdx1 = index;
                speech = ParentClass->RandomSpeechFollow[index];
                break;
            }
        }
        else if (ParentClass->QuestStage == ESCORT_STAGE_WAIT)
        {
            if (IsDone())
                return none;
            index = rand() % 5;
            if (index != lastRandomSpeechWaitIdx1 && index != lastRandomSpeechWaitIdx2)
            {
                if (IsDone())
                    return none;
                lastRandomSpeechWaitIdx2 = lastRandomSpeechWaitIdx1;
                lastRandomSpeechWaitIdx1 = index;
                speech = ParentClass->RandomSpeechWait[index];
                break;
            }
        }
        GSI->NewScriptFrame();
    }
    if (IsDone())
        return none;
    return speech;
}
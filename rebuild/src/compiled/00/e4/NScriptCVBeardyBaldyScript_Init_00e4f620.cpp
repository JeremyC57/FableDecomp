// NScript::CV_BeardyBaldyScript::Init() -- reset the Beardy Baldy hair quest: every flag false, phase 0 with InitialisePhase set, the three required-hair names emptied, the hair/death flags and counters cleared, and the ten random-speech text keys
// Leaf: ?Init@CV_BeardyBaldyScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4F620, 257 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, from slot 3 (Init) of the RTTI-less level-script vtable 0x012E5790, which the
//   landed NScript::CV_BeardyBaldyScript::CV_BeardyBaldyScript 0x00E4F5B0 and RegisterMain 0x00E4F730 use (level-script
//   slots: 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist); Ego_r.pdb ?Init@CV_BeardyBaldyScript@NScript@@UAEXXZ
//   (0x00EA4010, right after the ctor 0x00EA3F60). Catalog label sub_00e4f620.
// Callees: CCharString::operator=(const char*) 0x0099EFE0 (Ego_r ??4CCharString@@QAEABV0@PBD@Z), thirteen times.
// Literals: "" 0x0122D70E (three times), "TEXT_QST_014_RANDOM_00" .. "_90" 0x012E587C .. 0x012E57A4.
// Layout: Ego_r.pdb CV_BeardyBaldyScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40), as the landed
//   ctor 0x00E4F5B0 has them (RequiredHairdo +0x58, RandomSpeechTimer +0x74, RandomSpeech[10] +0x78, size 0xA4).
// Shape: the landed CV_ArcheryCompetitionScript::Init 0x00E323B0 model; the flag stores ahead of each operator= call are
//   scheduled around its `push / lea ecx` (ebx = 0).
class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
};
class CGameScriptInterfaceBase;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFinished;                                 // +0x48
    bool QuestComplete;                                 // +0x49
    bool QuestActivated;                                // +0x4a
    long QuestPhase;                                    // +0x4c
    bool InitialisePhase;                               // +0x50
    bool IntroComplete;                                 // +0x51
    bool Phase1RequirementsComplete;                    // +0x52
    bool Phase2RequirementsComplete;                    // +0x53
    bool Phase3RequirementsComplete;                    // +0x54
    CCharString RequiredHairdo;                         // +0x58
    CCharString RequiredBeard;                          // +0x5c
    CCharString RequiredTash;                           // +0x60
    bool AcquiredNewHairdo;                             // +0x64
    bool AcquiredNewBeard;                              // +0x65
    bool AcquiredNewTash;                               // +0x66
    bool AllHairChanged;                                // +0x67
    bool BeardyBaldyKilled;                             // +0x68
    bool BarberLadyKilled;                              // +0x69
    bool BeardyBaldyLeft;                               // +0x6a
    long IncorrectHairComboCount;                       // +0x6c
    bool AttackedByHero;                                // +0x70
    CTimer RandomSpeechTimer;                           // +0x74
    CCharString RandomSpeech[10];                       // +0x78
    enum { BEARDY_BALDY_PHASE_0 } BeardyBaldyPhase;     // +0xa0 (unnamed enum in the PDB)
    virtual void Init();
};

void CV_BeardyBaldyScript::Init()
{
    QuestFinished = false;
    QuestActivated = false;
    QuestComplete = false;
    QuestPhase = 0;
    InitialisePhase = true;
    IntroComplete = false;
    Phase1RequirementsComplete = false;
    Phase2RequirementsComplete = false;
    Phase3RequirementsComplete = false;
    RequiredHairdo = "";
    RequiredBeard = "";
    RequiredTash = "";
    BeardyBaldyKilled = false;
    BarberLadyKilled = false;
    BeardyBaldyLeft = false;
    AcquiredNewHairdo = false;
    AcquiredNewBeard = false;
    AcquiredNewTash = false;
    AllHairChanged = false;
    IncorrectHairComboCount = 0;
    AttackedByHero = false;
    RandomSpeech[0] = "TEXT_QST_014_RANDOM_00";
    RandomSpeech[1] = "TEXT_QST_014_RANDOM_10";
    RandomSpeech[2] = "TEXT_QST_014_RANDOM_20";
    RandomSpeech[3] = "TEXT_QST_014_RANDOM_30";
    RandomSpeech[4] = "TEXT_QST_014_RANDOM_40";
    RandomSpeech[5] = "TEXT_QST_014_RANDOM_50";
    RandomSpeech[6] = "TEXT_QST_014_RANDOM_60";
    RandomSpeech[7] = "TEXT_QST_014_RANDOM_70";
    RandomSpeech[8] = "TEXT_QST_014_RANDOM_80";
    RandomSpeech[9] = "TEXT_QST_014_RANDOM_90";
}
}
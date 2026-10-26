// CTCTrophy::FrameUpdate() -- trophy minigame tick: plays pending witness sounds; while the minigame runs (and the thing manager is not paused) counts the frames down, and either times out (out-of-time sound, "time up" dialog, wait for terminate) or updates the quest-info minigame display (time fraction, witnesses gained, witness total)
// Leaf: ?FrameUpdate@CTCTrophy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF480, 292 bytes (plain `ret`).
// Callees: CTCTrophy::UpdateGainedWitnessSounds() 0x006DEE80; CTCBase::GetThingManager() 0x00686AE0 (x2);
//          CTCTrophy::PlaySound(const CCharString&) 0x006DDCD0; CCharString(const char*, long) 0x0099EBF0
//          ("TEXT_GUI_TROPHY_TIME_UP" 0x0125FE78); NGameText::CDataBank::GetTextBySymbol(const CCharString&) 0x009C95E0
//          (text bank g_pMainGameComponent +0x14); ~CCharString 0x0099EAE0; CWideString(const wchar_t*) 0x0099B6B0
//          (L"" 0x0122D70C, x3); CWideString(const CWideString&) 0x0099B720; CTCTrophy::StartDialogBox(CWideString,
//          unsigned long, CWideString, CWideString, CWideString) 0x006DD7E0; ~CWideString 0x0099B510;
//          CDrawQuestInfo::UpdateMiniGame(float, long, long) 0x00644EFD (g_pPlayerGui +0x1c8).
// Layout (retail, PDB - 0x10 after the two 4-byte lists): CTCTrophy PDef +0x0c, MinigameInProgress +0x14,
//          MinigameInitialFramesLeft +0x1c, MinigameCurrentFramesLeft +0x20, MinigameStartingWitnessCount +0x24,
//          NumberOfWitnessesLeft +0x28, WaitingForTerminate +0x3e; CTrophyDef::SoundOutOfTime +0x54 (PDB offset holds);
//          CThingManager +0x54/+0x55 two pause flags.
// Shape: the text symbol is an implicit "..." conversion temporary (re-addressed with `lea`, an explicit CCharString pushes eax); the decremented frame count is a named local spilled at once (the `fild` source); the dialog strings are
//        by-value arguments built in their stack slots (three L"" from the right, then the copy of the named text);
//        the fraction is `left / (initial * 2.0f)` (VC folds the doubling into `fadd st,st`).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
class CWideString { public:
    void* m_data;
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
};
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
namespace NGameText {
class CDataBank { public:
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
}
class CMainGameComponent { public:
    unsigned char        _pad_0x00[0x14];
    NGameText::CDataBank* PTextBank;            // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CDrawQuestInfo {
    void UpdateMiniGame(float timeFraction, long gained, long total);   // 0x00644EFD
};
class CPlayerGui { public:
    unsigned char   _pad_0x00[0x1c8];
    CDrawQuestInfo* PDrawQuestInfo;             // +0x1c8
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CThingManager { public:
    unsigned char _pad_0x00[0x54];
    bool          PausedA;                      // +0x54
    bool          PausedB;                      // +0x55
};
class CTrophyDef { public:
    unsigned char _pad_0x00[0x54];
    CCharString   SoundOutOfTime;               // +0x54
};

class CTCBase { public:
    virtual void v00();
    void*         ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
    CThingManager* GetThingManager();           // 0x00686AE0
};
class CTCTrophy : public CTCBase { public:
    CTrophyDef*   PDef;                         // +0x0c (CDefPointer<const CTrophyDef>)
    long          NonMinigameDeedHandle;        // +0x10
    bool          MinigameInProgress;           // +0x14
    long          BestWitnessesAheadToDate;     // +0x18
    long          MinigameInitialFramesLeft;    // +0x1c
    long          MinigameCurrentFramesLeft;    // +0x20
    long          MinigameStartingWitnessCount; // +0x24
    long          NumberOfWitnessesLeft;        // +0x28
    unsigned char _pad_0x2c[0x12];
    bool          WaitingForTerminate;          // +0x3e
    void UpdateGainedWitnessSounds();           // 0x006DEE80
    unsigned long PlaySound(const CCharString& name);   // 0x006DDCD0
    void StartDialogBox(CWideString text, unsigned long flags, CWideString button1, CWideString button2, CWideString button3);   // 0x006DD7E0
    virtual void FrameUpdate();
};

void CTCTrophy::FrameUpdate()
{
    UpdateGainedWitnessSounds();
    if (!MinigameInProgress || GetThingManager()->PausedA || GetThingManager()->PausedB)
        return;
    long framesLeft = --MinigameCurrentFramesLeft;
    if (framesLeft <= 0) {
        PlaySound(PDef->SoundOutOfTime);
        CWideString text = g_pMainGameComponent->PTextBank->GetTextBySymbol("TEXT_GUI_TROPHY_TIME_UP");
        StartDialogBox(text, 10, L"", L"", L"");
        WaitingForTerminate = true;
        return;
    }
    float fraction = 0.0f;
    long initialFrames = MinigameInitialFramesLeft;
    if (initialFrames > 0)
        fraction = (float)framesLeft / ((float)initialFrames * 2.0f);
    g_pPlayerGui->PDrawQuestInfo->UpdateMiniGame(fraction, MinigameStartingWitnessCount - NumberOfWitnessesLeft,
                                                 MinigameStartingWitnessCount);
}
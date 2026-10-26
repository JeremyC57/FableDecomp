// CTCTrophy::EndMinigame() -- while a trophy minigame is in progress: open the "game aborted" dialog box (text TEXT_GUI_TROPHY_GAME_ABORTED, dialog 0x12a, no button texts) and wait for it to terminate the game
// Leaf: EndMinigame
// Retail: Fable.exe @ 0x006DDA50, 144 bytes.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (implicit conversion temporary), CDataBank::
//          GetTextBySymbol(const CCharString&) 0x009C95E0 on the engine globals' text bank (0x013B86A0 +0x14),
//          CCharString::~CCharString 0x0099EAE0, CWideString::CWideString(const wchar_t*) 0x0099B6B0 (x3, L"" 0x0122D70C),
//          CWideString::CWideString(const CWideString&) 0x0099B720, CTCTrophy::StartDialogBox(CWideString, unsigned long,
//          CWideString, CWideString, CWideString) 0x006DD7E0, CWideString::~CWideString 0x0099B510. String 0x0125FC2C.
// Layout: Ego_r.pdb CTCTrophy with retail 4-byte lists: MinigameInProgress +0x14, AvailableWitnesses +0x34, WitnessesAchieved
//         +0x38, WaitingForConfirmation +0x3c, WaitingForScore +0x3d, WaitingForTerminate +0x3e (PDB +0x4e).
// Shape: the by-value CWideString arguments (declared copy ctor) are built in their argument slots, right to left.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
};
class CWideString { public:
    void* m_pStringData;
    CWideString(const unsigned short* text);                      // 0x0099B6B0 (wchar_t is unsigned short without /Zc:wchar_t)
    CWideString(const CWideString& other);                        // 0x0099B720
    ~CWideString();                                               // 0x0099B510
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);       // 0x009C95E0
};
struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                    // +0x14
};
extern CEngineGlobals* g_pEngineGlobals;                          // 0x013B86A0

class CTCTrophy { public:
    virtual void v00();
    unsigned char _pad_0x04[0x10];
    bool          MinigameInProgress;                             // +0x14
    unsigned char _pad_0x15[0x27];
    bool          WaitingForConfirmation;                         // +0x3c
    bool          WaitingForScore;                                // +0x3d
    bool          WaitingForTerminate;                            // +0x3e
    void StartDialogBox(CWideString text, unsigned long dialog, CWideString button1, CWideString button2,
                        CWideString button3);                     // 0x006DD7E0
    void EndMinigame();
};

void CTCTrophy::EndMinigame()
{
    if (MinigameInProgress) {
        CWideString text = g_pEngineGlobals->m_pTextBank->GetTextBySymbol("TEXT_GUI_TROPHY_GAME_ABORTED");
        StartDialogBox(text, 0x12a, L"", L"", L"");
        WaitingForTerminate = true;
    }
}
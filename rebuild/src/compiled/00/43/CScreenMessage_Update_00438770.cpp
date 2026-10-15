// CScreenMessage::Update() -- per-frame tick: one frame less to show, reveal the GUI def's characters-per-frame more characters, clamped to [0, text length]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438770, length 117 bytes (+11 int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("PLAYER_GUI_PC" @ 0x0123173C),
// CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDef<T> @ 0x0043FF30, ~CCharString @ 0x0099EAE0,
// CWideString::GetLength @ 0x0099B220. Global: the GUI def pointer @ 0x013B878C (lazily loaded), whose +0x90C is the
// number of characters revealed per frame (descriptive name).
// Shape: the lazy def load is the same inline helper as CPlayerGui::GetAbilityButton (agent153 0x00438990); the
// sum is stored before the length call; the clamp is `if (n < 0) n = 0; else if (n > len) n = len;`.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CPlayerGuiDef {                                      // the "PLAYER_GUI_PC" definition (descriptive)
    unsigned char _pad000[0x90C];
    long          MessageCharactersPerFrame;                // +0x90C
};

struct CPlayerGuiDefPointer {                               // CDefPointer<CPlayerGuiDef const>
    CPlayerGuiDef* Object;
};
extern CPlayerGuiDefPointer g_PlayerGuiDef;                 // 0x013B878C (filled on first use)

struct CDefinitionManager {
    bool GetDef(const CCharString& name, CPlayerGuiDefPointer* out);   // 0x0043FF30 (GetDef<T> instantiation)
};

struct CGameDefinitionManager {
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

__forceinline const CPlayerGuiDef* GetPlayerGuiDef()        // lazily load the GUI def into the global pointer
{
    if (!g_PlayerGuiDef.Object) {
        CCharString name("PLAYER_GUI_PC", -1);
        CGameDefinitionManager::Get().GetDef(name, &g_PlayerGuiDef);
    }
    return g_PlayerGuiDef.Object;
}

struct CWideString {
    void* m_pData;
    long GetLength() const;                                 // 0x0099B220
};

class CScreenMessage {
public:
    void Update();
private:
    CWideString Text;                                       // +0x00
    long        FramesToShowFor;                            // +0x04
    long        NumberOfCharactersShowing;                  // +0x08
    long        LinesInText;                                // +0x0C
};

void CScreenMessage::Update()
{
    --FramesToShowFor;
    NumberOfCharactersShowing += GetPlayerGuiDef()->MessageCharactersPerFrame;
    long length = Text.GetLength();
    if (NumberOfCharactersShowing < 0)
        NumberOfCharactersShowing = 0;
    else if (NumberOfCharactersShowing > length)
        NumberOfCharactersShowing = length;
}
// CPlayerGui::InitialActivate() -- first activation of the player GUI: clear the subtitle renderer and the quest-info illustrator, hide the five stat bars, size the ability- and quick-access-button alpha arrays to the GUI definition's ability-button table (all fully opaque), clear the stat-update illustrator's flag and mark the GUI as drawn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043A380, length 332 bytes (+ int3 padding), plain `ret`.
// Callees: CSubtitleRenderer::Clear(bool) 0x00492BAB, NPlayerGui::CDrawQuestInfo::Clear 0x00647319, CCharString(const
// char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::
// GetDef<T> 0x0043FF30, vector<float>::resize(n, const float&) 0x00442770. Global: the GUI def pointer 0x013B878C
// ("PLAYER_GUI_PC", 0x0123173C), as the landed CPlayerGui::GetAbilityButton 0x00438990.
// Layout (retail offsets, local view): engine/CPlayerGui.h is the PDB layout and retail sits 8 bytes lower after the
// subtitle renderer (+0x18) and 0x14 lower after the three button vectors (12-byte STLport vectors; the landed
// GetAbilityButton puts AbilityButtons at +0x254): PDrawStatUpdates +0x1A8, PDrawQuestInfo +0x1C8, AbilityButtonsAlpha
// +0x260, QuickAccessButtonsAlpha +0x26C, OldDrawGui +0x291, the stat bars PMoney/PRenown/PMorality/PExperience/PCrime
// +0x2CC/+0x2D4/+0x2DC/+0x2E4/+0x2EC (each bar's Active flag at +8; UpdateMorality/UpdateCrime read +0x2DC/+0x2EC).
// Shape: the lazily loaded def through the landed inline helper; each resize's `1.0f` reference temporary is stored
// before the def test (right-to-left argument evaluation) and the count is the signed `/0x5c` of the def's table.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct SAbilityButtonEntry {                                // one 0x5c-byte entry of the GUI def's ability-button table
    unsigned char _body[0x5c];
};

struct CPlayerGuiDef {                                      // the "PLAYER_GUI_PC" definition (descriptive)
    unsigned char        _pad000[0x7fc];
    SAbilityButtonEntry* AbilityButtons_start;              // +0x7fc  vector<SAbilityButtonEntry>
    SAbilityButtonEntry* AbilityButtons_finish;             // +0x800
    SAbilityButtonEntry* AbilityButtons_end;                // +0x804
    unsigned long NumAbilityButtons() const { return AbilityButtons_finish - AbilityButtons_start; }
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

struct CFloatVector {                                       // STLport vector<float> (12 bytes)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    void resize(unsigned long n, const float& value);       // 0x00442770
};

struct CSubtitleRenderer {
    void Clear(bool immediately);                           // 0x00492BAB
};
namespace NPlayerGui {
struct CDrawQuestInfo {
    void Clear();                                           // 0x00647319
};
struct CDrawStatUpdates {
    unsigned char _pad00[0x30];
    bool          Updating;                                 // +0x30 (descriptive)
};
struct CStatBar {
    unsigned char _pad00[8];
    bool          Active;                                   // +0x08
};
}

struct CPlayerGui {                                         // retail-offset view (see Layout)
    unsigned char                   _pad000[0x18];
    CSubtitleRenderer               SubtitleRenderer;       // +0x18
    unsigned char                   _pad019[0x1a8 - 0x19];
    NPlayerGui::CDrawStatUpdates*   PDrawStatUpdates;       // +0x1a8 (CCountedPointer data)
    unsigned char                   _pad1ac[0x1c8 - 0x1ac];
    NPlayerGui::CDrawQuestInfo*     PDrawQuestInfo;         // +0x1c8 (CCountedPointer data)
    unsigned char                   _pad1cc[0x260 - 0x1cc];
    CFloatVector                    AbilityButtonsAlpha;    // +0x260
    CFloatVector                    QuickAccessButtonsAlpha;// +0x26c
    unsigned char                   _pad278[0x291 - 0x278];
    bool                            OldDrawGui;             // +0x291
    unsigned char                   _pad292[0x2cc - 0x292];
    NPlayerGui::CStatBar*           PMoney;                 // +0x2cc
    void*                           PMoney_Info;
    NPlayerGui::CStatBar*           PRenown;                // +0x2d4
    void*                           PRenown_Info;
    NPlayerGui::CStatBar*           PMorality;              // +0x2dc
    void*                           PMorality_Info;
    NPlayerGui::CStatBar*           PExperience;            // +0x2e4
    void*                           PExperience_Info;
    NPlayerGui::CStatBar*           PCrime;                 // +0x2ec
    void InitialActivate();
};

void CPlayerGui::InitialActivate()
{
    SubtitleRenderer.Clear(false);
    PDrawQuestInfo->Clear();
    PMoney->Active = false;
    PRenown->Active = false;
    PMorality->Active = false;
    PExperience->Active = false;
    PCrime->Active = false;
    AbilityButtonsAlpha.resize(GetPlayerGuiDef()->NumAbilityButtons(), 1.0f);
    QuickAccessButtonsAlpha.resize(GetPlayerGuiDef()->NumAbilityButtons(), 1.0f);
    PDrawStatUpdates->Updating = false;
    OldDrawGui = true;
}
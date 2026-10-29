// CPlayerGui::RenderUpdateGuiTimers(float dt) -- advance the open/close fade timers of the GUI itself, of the seven stat bars (money, renown, morality, experience, crime, stat updates, auto pickups) and of the quest-info illustrator (plus its secondary fade, forced open while it is showing)
// Leaf: ?RenderUpdateGuiTimers@CPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004387F0, 406 bytes + padding (`ret 4`). Callees: CPlayerGui::UpdateGuiTimer @ 0x00437500,
// CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0, GetDef<T> @ 0x0043FF30.
// Data: the GUI def pointer @ 0x013B878C, "PLAYER_GUI_PC" @ 0x0123173C. The def's three fade times (+0x4e4/+0x4f0/+0x584) and the
// quest-info illustrator's secondary timer/flag (+0x30/+0x34) are named descriptively; EPlayerGuiMode values other than 1/2 unnamed.
// Layout: retail CPlayerGui view as the landed GetAbilityButton 0x00438990 / UpdateMorality 0x00439F20 (PDB - 8 before the subtitle
// renderer's tail, - 0x14 after it): PDrawQuestInfo +0x1c8, GuiMode +0x294, FadeInTimer +0x298, the stat bars +0x2cc..+0x2fc.
// Shape: the lazily loaded def via the landed __forceinline GetPlayerGuiDef helper; the two stat-bar times are float locals; the
// quest-info secondary fade passes the address of a local mode forced to 2.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CPlayerGuiDef { public:
    unsigned char _pad000[0x4e4];
    float         StatBarOpenTime;                          // +0x4e4 (descriptive)
    unsigned char _pad4e8[0x4f0 - 0x4e8];
    float         StatBarCloseTime;                         // +0x4f0 (descriptive)
    unsigned char _pad4f4[0x584 - 0x4f4];
    float         QuestInfoFadeTime;                        // +0x584 (descriptive)
};
struct CPlayerGuiDefPointer {
    CPlayerGuiDef* Object;
};
extern CPlayerGuiDefPointer g_PlayerGuiDef;                 // 0x013B878C
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CPlayerGuiDefPointer* out);   // 0x0043FF30
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};
__forceinline const CPlayerGuiDef* GetPlayerGuiDef()
{
    if (!g_PlayerGuiDef.Object) {
        CCharString name("PLAYER_GUI_PC", -1);
        CGameDefinitionManager::Get().GetDef(name, &g_PlayerGuiDef);
    }
    return g_PlayerGuiDef.Object;
}
enum EPlayerGuiMode { GUI_MODE_CLOSED, GUI_MODE_1, GUI_MODE_OPENING, GUI_MODE_CLOSING };
namespace NPlayerGui {
class CStatBase { public:                                          // CStatBase (Ego_r.pdb offsets)
    unsigned char  _pad00[0x28];
    EPlayerGuiMode GuiMode;                                 // +0x28
    float          FadeInTimer;                             // +0x2c
};
class CDrawQuestInfo { public:
    unsigned char  _pad00[0x28];
    EPlayerGuiMode GuiMode;                                 // +0x28
    float          FadeInTimer;                             // +0x2c
    float          SecondaryFadeTimer;                      // +0x30 (descriptive)
    bool           ShowingSecondary;                        // +0x34 (descriptive)
};
}
template<class T> class CCountedPointer { public:
    T*    Ptr;
    void* Count;
    T* operator->() const { return Ptr; }
};
class CPlayerGui { public:                                         // retail-offset view (PDB - 8 / - 0x14)
    unsigned char _pad000[0x1c8];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;   // +0x1c8 (PDB +0x1d0)
    unsigned char _pad1d0[0x294 - 0x1d0];
    EPlayerGuiMode GuiMode;                                 // +0x294 (PDB +0x2a8)
    float          FadeInTimer;                             // +0x298
    unsigned char _pad29c[0x2cc - 0x29c];
    CCountedPointer<NPlayerGui::CStatBase> PMoney;          // +0x2cc
    CCountedPointer<NPlayerGui::CStatBase> PRenown;         // +0x2d4
    CCountedPointer<NPlayerGui::CStatBase> PMorality;       // +0x2dc
    CCountedPointer<NPlayerGui::CStatBase> PExperience;     // +0x2e4
    CCountedPointer<NPlayerGui::CStatBase> PCrime;          // +0x2ec
    CCountedPointer<NPlayerGui::CStatBase> PStatUpdates;    // +0x2f4
    CCountedPointer<NPlayerGui::CStatBase> PAutoPickups;    // +0x2fc
    void UpdateGuiTimer(float dt, float openTime, float closeTime, EPlayerGuiMode* mode, float* timer);   // 0x00437500
    void RenderUpdateGuiTimers(float dt);
};

void CPlayerGui::RenderUpdateGuiTimers(float dt)
{
    UpdateGuiTimer(dt, 0.5f, 0.5f, &GuiMode, &FadeInTimer);
    const CPlayerGuiDef* def = GetPlayerGuiDef();
    float openTime = def->StatBarOpenTime;
    float closeTime = def->StatBarCloseTime;
    UpdateGuiTimer(dt, openTime, closeTime, &PMoney->GuiMode, &PMoney->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PRenown->GuiMode, &PRenown->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PMorality->GuiMode, &PMorality->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PExperience->GuiMode, &PExperience->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PCrime->GuiMode, &PCrime->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PStatUpdates->GuiMode, &PStatUpdates->FadeInTimer);
    UpdateGuiTimer(dt, openTime, closeTime, &PAutoPickups->GuiMode, &PAutoPickups->FadeInTimer);
    if (PDrawQuestInfo->GuiMode != GUI_MODE_1) {
        UpdateGuiTimer(dt, def->QuestInfoFadeTime, def->QuestInfoFadeTime, &PDrawQuestInfo->GuiMode, &PDrawQuestInfo->FadeInTimer);
        if (PDrawQuestInfo->ShowingSecondary) {
            EPlayerGuiMode mode = GUI_MODE_OPENING;
            UpdateGuiTimer(dt, def->QuestInfoFadeTime, def->QuestInfoFadeTime, &mode, &PDrawQuestInfo->SecondaryFadeTimer);
        }
    }
}
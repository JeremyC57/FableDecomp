// CPlayerGui::Update(bool paused) -- per-frame GUI tick: react to a DrawGui toggle (open/close the GUI mode, clearing the GUI on close), update the hero-information screens and subtitles, and when not paused the stat bars and per-frame illustrators; then drop expired script screen texts and age the screen messages, discarding finished ones and stopping once the def's line budget is filled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043A080, length 544 bytes, `ret 4`.
// Callees: CPlayerGui::ClearGui 0x00438230, NHeroInformationScreens::CManager::Update 0x0066E8EE, CSubtitleRenderer::Update
// 0x004930D7, CPlayerGui::UpdateMoney 0x00439BB0 / UpdateRenown 0x00439E00 / UpdateMorality 0x00439F20 / UpdateCrime
// 0x004385F0, CPlayerManager::GetMainPlayer 0x00449970, CBaseIntelligentPointer::GetPItem 0x00A01B50 (the player's
// character pointer +0x34), NPlayerGui::CDrawQuestInfo / CDrawMeleeActionButtonEffects / CDrawPerceivers / CDrawDamage /
// CDrawStatUpdates / CDrawAutoPickups / CDrawGuildSeal / CDrawQuickAccessIcons ::Update (0x0064A32E, 0x0064BE45, 0x0064A1AE,
// 0x00644CE2, 0x0064A060, 0x00643EB1, 0x0065196C, 0x0064D6A8), GetWorldFrame 0x0049D870, ~CCharString 0x0099EAE0,
// ~CWideString 0x0099B510, free 0x00BFEA14 (STLport list node release), CScreenMessage::Update 0x00438770, the lazily
// loaded "PLAYER_GUI_PC" def (0x0099EBF0 / 0x0044C6B0 / 0x0043FF30; global 0x013B878C).
// Layout (retail; engine/CPlayerGui.h is the PDB view): the illustrator counted pointers sit 8 below the PDB (narrower
// CSubtitleRenderer, as the landed UpdateRenown 0x00439E00), DrawGui/OldDrawGui/GuiMode/FadeInTimer/ForcedClosed 0x14 below
// (+0x290/+0x291/+0x294/+0x298/+0x29c: three 12-byte vectors before them), ScriptScreenText +0x324, ScreenMessages +0x328
// and PHeroInfoScreensManager +0x32c (4-byte STLport lists, as the landed AddScreenMessage 0x00438AA0). The def's screen
// message line budget +0x904 is named descriptively.
// Shape: STLport list loops over the sentinel's _M_next with an inline erase (next/prev locals, implicit element dtor,
// free(node)); the player's character is an inline GetPItem accessor on the CPlayer intelligent pointer +0x34.
#include <stdlib.h>
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
class CPlayerGuiDef { public:
    unsigned char _pad000[0x904];
    long          MaxScreenMessageLines;                    // +0x904 (descriptive)
};
struct CPlayerGuiDefPointer { CPlayerGuiDef* Object; };
extern CPlayerGuiDefPointer g_PlayerGuiDef;                 // 0x013B878C
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CPlayerGuiDefPointer* out);   // 0x0043FF30
};
class CGameDefinitionManager { public:
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};
__forceinline const CPlayerGuiDef* GetPlayerGuiDef()
{
    if (!g_PlayerGuiDef.Object) {
        CCharString name("PLAYER_GUI_PC", -1);
        CGameDefinitionManager::Get().GetDef(name, &g_PlayerGuiDef);
    }
    return g_PlayerGuiDef.Object;
}
unsigned long GetWorldFrame();                              // 0x0049D870

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_statusFlags;                            // +0x91 (bit 0: dead)
};
struct CBaseIntelligentPointer {
    CThing* GetPItem() const;                               // 0x00A01B50
};
class CPlayer { public:
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer PCharacter;                     // +0x34
    CThing* GetPlayerCharacter() const { return PCharacter.GetPItem(); }
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                               // 0x00449970
};
class CSubtitleRenderer { public:
    void Update(bool paused);                               // 0x004930D7
};
namespace NHeroInformationScreens { class CManager { public: void Update(); }; }   // 0x0066E8EE
namespace NPlayerGui {
class CDrawStatUpdates { public: void Update(); };                 // 0x0064A060
class CDrawAutoPickups { public: void Update(); };                 // 0x00643EB1
class CDrawPerceivers { public: void Update(); };                  // 0x0064A1AE
class CDrawDamage { public: void Update(); };                      // 0x00644CE2
class CDrawQuestInfo { public: void Update(); };                   // 0x0064A32E
class CDrawGuildSeal { public: void Update(); };                   // 0x0065196C
class CDrawQuickAccessIcons { public: void Update(); };            // 0x0064D6A8
class CDrawMeleeActionButtonEffects { public: void Update(); };    // 0x0064BE45
class CScriptScreenText { public:
    CWideString   Text;                                     // +0x00
    float         VPos[2];                                  // +0x04
    unsigned long Colour;                                   // +0x0c
    CCharString   FontName;                                 // +0x10
    unsigned long FrameAdded;                               // +0x14
};
class CScreenMessage { public:
    CWideString Text;                                       // +0x00
    long        FramesToShowFor;                            // +0x04
    long        NumberOfCharactersShowing;                  // +0x08
    long        LinesInText;                                // +0x0c
    void Update();                                          // 0x00438770
};
}
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CList {                           // STLport list (sentinel pointer)
    SListNode<T>* _M_node;
    SListNode<T>* erase(SListNode<T>* pos)
    {
        SListNode<T>* next = pos->_M_next;
        SListNode<T>* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        pos->_M_data.~T();
        free(pos);
        return next;
    }
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};

class CPlayerGui { public:
    void*                                                   __vftable;
    void*                                                   World;                      // +0x04
    CPlayerManager*                                         PlayerManager;              // +0x08
    unsigned char                                           _pad00c[0x18 - 0xc];
    CSubtitleRenderer                                       SubtitleRenderer;           // +0x18
    unsigned char                                           _pad019[0x1a8 - 0x19];
    CCountedPointer<NPlayerGui::CDrawStatUpdates>           PDrawStatUpdates;           // +0x1a8
    CCountedPointer<NPlayerGui::CDrawAutoPickups>           PDrawAutoPickups;           // +0x1b0
    CCountedPointer<NPlayerGui::CDrawPerceivers>            PDrawPerceivers;            // +0x1b8
    CCountedPointer<NPlayerGui::CDrawDamage>                PDrawDamage;                // +0x1c0
    CCountedPointer<NPlayerGui::CDrawQuestInfo>             PDrawQuestInfo;             // +0x1c8
    CCountedPointer<NPlayerGui::CDrawGuildSeal>             PDrawGuildSeal;             // +0x1d0
    CCountedPointer<NPlayerGui::CDrawQuickAccessIcons>      PDrawQuickAccessIcons;      // +0x1d8
    unsigned char                                           _pad1e0[0x220 - 0x1e0];
    CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects> PDrawMeleeActionButtonEffects;   // +0x220
    unsigned char                                           _pad228[0x290 - 0x228];
    bool                                                    DrawGui;                    // +0x290
    bool                                                    OldDrawGui;                 // +0x291
    long                                                    GuiMode;                    // +0x294
    float                                                   FadeInTimer;                // +0x298
    bool                                                    ForcedClosed;               // +0x29c
    unsigned char                                           _pad29d[0x324 - 0x29d];
    CList<NPlayerGui::CScriptScreenText>                    ScriptScreenText;           // +0x324
    CList<NPlayerGui::CScreenMessage>                       ScreenMessages;             // +0x328
    NHeroInformationScreens::CManager*                      PHeroInfoScreensManager;    // +0x32c
    void ClearGui();                                        // 0x00438230
    void UpdateMoney();                                     // 0x00439BB0
    void UpdateRenown();                                    // 0x00439E00
    void UpdateMorality();                                  // 0x00439F20
    void UpdateCrime();                                     // 0x004385F0
    void Update(bool paused);
};

void CPlayerGui::Update(bool paused)
{
    if (DrawGui != OldDrawGui) {
        if (DrawGui && GuiMode == 1) {
            if (!ForcedClosed)
                GuiMode = 2;
        } else if (!DrawGui && (GuiMode == 0 || GuiMode == 2)) {
            GuiMode = 1;
            FadeInTimer = 0.0f;
            ClearGui();
        }
        OldDrawGui = DrawGui;
    }
    PHeroInfoScreensManager->Update();
    SubtitleRenderer.Update(paused);
    if (!paused) {
        UpdateMoney();
        UpdateRenown();
        UpdateMorality();
        UpdateCrime();
        CThing* hero = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
        if (hero && !(hero->m_statusFlags & 1))
            PDrawQuestInfo->Update();
        PDrawMeleeActionButtonEffects->Update();
        PDrawPerceivers->Update();
        PDrawDamage->Update();
        PDrawStatUpdates->Update();
        PDrawAutoPickups->Update();
        PDrawGuildSeal->Update();
        PDrawQuickAccessIcons->Update();
    }
    SListNode<NPlayerGui::CScriptScreenText>* text = ScriptScreenText._M_node->_M_next;
    while (text != ScriptScreenText._M_node) {
        if (text->_M_data.FrameAdded < GetWorldFrame() - 1)
            text = ScriptScreenText.erase(text);
        else
            text = text->_M_next;
    }
    long lines = 0;
    SListNode<NPlayerGui::CScreenMessage>* message = ScreenMessages._M_node->_M_next;
    while (message != ScreenMessages._M_node) {
        NPlayerGui::CScreenMessage& m = message->_M_data;
        m.Update();
        if (m.FramesToShowFor > 0) {
            lines += m.LinesInText;
            if (lines >= GetPlayerGuiDef()->MaxScreenMessageLines)
                break;
            message = message->_M_next;
        } else {
            message = ScreenMessages.erase(message);
        }
    }
}
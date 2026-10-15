// CSubtitleRenderer::IsWaitingForClickPast() const -- true while text is shown and either a forced-wait flag (0x48) is set, or the click-past flag (2) is set outside a cutscene with subtitles enabled in the user profile
// Leaf: IsWaitingForClickPast
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DB46, 61 bytes. Callee: CTBaseSingleton<CUserProfileManager>::Get @
// 0x0040D2A0 (no arguments, the manager in eax). The profile accessor is the landed family's inline getter
// (UseNewProfileValues +0xC selects SubtitlingEnabledNewProfile +0xCD over SubtitlingEnabled +0xD; PDB +0xD1 / +0xD).
// Layout: engine/CSubtitleRenderer.h is the PDB view (Ego_r: 16-byte vectors, a 0x18-byte deque); retail has the VC6
// containers (0x28-byte deque, 12-byte vectors), so DisplayFlags sits at +0x7C (PDB +0x6C) and the cutscene bools at
// +0xDC.. (PDB +0xE4..). The header disagrees with the bytes, so the class is modelled locally with the PDB names.
// Shape: DisplayFlags is loaded once (`mov eax,[ecx+7Ch] ; test al,48h ; test al,2`); the tests are NESTED ifs with
// `return true` inside and one trailing `return false`, so the failures share the `xor al,al` exit and the profile test
// falls into the shared `mov al,1` (flat early returns end in `setne al`); `cmp byte ptr [..],0` tests = a size TU
// (#pragma optimize("s", on)).
#pragma optimize("s", on)
class CWideString { public: void* PStringData; };

struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
class CFontBank;
class CGraphicDataBank;

struct CFadeInFadeOutBase {
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0C
    bool       Opening;                            // +0x0D
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
};

struct CRetailVector { void* m_pFirst; void* m_pLast; void* m_pEnd; };   // VC6 vector: 12 bytes

class CSubtitleRenderer {
public:
    void*              __vftable;                  // +0x00
    C2DBoxF            VWindow;                    // +0x04
    C2DBoxF            VTelevisionWindow;          // +0x14
    long               TextCharactersShowing;      // +0x24
    bool               TextGroupHasEntriesRemaining; // +0x28
    long               TextGroupIndex;             // +0x2C
    CWideString        TextGroupTag;               // +0x30
    unsigned char      TextInfoQueue[0x28];        // +0x34 (VC6 deque<CSetTextInfo>)
    CWideString        Text;                       // +0x5C
    CWideString        NameText;                   // +0x60
    CWideString        Button1Text;                // +0x64
    CWideString        Button2Text;                // +0x68
    CWideString        Button3Text;                // +0x6C
    CWideString        Button1OverrideText;        // +0x70
    CWideString        Button2OverrideText;        // +0x74
    unsigned char      Button1Alpha;               // +0x78
    unsigned char      Button2Alpha;               // +0x79
    unsigned long      DisplayFlags;               // +0x7C
    CFontBank*         PFont;                      // +0x80
    CGraphicDataBank*  Graphics;                   // +0x84
    float              MaxDisplayTime;             // +0x88
    float              TextTimer;                  // +0x8C
    long               PlayerModeAdded;            // +0x90
    CRetailVector      PrimitiveHandles;           // +0x94
    CRetailVector      ButtonPrimitiveHandles;     // +0xA0
    CRetailVector      EmbeddedPrimitiveHandles;   // +0xAC
    CRetailVector      EmbeddedGraphics;           // +0xB8
    CRetailVector      EmbeddedHighlights;         // +0xC4
    CRetailVector      EmbeddedHighlightActionNames; // +0xD0
    bool               PausedGame;                 // +0xDC
    bool               AllCharactersDisplayed;     // +0xDD
    bool               InCutscene;                 // +0xDE
    bool               CutsceneSkipped;            // +0xDF
    bool               CutsceneSkippable;          // +0xE0
    bool               CutsceneSkippableWhilePaused; // +0xE1
    bool               CutsceneActionMode;         // +0xE2
    bool               Button1TextOverridden;      // +0xE3
    bool               Button2TextOverridden;      // +0xE4
    bool               DisplayingText;             // +0xE5
    bool               UpdateWhilePaused;          // +0xE6
    bool               UpdateTimer;                // +0xE7
    CFadeInFadeOutBase SkipCutsceneTimer;          // +0xE8
    CFadeInFadeOutBase ButtonPulseTimer;           // +0x104
    bool IsWaitingForClickPast() const;
};

class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;             // +0x0C
    bool          SubtitlingEnabled;               // +0x0D
    unsigned char _pad0E[0xCD - 0x0E];
    bool          SubtitlingEnabledNewProfile;     // +0xCD (PDB +0xD1)
    static CUserProfileManager& Get();             // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
    bool IsSubtitlingEnabled() const
    {
        if (UseNewProfileValues)
            return SubtitlingEnabledNewProfile;
        return SubtitlingEnabled;
    }
};

bool CSubtitleRenderer::IsWaitingForClickPast() const
{
    if (DisplayingText) {
        if (DisplayFlags & 0x48)
            return true;
        if (DisplayFlags & 2) {
            if (!InCutscene) {
                if (CUserProfileManager::Get().IsSubtitlingEnabled())
                    return true;
            }
        }
    }
    return false;
}

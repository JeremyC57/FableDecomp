// NTCScriptedControl::CActionTalkToThing::Update() -- skip to the end of the current line once the player has clicked it away (subtitles on: the renderer stopped displaying it; subtitles off: a type-0x11 message event arrived), then run the base talk update
// Leaf: ?Update@CActionTalkToThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007173D0, 126 bytes (+2 int3 padding); vtable 0x012659C8 slot 1.
// FableWin.pdb: ?Update@CActionTalkToThing@NTCScriptedControl@@UAEXXZ.
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (no arguments, the manager in eax) with the landed
//          family's inline IsSubtitlingEnabled() (as CSubtitleRenderer::IsWaitingForClickPast @ 0x0048DB46),
//          CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> @ 0x004AB8B0 (as the landed
//          CGameScriptInterface::CheckForCameraMessage @ 0x006E7920), CActionTalkBase::Update @ 0x00716DE0.
// Data: g_pPlayerGui @ 0x013B8790 (SubtitleRenderer +0x18, its retail DisplayingText +0xE5 = GUI +0xFD per the landed
//          0x0048DB46 layout), g_pMainGameComponent @ 0x013B86A0 (PWorld +0x24; CWorld PMessageEventManager +0x60).
// Layout: retail CActionTalkBase UpdateCounter +0x18, GameTurnDuration +0x48, SkippedCurrentLine +0x8F (Ego_r.pdb +0x93:
//          the debug CArray is 4 bytes larger); CUserProfileManager UseNewProfileValues +0xC, SubtitlingEnabled +0xD,
//          SubtitlingEnabledNewProfile +0xCD.
// Shape: the skip is an inline helper called from both arms; VC tail-duplicates the base call into the first arm. The
//          message manager goes into a named local first: the inline filter temporary in the call chain stores its 0x11
//          before the `push` (retail stores it after).
// Layout (supersedes any local-model note above): engine/CSubtitleRenderer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CSubtitleRenderer CSubtitleRendererDonor
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CThing;
class CMessageEvent;
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
};
template<class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CWorld { public:
    void*         __vftable;
    unsigned char _pad04[0x60 - 0x04];
    CCountedPointer<CMessageEventManager> PMessageEventManager;   // +0x60
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                          // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0

class CSubtitleRenderer : public CSubtitleRendererDonor {
 public:
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;            // +0x18
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790

class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;             // +0x0C
    bool          SubtitlingEnabled;               // +0x0D
    unsigned char _pad0E[0xCD - 0x0E];
    bool          SubtitlingEnabledNewProfile;     // +0xCD (PDB +0xD1)
    static CUserProfileManager& Get();             // 0x0040D2A0
    bool IsSubtitlingEnabled() const
    {
        if (UseNewProfileValues)
            return SubtitlingEnabledNewProfile;
        return SubtitlingEnabled;
    }
};
class CTCScriptedControl;

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionTalkBase : public CActionBase {
public:
    virtual void Update();                         // 0x00716DE0
    unsigned char _pad0c[0x18 - 0x0c];             // PTargetThing (CScriptThing)
    long          UpdateCounter;                   // +0x18
    unsigned char _pad1c[0x48 - 0x1c];             // line times, Messages, CurrentMessageIndex, Conversation
    long          GameTurnDuration;                // +0x48
    unsigned char _pad4c[0x8f - 0x4c];             // tag indices .. ParentIsDemonDoor, GettingUp
    bool          SkippedCurrentLine;              // +0x8F
    void SkipCurrentLine()
    {
        UpdateCounter = GameTurnDuration;
        SkippedCurrentLine = true;
    }
};

class CActionTalkToThing : public CActionTalkBase {
public:
    virtual void Update();
    bool AddedDuringCutscene;                      // +0x90
    bool AskingHeroQuestion;                       // +0x91
    bool DisplayOverScreenFade;                    // +0x92
};

void CActionTalkToThing::Update()
{
    if (CUserProfileManager::Get().IsSubtitlingEnabled()) {
        if (!g_pPlayerGui->SubtitleRenderer.DisplayingText)
            SkipCurrentLine();
    } else {
        CMessageEventManager* manager = g_pMainGameComponent->PWorld->PMessageEventManager.Data;
        if (manager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x11)))
            SkipCurrentLine();
    }
    CActionTalkBase::Update();
}
}
// NTCScriptedControl::CActionTalkToThing::OnActivate() -- a line queued during a cutscene that has since been skipped finishes at once; otherwise the base talk activation runs
// Leaf: ?OnActivate@CActionTalkToThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717920, 35 bytes (+13 int3 padding); vtable 0x012659C8 slot 2.
// Slot names: slot 2 = OnActivate (CActionTalkBase slot 2 is 0x00717710; FableWin.pdb has
//          ?OnActivate@CActionTalkToThing@NTCScriptedControl@@UAEXXZ and ?OnActivate@CActionTalkBase@...).
// Callees: CActionTalkBase::OnActivate @ 0x00717710 (tail `jmp`). Data: g_pPlayerGui @ 0x013B8790 (as the landed
//          CTCBoastUI / CThingAICreature rows).
// Layout: retail CActionTalkBase is 0x90 bytes (Ego_r.pdb 0x98: the debug CArray is 4 bytes larger and the tail pads to 8),
//          so CActionTalkToThing::AddedDuringCutscene (PDB +0x98) is retail +0x90. The player-GUI byte +0xF7 sits in the
//          embedded CSubtitleRenderer (+0x18) at +0xDF = CutsceneSkipped (retail layout of the landed
//          CSubtitleRenderer::IsWaitingForClickPast @ 0x0048DB46; PDB +0xE7).
// Shape: `&&` of the two byte tests to the Terminated store and `ret`; the else arm is the qualified base call, emitted as
//          a tail jump.
class CPlayerGui { public:
    unsigned char _pad00[0xf7];
    bool          CutsceneSkipped;                 // +0xF7 SubtitleRenderer.CutsceneSkipped
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
class CTCScriptedControl;

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    virtual void OnActivate();                     // slot 2
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionTalkBase : public CActionBase {
public:
    virtual void OnActivate();                     // 0x00717710
    unsigned char _pad0c[0x90 - 0x0c];             // PTargetThing .. SkippedCurrentLine
};

class CActionTalkToThing : public CActionTalkBase {
public:
    virtual void OnActivate();
    bool AddedDuringCutscene;                      // +0x90
    bool AskingHeroQuestion;                       // +0x91
    bool DisplayOverScreenFade;                    // +0x92
};

void CActionTalkToThing::OnActivate()
{
    if (AddedDuringCutscene && g_pPlayerGui->CutsceneSkipped)
        Terminated = true;
    else
        CActionTalkBase::OnActivate();
}
}
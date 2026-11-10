// CWorld::EASkipCutScene(const CGameEvent*) -- game-event action: if a skippable cutscene is showing, play the text-box button sound, post the skip-cutscene message and clear the subtitles, marking the cutscene skipped
// Leaf: ?EASkipCutScene@CWorld@@QAEXPBVCGameEvent@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00629F50, 255 bytes, `ret 4`. PDB ?EASkipCutScene@CWorld@@QAEXPBVCGameEvent@@@Z.
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h PSoundManager, slot 9 Get2DEmitter (the 2-D CASoundEmitter,
//   slot 4 PlaySoundEvent) and engine/CSoundInit.h under FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR (the expanded constructor); was a local
//   CSoundSystem / CSoundPlayer / CSoundInit model.
// Callees: CSubtitleRenderer::IsCutsceneSkippable() const 0x0048DE44 (landed), __fastcall PeekGuiDef() 0x00438570,
//   the sound system 0x013B8394 slot 9 (+0x24, its sound player) and the player's PlaySoundEvent (slot 4, +0x10; landed
//   CThingSoundEmitter::PlaySample shows the slot table), CWorld::GetSampleBank() const 0x0049D9B0 (Component+0x10;
//   label "CPixelShader::CountReferences" is wrong), CMessageEventManager::AddMessage 0x00687540, CSubtitleRenderer::
//   Clear(bool) 0x00492BAB. Globals: (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790 (subtitle renderer at +0x18), CMainGameComponent::GetConstantFPS() 0x01375550.
// Layout: CSubtitleRenderer InCutscene +0xDE, CutsceneSkipped +0xDF, CutsceneSkippable +0xE0 (Ego_r.pdb 0xE6..0xE8 less
//   the 4 bytes of each of the two 12-byte retail vectors before them); CPlayerGuiDef text-box button sound criteria
//   +0x5EC = TextBoxButtonBSoundCriteria (Ego_r.pdb +0x628 less 0x3C: TextBoxSkipCutsceneGraphic +0x5E4 in the landed
//   CPlayerGuiDef ctors, button A +0x5E8 and B +0x5EC both read by CWorld::EAAnswerQuestion 0x00629DE0); CWorld
//   PMessageEventManager +0x60 (engine/CWorld.h).
// Shape: the CSoundInit default ctor of the landed PlaySoundAtPos (list + body stores) with GainFaderBits = 0x8010; the
//   sound player's vtable is spilled across the GetSampleBank() call that supplies the first argument; the message
//   position is a zero C3DVector temporary.
// Layout (supersedes any local-model note above): engine/CSubtitleRenderer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CSubtitleRenderer CSubtitleRendererDonor
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CCharString { public: void* PStringData; };
#include "engine/C3DVector.h"
class CThing;
class CGameEvent;
class CASoundBank;
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"
class CPlayerGuiDef { public:
    unsigned char _pad[0x5ec];
    CCharString   TextBoxButtonBSoundCriteria;    // +0x5EC
};
class CSubtitleRenderer : public CSubtitleRendererDonor {
 public:
    bool IsCutsceneSkippable() const;              // 0x0048DE44
    void Clear(bool all);                          // 0x00492BAB
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();            // 0x00438570
    unsigned char     _pad[0x18];
    CSubtitleRenderer SubtitleRenderer;            // +0x18
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
class CWorld { public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60 (CCountedPointer data word)
    CASoundBank& GetSampleBank() const;            // 0x0049D9B0
    void EASkipCutScene(const CGameEvent* event);
};
void CWorld::EASkipCutScene(const CGameEvent* event)
{
    CSubtitleRenderer& subtitles = CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer;
    if (subtitles.InCutscene && subtitles.IsCutsceneSkippable()) {
        const CPlayerGuiDef* def = &CPlayerGui::PeekGuiDef();
        CSoundInit init;
        init.GainFaderBits = 0x8010;
        PSoundManager->Get2DEmitter().PlaySoundEvent(GetSampleBank(), def->TextBoxButtonBSoundCriteria, true, init, false);
        PMessageEventManager->AddMessage(0x32, CMainGameComponent::GetConstantFPS(), C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
        subtitles.Clear(false);
        subtitles.CutsceneSkipped = true;
        subtitles.CutsceneSkippable = false;
    }
}
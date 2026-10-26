// CWorld::EASkipCutScene(const CGameEvent*) -- game-event action: if a skippable cutscene is showing, play the text-box button sound, post the skip-cutscene message and clear the subtitles, marking the cutscene skipped
// Leaf: ?EASkipCutScene@CWorld@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00629F50, 255 bytes, `ret 4`. PDB ?EASkipCutScene@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CSubtitleRenderer::IsCutsceneSkippable() const 0x0048DE44 (landed), __fastcall PeekGuiDef() 0x00438570,
//   the sound system 0x013B8394 slot 9 (+0x24, its sound player) and the player's PlaySoundEvent (slot 4, +0x10; landed
//   CThingSoundEmitter::PlaySample shows the slot table), CWorld::GetSampleBank() const 0x0049D9B0 (Component+0x10;
//   label "CPixelShader::CountReferences" is wrong), CMessageEventManager::AddMessage 0x00687540, CSubtitleRenderer::
//   Clear(bool) 0x00492BAB. Globals: g_pPlayerGui 0x013B8790 (subtitle renderer at +0x18), g_framesPerSecond 0x01375550.
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CThing;
class CGameEvent;
class CASoundBank;
class CSoundInit { public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
        GainFaderBits = 0;
    }
};
struct CSoundPlayer {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);   // slot 4 (+0x10)
};
struct CSoundSystem {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual CSoundPlayer* GetSoundPlayer();       // slot 9 (+0x24)
};
extern CSoundSystem* g_pSoundSystem;               // 0x013B8394
class CPlayerGuiDef { public:
    unsigned char _pad[0x5ec];
    CCharString   TextBoxButtonBSoundCriteria;    // +0x5EC
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
class CSubtitleRenderer : public CSubtitleRendererDonor {
 public:
    bool IsCutsceneSkippable() const;              // 0x0048DE44
    void Clear(bool all);                          // 0x00492BAB
};
class CPlayerGui { public:
    unsigned char     _pad[0x18];
    CSubtitleRenderer SubtitleRenderer;            // +0x18
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
extern unsigned long g_framesPerSecond;            // 0x01375550
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
    CSubtitleRenderer& subtitles = g_pPlayerGui->SubtitleRenderer;
    if (subtitles.InCutscene && subtitles.IsCutsceneSkippable()) {
        CPlayerGuiDef* def = PeekGuiDef();
        CSoundInit init;
        init.GainFaderBits = 0x8010;
        g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(GetSampleBank(), def->TextBoxButtonBSoundCriteria, true, init, false);
        PMessageEventManager->AddMessage(0x32, g_framesPerSecond, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
        subtitles.Clear(false);
        subtitles.CutsceneSkipped = true;
        subtitles.CutsceneSkippable = false;
    }
}
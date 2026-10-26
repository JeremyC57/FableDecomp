// CWorld::EAAnswerQuestion(const CGameEvent* event) -- game-event action: read the answer from the event, post message 0x10 carrying it after 50 frames, and while the subtitle box is displaying text play the A (answer 1) or B button sound and clear the subtitles
// Leaf: ?EAAnswerQuestion@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00629DE0, 324 bytes (+ int3 padding to 336), thiscall `ret 4`, /GS (cookie
//   0x0139C8A8, __security_check_cookie 0x00BFE9F9). Ego_r.pdb name set: ?EAAnswerQuestion@CWorld@@QAEXPBVCGameEvent@@@Z;
//   the donor build orders EAAnswerQuestion -> EAClickPastText -> EASkipCutScene and retail has 0x00629DE0 -> the
//   17-byte subtitle forwarder 0x00629F30 -> the landed EASkipCutScene 0x00629F50. The static event table stores it at
//   0x013B99C8 (initialiser 0x0121C57F). Catalog label "CPixelShader_OnTextureUpdateEvent" is propagated and wrong.
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CMessageEventManager::AddMessage 0x00687540
//   (on CWorld +0x60), __fastcall PeekGuiDef() 0x00438570, the sound system 0x013B8394 slot 9 (+0x24, GetSoundPlayer)
//   and the player's PlaySoundEvent (slot 4, +0x10), CWorld::GetSampleBank() const 0x0049D9B0,
//   CSubtitleRenderer::Clear(bool) 0x00492BAB. Globals: g_pPlayerGui 0x013B8790 (subtitle renderer at +0x18).
// Layout: CSubtitleRenderer DisplayingText +0xE5 (PDB +0xED, less the two 4-byte vector shrinks); CPlayerGuiDef
//   TextBoxButtonA/BSoundCriteria +0x5E8/+0x5EC (PDB +0x624/+0x628).
// Shape: as the landed EASkipCutScene (CSoundInit default ctor + GainFaderBits 0x8010, the sound player's vtable spilled
//   across GetSampleBank()), with the answer read into the dead event-argument slot and the zero message position a
//   C3DVector temporary. The two PlaySoundEvent statements of the if/else share their tail (`mov ecx,edi; call [eax+10]`);
//   `mov eax,1` serves both the CSoundInit stores and the `answer == 1` compare.
// Layout (supersedes any local-model note above): engine/CSubtitleRenderer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CSubtitleRenderer CSubtitleRendererDonor
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CCharString { public: void* PStringData; };
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CThing;
class CASoundBank;
class CSoundInit {
public:
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
class CSoundPlayer {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);   // slot 4 (+0x10)
};
class CSoundSystem {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual CSoundPlayer* GetSoundPlayer();           // slot 9 (+0x24)
};
extern CSoundSystem* g_pSoundSystem;                   // 0x013B8394
class CPlayerGuiDef {
public:
    unsigned char _pad[0x5e8];
    CCharString   TextBoxButtonASoundCriteria;        // +0x5E8 (PDB +0x624)
    CCharString   TextBoxButtonBSoundCriteria;        // +0x5EC (PDB +0x628)
};
CPlayerGuiDef* __fastcall PeekGuiDef();                // 0x00438570
class CSubtitleRenderer : public CSubtitleRendererDonor {
public:
    void Clear(bool all);                              // 0x00492BAB
};
class CPlayerGui {
public:
    unsigned char     _pad[0x18];
    CSubtitleRenderer SubtitleRenderer;                // +0x18
};
extern CPlayerGui* g_pPlayerGui;                       // 0x013B8790
class CMessageEventManager {
public:
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
class CMemoryBuffer {
public:
    unsigned char Data[0x20];                          // +0x00
    long          Length;                              // +0x20
    long          ReadPos;                             // +0x24
    long          WritePos;                            // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);       // 0x009F18F0
    __forceinline long ReadLong() { long value; Read(4, &value); return value; }
};
class CGameEvent {
public:
    long          Type;                                // +0x00
    signed char   Player;                              // +0x04
    unsigned char Data[32];                            // +0x05
    unsigned char EndPos;                              // +0x25
    bool          Valid;                               // +0x26
    bool          Replacement;                         // +0x27
    long GetData(CMemoryBuffer& buffer) const;         // 0x009F17A0
};
class CWorld {
public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;        // +0x60 (CCountedPointer data word)
    CASoundBank& GetSampleBank() const;                // 0x0049D9B0
    void EAAnswerQuestion(const CGameEvent* event);
};

void CWorld::EAAnswerQuestion(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    long answer = buffer.ReadLong();
    PMessageEventManager->AddMessage(0x10, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, answer);
    CSubtitleRenderer& subtitles = g_pPlayerGui->SubtitleRenderer;
    if (subtitles.DisplayingText) {
        CPlayerGuiDef* def = PeekGuiDef();
        CSoundInit init;
        init.GainFaderBits = 0x8010;
        if (answer == 1)
            g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(GetSampleBank(), def->TextBoxButtonASoundCriteria, true, init, false);
        else
            g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(GetSampleBank(), def->TextBoxButtonBSoundCriteria, true, init, false);
        subtitles.Clear(false);
    }
}
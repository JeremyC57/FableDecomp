// CThingSoundEmitter::SetSpecificSamplePitch(unsigned long, float) -- set the pitch of one playing sample: forwards to the emitter's sound object (+0x98), CASoundEmitter vtable slot 6
// Leaf: ?SetSpecificSamplePitch@CThingSoundEmitter@@QAEXKM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836790, 11 bytes: `mov ecx,[ecx+98h] ; mov eax,[ecx] ; jmp dword ptr [eax+18h]` (a sibling call:
//   the sample id and the pitch stay in place). Unlike StopSpecificSample 0x008368D0 there is no game-state / Dead guard.
// Identity: FableWin.pdb ?SetSpecificSamplePitch@CThingSoundEmitter@@QAEXKM@Z (16-byte body there too). FableWin orders the CThingSoundEmitter
//   TU .., StopSpecificSample, ReleaseLoop, WorldUpdate, RenderUpdate, SetSpecificSamplePitch, SetSpecificSampleVolume; retail
//   0x00836790 / 0x008367A0 sit together, and the only caller is CTCSoundPlayer::SetSpecificSamplePitch 0x0066AFDB (the landed binding already
//   spells it this way). The catalog label "CScriptThing::operator==" is propagated across the byte-twin shape and is wrong.
// Sound object: created by PSoundManager's slot 4 (CThingSoundEmitter::OnCreate 0x00836740) as a CSoundEmitterLH (ctor 0x00AAC580,
//   vtable 0x0129E6EC, RTTI CSoundEmitterLH : CASoundEmitter : CBaseClass). CASoundEmitter's vtable 0x0129E684 is _purecall in
//   slots 1-9. The sound module is absent from both PDBs: the slot names are descriptive (the wrapper's own names; slot 9 =
//   StopSpecificSample as the landed 0x008368D0 calls it), and so is the member name (Ego_r.pdb lacks the retail-only member).
// Layout: retail CThing is 0x98 bytes (engine/CThing.h); the sound object is the first CThingSoundEmitter member, as the landed
//   constructor 0x008367B0 zeroes it. Local model (no engine/CThingSoundEmitter.h).
#include "engine/CThing.h"

class CASoundEmitter : public CBaseClass {
public:
    virtual void Virtual01() = 0;
    virtual void Virtual02() = 0;
    virtual void Virtual03() = 0;
    virtual void Virtual04() = 0;
    virtual void Virtual05() = 0;
    virtual void SetSpecificSamplePitch(unsigned long sampleID, float pitch) = 0;     // slot 6 (+0x18)
    virtual void SetSpecificSampleVolume(unsigned long sampleID, float volume) = 0;   // slot 7 (+0x1C)
    virtual void Virtual08() = 0;
    virtual void StopSpecificSample(unsigned long sampleID) = 0;                      // slot 9 (+0x24)
};

class CThingSoundEmitter : public CThing {
public:
    CASoundEmitter* PSoundEmitter;                      // +0x98 (retail-only member)
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch);
};

void CThingSoundEmitter::SetSpecificSamplePitch(unsigned long sampleID, float pitch)
{
    PSoundEmitter->SetSpecificSamplePitch(sampleID, pitch);
}

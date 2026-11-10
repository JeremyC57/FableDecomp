// CCodaSystem::CreateHardwareVoiceA() -- slot 4 of the GameCODA audio system: ask the OpenAL audio context (+0x1d4) for a new non-3D hardware voice (the context's slot 4)
// Leaf: ?CreateHardwareVoiceA@CCodaSystem@@UAEPAVCCodaVoice@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3EC10, 11 bytes, thiscall: `mov ecx,[ecx+1D4h] ; mov eax,[ecx] ; jmp [eax+10h]` (a virtual
//   tail call: `this` becomes the context, the return value is the context's), then 5 int3 to the sibling 0x00C3EC20.
// Split (chunk 900): the gap-scan row 0x00C3EC10 (27 bytes) is this function (11) + 5 int3 + CreateHardwareVoiceB 0x00C3EC20 (11).
// Identity: slot 4 (cell 0x012B5BD0) of the CCodaSystem vtable 0x012B5BC0 (slot 5 = 0x00C3EC20, slot 7 = 0x00C3EBB0); the class is the
//   one whose ctor 0x00C3E520 stores that vtable and zeroes +0x1d4 (landed CCodaSystem::GetEAXProperty 0x00C3EB80 / SetEAXProperty
//   0x00C3EB60 on the same member). The base CAudioPlatform vtable 0x012B5B80 has `xor eax,eax ; ret` stubs at slots 4 / 5 (no
//   hardware voice). +0x1d4 holds g_AudioContext 0x0143BDB0 (stored by CAnimationEnvironment::InitializeWithDefaults 0x00C3EC30),
//   vtable 0x012B8C18: slot 4 = 0x00C56CD0 / slot 5 = 0x00C56860 `new` (0x88 bytes) the hardware voices with vtables 0x012B8D48
//   (non-3D) / 0x012B8CC8 (3D), and slots 8 / 9 (0x00C564F0 / 0x00C55F60) are the EAX set / get the landed 0x00C3EB60 / 0x00C3EB80
//   forward to. A comes before B in the vtable, as in CAudioPlatform::ReleaseVoice 0x00C4D070's hardware pools.
//   Names: descriptive (libpk / GameCODA, no PDB or RTTI; owner 2026-11-04 vendor-code naming rule).
class CCodaVoice;

class CAudioContext {                               // the OpenAL audio context (g_AudioContext 0x0143BDB0, vtable 0x012B8C18)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CCodaVoice* CreateVoiceA();             // slot 4 (+0x10): 0x00C56CD0, non-3D hardware voice (vtable 0x012B8D48)
    virtual CCodaVoice* CreateVoiceB();             // slot 5 (+0x14): 0x00C56860, 3D hardware voice (vtable 0x012B8CC8)
};

class CCodaSystem {                                 // vtable 0x012B5BC0 (CAudioPlatform-derived)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CCodaVoice* CreateHardwareVoiceA();     // slot 4: 0x00C3EC10
    virtual CCodaVoice* CreateHardwareVoiceB();     // slot 5: 0x00C3EC20
    unsigned char _pad04[0x1d4 - 4];
    CAudioContext* m_pContext;                      // +0x1d4 (0 until the hardware device opens)
};

// Create a non-3D hardware voice: the audio context owns the hardware voice factories.
CCodaVoice* CCodaSystem::CreateHardwareVoiceA()
{
    return m_pContext->CreateVoiceA();
}
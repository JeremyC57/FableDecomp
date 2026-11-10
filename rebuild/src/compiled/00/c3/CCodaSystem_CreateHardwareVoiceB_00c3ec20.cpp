// CCodaSystem::CreateHardwareVoiceB() -- slot 5 of the GameCODA audio system: ask the OpenAL audio context (+0x1d4) for a new 3D hardware voice (the context's slot 5)
// Leaf: ?CreateHardwareVoiceB@CCodaSystem@@UAEPAVCCodaVoice@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3EC20, 11 bytes, thiscall: `mov ecx,[ecx+1D4h] ; mov eax,[ecx] ; jmp [eax+14h]` (a virtual
//   tail call), then int3 padding to the landed 0x00C3EC30.
// Split (chunk 900): NEW function start exposed by splitting the gap-scan row 0x00C3EC10 (27 bytes): 0x00C3EC10 (11) + 5 int3 + this (11).
// Identity: slot 5 (cell 0x012B5BD4) of the CCodaSystem vtable 0x012B5BC0 (slot 4 = CreateHardwareVoiceA 0x00C3EC10); the class is the
//   one whose ctor 0x00C3E520 stores that vtable and zeroes +0x1d4 (landed CCodaSystem::GetEAXProperty 0x00C3EB80 / SetEAXProperty
//   0x00C3EB60 on the same member). +0x1d4 holds g_AudioContext 0x0143BDB0, vtable 0x012B8C18, whose slot 5 = 0x00C56860 `new`s the
//   3D hardware voice (vtable 0x012B8CC8; CAudioPlatform::ReleaseVoice 0x00C4D070 lists it second, after the non-3D 0x012B8D48).
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

// Create a 3D hardware voice: the audio context owns the hardware voice factories.
CCodaVoice* CCodaSystem::CreateHardwareVoiceB()
{
    return m_pContext->CreateVoiceB();
}
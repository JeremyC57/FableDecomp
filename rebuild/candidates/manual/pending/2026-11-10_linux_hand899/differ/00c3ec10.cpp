// CCodaSystem::CreateHardwareVoiceA() -- slot 4 of the GameCODA audio system: create a (non-3D) hardware voice through the OpenAL context at +0x1d4 (its slot 4)
// Grade: DIFFER(11v27) -- EXTENT_WRONG: the row holds TWO functions: 0x00C3EC10 (11 bytes, this one) and 0x00C3EC20 (11 bytes,
//   CreateHardwareVoiceB below, the context's slot 5), with 5 bytes of int3 padding between; each graded alone is byte-exact.
// Leaf: ?CreateHardwareVoiceA@CCodaSystem@@UAEPAVCCodaVoice@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3EC10, 11 bytes: `mov ecx,[ecx+1D4h] ; mov eax,[ecx] ; jmp [eax+10h]`; @ 0x00C3EC20, 11 bytes:
//   `mov ecx,[ecx+1D4h] ; mov eax,[ecx] ; jmp [eax+14h]`.
// Identity: slots 4 and 5 (cells 0x012B5BD0 / 0x012B5BD4) of the CCodaSystem vtable 0x012B5BC0 (the landed GetEAXProperty 0x00C3EB80 /
//   SetEAXProperty 0x00C3EB60 class; base CAudioPlatform vtable 0x012B5B80, whose slots 4 / 5 are `xor eax,eax ; ret`, i.e. no voice).
//   +0x1d4 is the OpenAL context object g_AudioContext 0x0143BDB0 (stored by 0x00C3EC4B; class CAudioContext, vtable 0x012B8C18, landed
//   Open 0x00C56A40); its slots 4 / 5 are 0x00C56CD0 / 0x00C56860: `new` (0x88 bytes, CodaMalloc) of the hardware voices with vtables
//   0x012B8D48 (non-3D) and 0x012B8CC8 (3D) -- the CCodaVoice vtables CAudioPlatform::ReleaseVoice 0x00C4D070 lists as hardware voices,
//   the 3D one second as in its A/B pools (CreateVoiceA / CreateVoiceB are slots 6 / 7). Descriptive (libpk/GameCODA, no PDB).
class CCodaVoice;

class CAudioContext {                               // vtable 0x012B8C18 (g_AudioContext 0x0143BDB0)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CCodaVoice* CreateVoiceA();             // slot 4 (+0x10): 0x00C56CD0, vtable 0x012B8D48
    virtual CCodaVoice* CreateVoiceB();             // slot 5 (+0x14): 0x00C56860, vtable 0x012B8CC8 (3D)
};

class CCodaSystem {                                 // vtable 0x012B5BC0 (CAudioPlatform-derived)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CCodaVoice* CreateHardwareVoiceA();     // slot 4: 0x00C3EC10
    virtual CCodaVoice* CreateHardwareVoiceB();     // slot 5: 0x00C3EC20
    unsigned char _pad04[0x1d4 - 4];
    CAudioContext* m_pContext;                      // +0x1d4 (0 until the hardware device opens)
};

CCodaVoice* CCodaSystem::CreateHardwareVoiceA()
{
    return m_pContext->CreateVoiceA();
}

CCodaVoice* CCodaSystem::CreateHardwareVoiceB()
{
    return m_pContext->CreateVoiceB();
}

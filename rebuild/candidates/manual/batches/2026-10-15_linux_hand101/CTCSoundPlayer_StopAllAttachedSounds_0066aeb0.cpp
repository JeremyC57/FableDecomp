// CTCSoundPlayer::StopAllAttachedSounds() -- kill the attached sound emitter (when the sound system is up and the emitter is alive)
// Leaf: StopAllAttachedSounds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AEB0, length 58 bytes (callers 0x00773DA6, 0x00780CCE, 0x008337EA).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CTCSoundPlayer: PAttachedEmitter (CIntelligentPointer<CThingSoundEmitter>) +0x0c; the emitter is a CThing
// whose flag byte +0x91 bit 0 marks it as killed. 0x013B8394 is a global pointer (the sound system) tested first.
// Label correction: the chunk calls this CTCSummonSpell::ResumeForCutscene (propagated from the byte-identical shape);
// it sits among the CTCSoundPlayer functions and works on CTCSoundPlayer::PAttachedEmitter at +0x0c; the name is the Ego_r
// CTCSoundPlayer method whose signature fits (ghidra_out/egor_signatures.tsv).
// Shape: three GetPItem calls (the test, the flag test, the call) -- an inline "has live emitter" predicate followed by a
// fresh GetPItem for the call.
struct CBaseObject;

struct CThingSoundEmitter {
    unsigned char _pad[0x91];
    unsigned char m_thingFlags;                             // +0x91, bit 0: killed
    void Kill(bool immediately);                            // CThing::Kill @ 0x004C9B80
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch);    // 0x00836790
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);  // 0x008367A0
};

struct CBaseIntelligentPointer {
    void*        m_vtable;
    CBaseObject* m_pItem;
    CThingSoundEmitter* GetPItem() const;                   // 0x00A01B50
};

extern void* g_pSoundSystem;                                // 0x013B8394

struct CTCSoundPlayer {
    void*                   __vftable;                      // +0x00
    void*                   ParentThing;                    // +0x04
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    CBaseIntelligentPointer PAttachedEmitter;               // +0x0c CIntelligentPointer<CThingSoundEmitter>
    bool HasLiveAttachedEmitter() const
    {
        return PAttachedEmitter.GetPItem() && !(PAttachedEmitter.GetPItem()->m_thingFlags & 1);
    }
    void StopAllAttachedSounds();
};

void CTCSoundPlayer::StopAllAttachedSounds()
{
    if (g_pSoundSystem) {
        if (HasLiveAttachedEmitter())
            PAttachedEmitter.GetPItem()->Kill(true);
    }
}

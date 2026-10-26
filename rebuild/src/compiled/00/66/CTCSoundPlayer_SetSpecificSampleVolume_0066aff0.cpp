// CTCSoundPlayer::SetSpecificSampleVolume(unsigned long sampleID, float volume) -- forward a sample volume change to the attached emitter (when the sound system is up and the emitter is alive)
// Leaf: SetSpecificSampleVolume
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AFF0, length 68 bytes (`ret 8`; callers 0x006D29D8, 0x006E3423, 0x006E3555, 0x008EF50C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CThingSoundEmitter::SetSpecificSampleVolume @ 0x008367A0
// (manifest "CScriptThing::operator==", propagated; it forwards to the emitter's sound object at +0x98).
// Layout: Ego_r.pdb CTCSoundPlayer: PAttachedEmitter (CIntelligentPointer<CThingSoundEmitter>) +0x0c; the emitter is a CThing
// whose flag byte +0x91 bit 0 marks it as killed. 0x013B8394 is a global pointer (the sound system) tested first.
// Label correction: the chunk calls this CTCSummonSpell::ResumeForCutscene (propagated from the byte-identical shape);
// it sits among the CTCSoundPlayer functions and works on CTCSoundPlayer::PAttachedEmitter at +0x0c; the name is the Ego_r
// CTCSoundPlayer method whose signature fits (ghidra_out/egor_signatures.tsv).
// Shape: three GetPItem calls (the test, the flag test, the call) -- an inline "has live emitter" predicate followed by a
// fresh GetPItem for the call.
class CBaseObject;

class CThingSoundEmitter { public:
    unsigned char _pad[0x91];
    unsigned char m_thingFlags;                             // +0x91, bit 0: killed
    void Kill(bool immediately);                            // CThing::Kill @ 0x004C9B80
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch);    // 0x00836790
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);  // 0x008367A0
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

extern void* g_pSoundSystem;                                // 0x013B8394

class CTCSoundPlayer { public:
    void*                   __vftable;                      // +0x00
    void*                   ParentThing;                    // +0x04
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;               // +0x0c CIntelligentPointer<CThingSoundEmitter>
    bool HasLiveAttachedEmitter() const
    {
        return PAttachedEmitter.GetPItem() && !(PAttachedEmitter.GetPItem()->m_thingFlags & 1);
    }
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);
};

void CTCSoundPlayer::SetSpecificSampleVolume(unsigned long sampleID, float volume)
{
    if (g_pSoundSystem) {
        if (HasLiveAttachedEmitter())
            PAttachedEmitter.GetPItem()->SetSpecificSampleVolume(sampleID, volume);
    }
}
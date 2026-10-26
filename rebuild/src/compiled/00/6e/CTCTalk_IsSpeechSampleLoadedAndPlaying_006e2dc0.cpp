// CTCTalk::IsSpeechSampleLoadedAndPlaying() const -- while a speech sample is pending and its frame window has not run out, ask the owner's speech emitter whether the sample is loaded and playing; otherwise true
// Leaf: IsSpeechSampleLoadedAndPlaying
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2DC0, length 147 bytes (padded row 160).
// Callees: the world frame getter @ 0x00416225 on g_pWorld @ 0x013B86A0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
// @ 0x004365B0 (inlined GetTC of interface 0x10 = bit 16 of CThing+0x20), the emitter's sample query @ 0x0066AE60 (takes the sample id,
// returns bool; manifest label "CScriptGameResourceObjectScriptedThing::GetScriptThing" is propagated).
// Globals: an int frame-rate setting @ 0x01375550 (15 at rest), constant 10000.0f @ 0x0125FFE0.
// Layout (Ego_r.pdb CTCTalk = retail): SpeechSample +0x0C, SampleStartedFrame +0x10, SampleID +0x28.
// Shape: the global is copied into a named local before the frame call (both are `fild`ed from the frame; the lookup key later reuses its
// slot); the sample id test is unsigned (`jbe`); `!(elapsed > limit)` gives the NaN-aware `test ah,5 ; jnp` (`<=` gives `test ah,1`);
// GetTC must be `__forceinline` or it is emitted out of line.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CSpeechEmitter {
    bool IsSampleLoadedAndPlaying(unsigned long sampleID);   // 0x0066AE60
};
class CWorld { public:
    long GetFrame();                            // 0x00416225
};
extern CWorld* g_pWorld;                        // 0x013B86A0
extern long g_FrameRate;                        // 0x01375550

class CTCTalk { public:
    void*         __vfptr;                      // +0x00 (CTCBase)
    const CThing* ParentThing;                  // +0x04
    unsigned char _base08[4];
    long          SpeechSample;                 // +0x0c
    long          SampleStartedFrame;           // +0x10
    long          SampleDurationFrames;         // +0x14
    long          Attitude;                     // +0x18
    float         MimeStartTime;                // +0x1c
    float         SampleLoadedTime;             // +0x20
    void*         PSpeechBank;                  // +0x24
    unsigned long SampleID;                     // +0x28
    bool IsSpeechSampleLoadedAndPlaying() const;
};

bool CTCTalk::IsSpeechSampleLoadedAndPlaying() const
{
    if (SpeechSample > 0 && SampleID > 0) {
        long rate = g_FrameRate;
        long elapsed = g_pWorld->GetFrame() - SampleStartedFrame;
        if (!((float)elapsed > (float)rate * 10000.0f) && ParentThing->HasTC(0x10))
            return ((CSpeechEmitter*)ParentThing->GetTC(0x10))->IsSampleLoadedAndPlaying(SampleID);
    }
    return true;
}
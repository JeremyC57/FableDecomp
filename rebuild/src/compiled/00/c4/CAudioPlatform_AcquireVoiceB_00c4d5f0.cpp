// CAudioPlatform::AcquireVoiceB(CSoundDesc* desc, CPooledVoice** out) -- hand out a voice of kind B for the description: recycle the head of the free chain while preallocated voices remain (counting it down), else create one through the platform (slot 7); initialise it, keep it on the active chain, and return the result with the voice (null on failure; -5 when none can be created)
// Leaf: ?AcquireVoiceB@CAudioPlatform@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D5F0, length 197 bytes (+ int3 padding); `ret 8`.
// Callees: the pool at +0x40: GetFreeHead 0x00C3C9A0, PopFree 0x00C3CA30, PushActive 0x00C3C9B0 (landed under the propagated
// "CCombatActionPool" names); the voice's slot 1 (Init) and slot 0 (deleting destructor); the platform's slot 7.
// Globals: the voice-slot spare count 0x0139C9D4 and the shared preallocation counter 0x0139CA18, both inside the 0xa0-byte
// configuration record 0x0139C988 (landed CSystemInit 0x00C3FD20 calls the slot "SubSystemBSlot1").
// Caller: 0x00C43CB5 / 0x00C410DA, which picks this row when the description's slot-1 test is true (else AcquireVoiceA).
// The catalog label "CombatSequence_ProcessActionRound_Alt" is propagated; descriptive name (libpk, no PDB/RTTI).
// Shape: the pool address is taken once into a local (edi for the whole body, the create arm keeps ecx = this);
// one exit (`*out = voice; return result;`) that VC7.1 tail-duplicates into every arm; the recycled voice's
// failed Init shares the delete arm's `voice = 0`.

struct CSoundDesc;

struct CPooledVoice {                               // a recyclable voice (landed CPooledNode_00c3c930 shape)
    virtual ~CPooledVoice();                        // slot 0 (deleting destructor)
    virtual long Init(CSoundDesc* desc);            // slot 1
    CPooledVoice* m_pPrev;                          // +0x04
    CPooledVoice* m_pNext;                          // +0x08
};

struct CVoicePool {                                 // 0x14 bytes (landed CNodePool_00c3c930 / CCombatActionPool_00c3c9b0)
    void*         m_pOwner;                         // +0x00
    CPooledVoice* m_pActiveHead;                    // +0x04
    CPooledVoice* m_pFreeHead;                      // +0x08
    long          m_freeCount;                      // +0x0c
    long          m_activeCount;                    // +0x10
    void          Clear();                          // 0x00C3C930
    CPooledVoice* GetFreeHead();                    // 0x00C3C9A0 (returns m_pFreeHead)
    void          PushActive(CPooledVoice* voice);  // 0x00C3C9B0
    void          PopFree();                        // 0x00C3CA30
};

struct SVoiceSlot {                                 // 0x18 bytes (landed SSubSystemSlot)
    long m_00;                                      // +0x00
    long m_spare;                                   // +0x04: preallocated voices still on the free chain
    long m_rest[4];
};

struct SAudioConfig {                               // 0xa0 bytes @ 0x0139C988
    unsigned char _pad00[0x30];
    SVoiceSlot    m_voicesA;                        // +0x30 (0x0139C9B8; pool +0x2c)
    SVoiceSlot    m_voicesB;                        // +0x48 (0x0139C9D0; pool +0x40)
    unsigned char _pad60[0x30];
    long          m_preallocatedLeft;               // +0x90 (0x0139CA18)
    unsigned char _pad94[0xc];
};
extern SAudioConfig       g_audioConfig;            // 0x0139C988
extern const SAudioConfig k_defaultAudioConfig;     // 0x012B5AD8

struct CAudioMaster;
extern CAudioMaster* g_pAudioMaster;                // 0x0143BC2C

struct CAudioPlatform {                             // vtable 0x012B5B80 (the base; slots 6 and 7 return 0 there)
    virtual ~CAudioPlatform();                      // slot 0
    virtual void v04();
    virtual void v08();
    virtual void Shutdown();                        // slot 3
    virtual void v10();
    virtual void v14();
    virtual CPooledVoice* CreateVoiceA();           // slot 6 (+0x18)
    virtual CPooledVoice* CreateVoiceB();           // slot 7 (+0x1c)
    CVoicePool m_pool04;                            // +0x04
    CVoicePool m_pool18;                            // +0x18
    CVoicePool m_poolA;                             // +0x2c
    CVoicePool m_poolB;                             // +0x40
    long AcquireVoiceA(CSoundDesc* desc, CPooledVoice** out);
    long AcquireVoiceB(CSoundDesc* desc, CPooledVoice** out);
};

long CAudioPlatform::AcquireVoiceB(CSoundDesc* desc, CPooledVoice** out)
{
    CVoicePool* pool = &m_poolB;
    CPooledVoice* voice;
    long result;
    if (g_audioConfig.m_voicesB.m_spare > 0) {
        voice = pool->GetFreeHead();
        result = voice->Init(desc);
        if (result == 0) {
            pool->PopFree();
            pool->PushActive(voice);
            --g_audioConfig.m_voicesB.m_spare;
            if (g_audioConfig.m_preallocatedLeft > 0)
                --g_audioConfig.m_preallocatedLeft;
        } else {
            voice = 0;
        }
    } else {
        voice = CreateVoiceB();
        if (!voice) {
            result = -5;
        } else {
            result = voice->Init(desc);
            if (result == 0) {
                pool->PushActive(voice);
            } else {
                delete voice;
                voice = 0;
            }
        }
    }
    *out = voice;
    return result;
}
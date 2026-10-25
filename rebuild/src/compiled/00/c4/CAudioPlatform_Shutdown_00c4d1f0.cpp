// CAudioPlatform::Shutdown() -- empty the platform layer's four voice pools, restore the global audio configuration record from its defaults and forget the global audio master
// Leaf: ?Shutdown@CAudioPlatform@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D1F0, length 66 bytes (+ int3 padding); plain `ret`; slot 3 of the
// platform layer's vtable 0x012B5B80, also called directly by its destructor 0x00C3E2A0 (landed as
// "CTCFishingSpot_00c3e2a0::~CTCFishingSpot_00c3e2a0", a propagated label; there it is "Cleanup").
// Callees: the landed CNodePool::Clear 0x00C3C930 on the pools at +4, +0x18, +0x2c (AcquireVoiceA 0x00C4D520) and +0x40
// (AcquireVoiceB 0x00C4D5F0).
// Globals: the 0xa0-byte configuration record 0x0139C988 and its defaults 0x012B5AD8 (the same pair the landed
// CreateMasterObject 0x00C4D340 restores on failure), the audio master pointer 0x0143BC2C.
// The catalog label "SetCurrentInterpolateInfo" is propagated; descriptive name (libpk, no PDB/RTTI).
// Shape: the record copy is a struct assignment (`mov ecx,28h ; rep movsd`); the master pointer store follows it.

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

void CAudioPlatform::Shutdown()
{
    m_pool04.Clear();
    m_pool18.Clear();
    m_poolA.Clear();
    m_poolB.Clear();
    g_audioConfig = k_defaultAudioConfig;
    g_pAudioMaster = 0;
}
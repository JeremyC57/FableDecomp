// CAudioPlatform::ReleaseVoice(CCodaVoice* voice) -- give a finished voice back to the platform: stop it; unless it is pinned pick its pool (software voices: the A/B pools at +0x2C/+0x40, else the pools at +0x04/+0x18; 3D voices the second of each pair) and its configuration slot; with pooling enabled for the slot recycle it onto the free chain (one more spare, and one more preallocated voice while the limit is positive), else unlink and delete it; when the +0x04/+0x18 pools are the same chains, mirror the pool and the spare count into the twin
// Leaf: ?ReleaseVoice@CAudioPlatform@@QAEXPAVCCodaVoice@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4D070, 328 bytes, thiscall `ret 4`; 0xC-byte frame (the two counter pointers and
//   the twin slot), the twin pool homed in the dead `voice` argument slot.
// Class: CAudioPlatform (libpk/GameCODA, vtable 0x012B5B80; ctor 0x00C4D000, Shutdown 0x00C4D1F0, AcquireVoiceA 0x00C4D520,
//   AcquireVoiceB 0x00C4D5F0 landed): this body sits between its ctor and Shutdown and works the same four 0x14-byte node
//   pools (+0x04, +0x18, +0x2C, +0x40) and the same configuration record g_audioConfig 0x0139C988 (voice slots +0x00 /
//   +0x18 / +0x30 / +0x48, preallocation counters +0x84..+0x90) as AcquireVoiceA/B; it is their inverse.
// Voice: every pooled voice is a CCodaVoice (vtables 0x012B5C10 base, 0x012B5C60 / 0x012B5D70 / 0x012B8AE0 software,
//   0x012B8CC8 / 0x012B8D48 / 0x012BEE40 hardware): slot 0 the deleting destructor, slot 2 Stop (0x00C59710, bound
//   `?Stop@CVtable_012B5C10@@UAEXXZ`), slot 4 true for the software voices (landed name SupportsEAX), slot 5 true for the
//   two 3D voices 0x012B8AE0 / 0x012B8CC8 (Is3D, the AcquireVoiceB selector), slot 6 false in every vtable (IsPinned).
// Callers: eight sites in the CAudioObject voice handling 0x00C435xx..0x00C441xx. Names descriptive (libpk has no
//   PDB/RTTI); the manifest label "CCombatSequenceManager_ProcessAction" is propagated.
// Callees: CNodePool::RemoveActive 0x00C3C9E0, CNodePool::PushFree 0x00C3CA00 (the landed rows' own spellings).
struct CPooledNode {                            // intrusive doubly-linked node (landed CNodePool rows)
    virtual ~CPooledNode();                     // slot 0 (a discarded node is deleted through it)
    CPooledNode* m_pPrev;                       // +0x04
    CPooledNode* m_pNext;                       // +0x08
};
struct CNodePool {                              // 20 bytes: an active chain and a free chain
    void*        m_pOwner;                      // +0x00
    CPooledNode* m_pActiveHead;                 // +0x04
    CPooledNode* m_pFreeHead;                   // +0x08
    long         m_freeCount;                   // +0x0c
    long         m_activeCount;                 // +0x10
    void RemoveActive(CPooledNode* node);       // 0x00C3C9E0
    void PushFree(CPooledNode* node);           // 0x00C3CA00
    bool operator==(const CNodePool& other) const   // inline: the same two chains
    {
        return m_pActiveHead == other.m_pActiveHead && m_pFreeHead == other.m_pFreeHead;
    }
};
class CAudioObject;
class CCodaVoice : public CPooledNode {         // vtable 0x012B5C10 (19 slots)
public:
    virtual long Initialise(CAudioObject* object);  // slot 1 (0x00C596E0)
    virtual void Stop();                        // slot 2 (+0x08, 0x00C59710)
    virtual long ClearField0C(void* owner);     // slot 3 (0x00C3F460)
    virtual bool SupportsEAX();                 // slot 4 (+0x10)
    virtual bool Is3D();                        // slot 5 (+0x14)
    virtual bool IsPinned();                    // slot 6 (+0x18)
};
struct SVoiceSlot {                             // 0x18 bytes (landed SSubSystemSlot)
    long m_poolLimit;                           // +0x00: > 0 = released voices are kept for reuse
    long m_spare;                               // +0x04: pooled voices still on the free chain
    long m_rest[4];
};
struct SAudioConfig {                           // 0xa0 bytes @ 0x0139C988
    SVoiceSlot    m_voices04;                   // +0x00 (0x0139C988; pool +0x04)
    SVoiceSlot    m_voices18;                   // +0x18 (0x0139C9A0; pool +0x18)
    SVoiceSlot    m_voicesA;                    // +0x30 (0x0139C9B8; pool +0x2c)
    SVoiceSlot    m_voicesB;                    // +0x48 (0x0139C9D0; pool +0x40)
    unsigned char _pad60[0x84 - 0x60];
    long          m_preallocatedMax04;          // +0x84 (0x0139CA0C)
    long          m_preallocatedLeft04;         // +0x88 (0x0139CA10)
    long          m_preallocatedMax;            // +0x8c (0x0139CA14)
    long          m_preallocatedLeft;           // +0x90 (0x0139CA18)
    unsigned char _pad94[0xc];
};
extern SAudioConfig g_audioConfig;              // 0x0139C988

struct CAudioPlatform {                         // vtable 0x012B5B80
    virtual ~CAudioPlatform();                  // slot 0 (slots 1..7 not used here)
    CNodePool m_pool04;                         // +0x04
    CNodePool m_pool18;                         // +0x18
    CNodePool m_poolA;                          // +0x2c
    CNodePool m_poolB;                          // +0x40
    void ReleaseVoice(CCodaVoice* voice);
};

void CAudioPlatform::ReleaseVoice(CCodaVoice* voice)
{
    voice->Stop();
    if (voice->IsPinned())
        return;
    CNodePool* twin = 0;
    SVoiceSlot* twinSlot = 0;
    CNodePool* pool;
    SVoiceSlot* slot;
    long* preallocatedLeft;
    long* preallocatedMax;
    if (voice->SupportsEAX()) {
        preallocatedLeft = &g_audioConfig.m_preallocatedLeft;
        preallocatedMax = &g_audioConfig.m_preallocatedMax;
        if (voice->Is3D()) {
            pool = &m_poolB;
            slot = &g_audioConfig.m_voicesB;
        } else {
            pool = &m_poolA;
            slot = &g_audioConfig.m_voicesA;
        }
    } else {
        preallocatedLeft = &g_audioConfig.m_preallocatedLeft04;
        preallocatedMax = &g_audioConfig.m_preallocatedMax04;
        if (voice->Is3D()) {
            pool = &m_pool18;
            slot = &g_audioConfig.m_voices18;
            if (m_pool18 == m_pool04) {
                twin = &m_pool04;
                twinSlot = &g_audioConfig.m_voices04;
            }
        } else {
            pool = &m_pool04;
            slot = &g_audioConfig.m_voices04;
            if (m_pool18 == m_pool04) {
                twin = &m_pool18;
                twinSlot = &g_audioConfig.m_voices18;
            }
        }
    }
    if (slot->m_poolLimit > 0) {
        ++slot->m_spare;
        if (*preallocatedMax > 0)
            ++*preallocatedLeft;
        pool->RemoveActive(voice);
        pool->PushFree(voice);
    } else {
        pool->RemoveActive(voice);
        delete voice;
    }
    if (twin) {
        twin->m_pActiveHead = pool->m_pActiveHead;
        twin->m_pFreeHead = pool->m_pFreeHead;
        twin->m_freeCount = pool->m_freeCount;
        twin->m_activeCount = pool->m_activeCount;
    }
    if (twinSlot)
        twinSlot->m_spare = slot->m_spare;
}
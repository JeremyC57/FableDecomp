// Behaviour test for CAudioPlatform::ReleaseVoice (0x00C4D070): the voice is stopped; a pinned voice is left alone;
// otherwise its pool and configuration slot follow from SupportsEAX / Is3D; with pooling enabled for the slot the voice
// moves from the active to the free chain (one more spare; one more preallocated voice while that limit is positive),
// else it is unlinked and deleted; when the +0x04/+0x18 pools share their chains the twin pool and slot are mirrored.
#include <cstdio>
static int g_deleted = 0;
struct CPooledNode {
    virtual ~CPooledNode() {}
    CPooledNode* m_pPrev; CPooledNode* m_pNext;
};
struct CNodePool {
    void* m_pOwner; CPooledNode* m_pActiveHead; CPooledNode* m_pFreeHead; long m_freeCount; long m_activeCount;
    void RemoveActive(CPooledNode* node)
    {
        if (node == m_pActiveHead) m_pActiveHead = node->m_pNext;
        if (node->m_pPrev) node->m_pPrev->m_pNext = node->m_pNext;
        if (node->m_pNext) node->m_pNext->m_pPrev = node->m_pPrev;
        node->m_pPrev = node->m_pNext = 0; --m_activeCount;
    }
    void PushFree(CPooledNode* node) { node->m_pNext = m_pFreeHead; node->m_pPrev = 0; if (m_pFreeHead) m_pFreeHead->m_pPrev = node; m_pFreeHead = node; ++m_freeCount; }
    bool operator==(const CNodePool& other) const { return m_pActiveHead == other.m_pActiveHead && m_pFreeHead == other.m_pFreeHead; }
};
class CCodaVoice : public CPooledNode {
public:
    bool eax, is3D, pinned; int stops;
    CCodaVoice(bool e, bool d, bool p) : eax(e), is3D(d), pinned(p), stops(0) { m_pPrev = m_pNext = 0; }
    ~CCodaVoice() { ++g_deleted; }
    virtual void Stop() { ++stops; }
    virtual bool SupportsEAX() { return eax; }
    virtual bool Is3D() { return is3D; }
    virtual bool IsPinned() { return pinned; }
};
struct SVoiceSlot { long m_poolLimit; long m_spare; long m_rest[4]; };
struct SAudioConfig {
    SVoiceSlot m_voices04, m_voices18, m_voicesA, m_voicesB;
    long m_preallocatedMax04, m_preallocatedLeft04, m_preallocatedMax, m_preallocatedLeft;
};
static SAudioConfig g_audioConfig;
struct CAudioPlatform {
    CNodePool m_pool04, m_pool18, m_poolA, m_poolB;
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
        if (voice->Is3D()) { pool = &m_poolB; slot = &g_audioConfig.m_voicesB; }
        else { pool = &m_poolA; slot = &g_audioConfig.m_voicesA; }
    } else {
        preallocatedLeft = &g_audioConfig.m_preallocatedLeft04;
        preallocatedMax = &g_audioConfig.m_preallocatedMax04;
        if (voice->Is3D()) {
            pool = &m_pool18; slot = &g_audioConfig.m_voices18;
            if (m_pool18 == m_pool04) { twin = &m_pool04; twinSlot = &g_audioConfig.m_voices04; }
        } else {
            pool = &m_pool04; slot = &g_audioConfig.m_voices04;
            if (m_pool18 == m_pool04) { twin = &m_pool18; twinSlot = &g_audioConfig.m_voices18; }
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
static void activate(CNodePool& pool, CCodaVoice* v) { v->m_pNext = pool.m_pActiveHead; v->m_pPrev = 0; if (pool.m_pActiveHead) pool.m_pActiveHead->m_pPrev = v; pool.m_pActiveHead = v; ++pool.m_activeCount; }
static void clear(CNodePool& p) { p.m_pOwner = 0; p.m_pActiveHead = 0; p.m_pFreeHead = 0; p.m_freeCount = 0; p.m_activeCount = 0; }
static int fail(const char* why) { std::printf("00c4d070_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CAudioPlatform platform;
    clear(platform.m_pool04); clear(platform.m_pool18); clear(platform.m_poolA); clear(platform.m_poolB);
    g_audioConfig.m_voicesB.m_poolLimit = 4; g_audioConfig.m_voicesB.m_spare = 0;
    g_audioConfig.m_voicesA.m_poolLimit = 0; g_audioConfig.m_voicesA.m_spare = 0;
    g_audioConfig.m_preallocatedMax = 2; g_audioConfig.m_preallocatedLeft = 0;
    // pinned: stopped only
    CCodaVoice pinned(true, true, true); activate(platform.m_poolB, &pinned);
    platform.ReleaseVoice(&pinned);
    if (pinned.stops != 1 || platform.m_poolB.m_activeCount != 1 || g_audioConfig.m_voicesB.m_spare != 0) return fail("pinned");
    platform.m_poolB.RemoveActive(&pinned);
    // software 3D with pooling: recycled onto pool B's free chain
    CCodaVoice v3d(true, true, false); activate(platform.m_poolB, &v3d);
    platform.ReleaseVoice(&v3d);
    if (platform.m_poolB.m_pFreeHead != &v3d || platform.m_poolB.m_activeCount != 0 || g_audioConfig.m_voicesB.m_spare != 1
        || g_audioConfig.m_preallocatedLeft != 1 || g_deleted) return fail("recycle B");
    // software 2D without pooling: deleted from pool A
    CCodaVoice* v2d = new CCodaVoice(true, false, false); activate(platform.m_poolA, v2d);
    platform.ReleaseVoice(v2d);
    if (g_deleted != 1 || platform.m_poolA.m_activeCount != 0 || platform.m_poolA.m_pActiveHead) return fail("delete A");
    // hardware pools sharing their chains: the twin follows the released pool
    g_audioConfig.m_voices04.m_poolLimit = 1; g_audioConfig.m_voices04.m_spare = 0; g_audioConfig.m_voices18.m_spare = 0;
    g_audioConfig.m_preallocatedMax04 = 0; g_audioConfig.m_preallocatedLeft04 = 0;
    CCodaVoice hw(false, false, false); activate(platform.m_pool04, &hw);
    platform.m_pool18 = platform.m_pool04;
    platform.ReleaseVoice(&hw);
    if (platform.m_pool04.m_pFreeHead != &hw || platform.m_pool18.m_pFreeHead != &hw || platform.m_pool18.m_activeCount != 0
        || platform.m_pool18.m_freeCount != 1 || g_audioConfig.m_voices04.m_spare != 1 || g_audioConfig.m_voices18.m_spare != 1
        || g_audioConfig.m_preallocatedLeft04 != 0) return fail("twin mirror");
    // hardware pools not shared: no mirroring
    clear(platform.m_pool04); clear(platform.m_pool18); g_audioConfig.m_voices18.m_poolLimit = 1; g_audioConfig.m_voices04.m_spare = 7;
    CCodaVoice hw3d(false, true, false); activate(platform.m_pool18, &hw3d); platform.m_pool04.m_pFreeHead = &pinned;
    platform.ReleaseVoice(&hw3d);
    if (platform.m_pool18.m_pFreeHead != &hw3d || platform.m_pool04.m_pFreeHead != &pinned || g_audioConfig.m_voices04.m_spare != 7
        || g_audioConfig.m_voices18.m_spare != 2) return fail("no twin");
    std::printf("00c4d070_TEST PASS\n");
    return 0;
}
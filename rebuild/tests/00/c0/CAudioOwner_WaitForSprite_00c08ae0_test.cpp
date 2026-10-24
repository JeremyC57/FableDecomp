// behaviour test for 0x00C08AE0 CAudioOwner::WaitForSprite: poll the optional sprite under the lock, sleep 100 ms after every poll, until it is finished
#include <cstdio>

static int g_sleeps = 0, g_locks = 0, g_unlocks = 0, g_lockDepth = 0, g_badPoll = 0;
static unsigned long g_lastSleep = 0;
static void TestSleep(unsigned long ms) { ++g_sleeps; g_lastSleep = ms; if (g_lockDepth != 0) g_badPoll = 1; }   // Sleep stand-in

struct CGlobalLockTarget { int m_unused; };
static CGlobalLockTarget g_lock_0143bb14;

struct CScopedLock {
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target) : m_pTarget(target), m_state(1) { ++g_locks; ++g_lockDepth; }
    ~CScopedLock() { ++g_unlocks; --g_lockDepth; }
};

struct CSprite {
    int m_pollsLeft;
    bool IsFinished() { if (g_lockDepth != 1) g_badPoll = 1; return m_pollsLeft-- <= 0; }   // stand-in for 0x00C15B00
};

struct CAudioOwner {
    CSprite* m_pSprite;
    void WaitForSprite();
};

void CAudioOwner::WaitForSprite()
{
    bool done;
    do {
        done = true;
        {
            CScopedLock lock(&g_lock_0143bb14);
            if (m_pSprite && !m_pSprite->IsFinished())
                done = false;
        }
        TestSleep(100);
    } while (!done);
}

int main()
{
    CAudioOwner owner; owner.m_pSprite = 0;
    owner.WaitForSprite();                                        // no sprite: one poll, one sleep
    if (g_sleeps != 1 || g_locks != 1 || g_unlocks != 1 || g_lastSleep != 100) { std::printf("00c08ae0_TEST FAIL\n"); return 1; }
    CSprite sprite; sprite.m_pollsLeft = 3;
    owner.m_pSprite = &sprite;
    owner.WaitForSprite();                                        // three unfinished polls, then finished
    if (g_sleeps != 5 || g_locks != 5 || g_unlocks != 5 || g_badPoll) { std::printf("00c08ae0_TEST FAIL\n"); return 1; }
    std::printf("00c08ae0_TEST PASS\n");
    return 0;
}
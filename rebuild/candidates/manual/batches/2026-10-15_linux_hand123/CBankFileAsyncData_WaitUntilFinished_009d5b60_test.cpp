// Behaviour test for CBankFileAsyncData::WaitUntilFinished (0x009D5B60): early success when async is
// off or the read finished, failure when the buffer cannot be set up, otherwise kick + unlock + wait.
#include <cstdio>

static int g_locks = 0, g_unlocks = 0, g_kicks = 0, g_waits = 0;
struct CLock {
    bool Locked;
    CLock() : Locked(true) { ++g_locks; }
    ~CLock() { if (Locked) ++g_unlocks; }
    void Unlock() { if (Locked) { ++g_unlocks; Locked = false; } }
};
struct CBank { bool DisableAsync; void KickPendingReads() { ++g_kicks; } void Wait(unsigned int) { ++g_waits; } };
struct CData {
    CBank* m_pBank; bool Finished, m_completionPending, setupOk; unsigned int uid;
    bool SetupBuffer(bool) { return setupOk; }
    bool WaitUntilFinished()
    {
        CLock lock;
        if (!m_pBank->DisableAsync && !Finished) {
            if (!SetupBuffer(true)) return false;
            m_completionPending = false;
            m_pBank->KickPendingReads();
            lock.Unlock();
            m_pBank->Wait(uid);
        }
        return true;
    }
};

int main()
{
    CBank bank = { false };
    CData d = { &bank, true, true, true, 5 };
    if (!d.WaitUntilFinished() || g_waits != 0 || g_unlocks != 1) { std::printf("009d5b60_TEST FAIL\n"); return 1; }
    d.Finished = false; d.setupOk = false;
    if (d.WaitUntilFinished() || g_unlocks != 2 || g_kicks != 0) { std::printf("009d5b60_TEST FAIL\n"); return 1; }
    d.setupOk = true;
    if (!d.WaitUntilFinished() || g_kicks != 1 || g_waits != 1 || d.m_completionPending || g_unlocks != 3 || g_locks != 3) { std::printf("009d5b60_TEST FAIL\n"); return 1; }
    std::printf("009d5b60_TEST PASS\n");
    return 0;
}

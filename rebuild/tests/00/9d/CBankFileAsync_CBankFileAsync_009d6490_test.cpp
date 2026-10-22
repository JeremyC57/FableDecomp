#include <cstdio>
#include <set>
#include <string>
// behaviour test: CBankFileAsync::~CBankFileAsync (0x009D6490) -- drain the async-data set under the lock
static std::string g_log;
struct CCriticalSectionLock {
    bool m_locked;
    CCriticalSectionLock() : m_locked(true) { g_log += "L"; }
    void Unlock() { if (m_locked) g_log += "u"; m_locked = false; }
    void Lock() { g_log += "l"; m_locked = true; }
    ~CCriticalSectionLock() { if (m_locked) g_log += "U"; g_log += "~"; }
};
static void Sleep(unsigned int ms) { char b[16]; std::sprintf(b, "S%u", ms); g_log += b; }
struct CBankFileAsyncData {
    int Id;
    int TriesLeft;          // Abort fails this many times first
    bool Abort() { if (TriesLeft > 0) { --TriesLeft; return false; } char b[8]; std::sprintf(b, "A%d", Id); g_log += b; return true; }
};
struct ById { bool operator()(const CBankFileAsyncData* a, const CBankFileAsyncData* b) const { return a->Id < b->Id; } };
struct CHandler { int Removed; void RemoveBank(void*) { ++Removed; g_log += "R"; } };
struct CBankFileAsync {
    std::set<CBankFileAsyncData*, ById> AsyncDataInstances;
    CHandler* MemoryFailureHandler;
    bool Deleted;
    void Destroy()
    {
        CCriticalSectionLock lock;
        while (AsyncDataInstances.size() != 0) {
            std::set<CBankFileAsyncData*, ById>::iterator first = AsyncDataInstances.begin();
            if ((*first)->Abort()) {
                AsyncDataInstances.erase(first);
            } else {
                lock.Unlock();
                Sleep(10);
                lock.Lock();
            }
        }
        lock.Unlock();
        if (MemoryFailureHandler)
            MemoryFailureHandler->RemoveBank(this);
        g_log += "D";
        Deleted = true;
    }
};
int main()
{
    int fail = 0;
    CBankFileAsyncData a = { 2, 0 }, b = { 1, 2 }, c = { 3, 1 };
    CHandler h = { 0 };
    CBankFileAsync bank;
    bank.AsyncDataInstances.insert(&a);
    bank.AsyncDataInstances.insert(&b);
    bank.AsyncDataInstances.insert(&c);
    bank.MemoryFailureHandler = &h;
    bank.Deleted = false;
    bank.Destroy();
    // id 1 needs two retries (unlock, sleep, relock each), then 2, then 3 after one retry
    if (g_log != "LuS10luS10lA1A2uS10lA3uRD~") { std::printf("FAIL log %s\n", g_log.c_str()); fail = 1; }
    if (!bank.AsyncDataInstances.empty() || h.Removed != 1 || !bank.Deleted) { std::printf("FAIL state\n"); fail = 1; }
    g_log = "";
    CBankFileAsync empty;
    empty.MemoryFailureHandler = 0;
    empty.Deleted = false;
    empty.Destroy();
    if (g_log != "LuD~") { std::printf("FAIL empty %s\n", g_log.c_str()); fail = 1; }
    if (!fail) std::printf("009d6490_TEST PASS\n");
    return fail;
}
// Behaviour test for 0x00c0e5c0 CVtable_012b25a8::Unlock() (libpk bank object): slot 5 on the +0x50 container, then on the +0x60 one.
#include <cstdio>
#include <cstring>

static char g_log[16];
static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }

class CContainer {
public:
    explicit CContainer(char id) : Id(id) { }
    virtual ~CContainer() { }
    virtual void Unlock() { Log(Id); Log('u'); }      // slot 5
    virtual void Lock() { Log(Id); Log('l'); }        // slot 6
    char Id;
};

class CVtable_012b25a8 {
public:
    CVtable_012b25a8() : m_50('5'), m_60('6') { }
    void Lock() { m_60.Lock(); m_50.Lock(); }
    void Unlock() { m_50.Unlock(); m_60.Unlock(); }
    CContainer m_50;
    CContainer m_60;
};

int main()
{
    CVtable_012b25a8 bank;
    bank.Unlock();
    if (std::strcmp(g_log, "5u6u") != 0) {
        std::printf("00c0e5c0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00c0e5c0_TEST PASS\n");
    return 0;
}
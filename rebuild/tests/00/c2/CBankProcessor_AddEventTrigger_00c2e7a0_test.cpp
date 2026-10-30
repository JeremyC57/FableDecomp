#include <cstdio>
#include <cstring>

// Behaviour test for 0x00C2E7A0 CBankProcessor::AddEventTrigger: no registry -> nothing; duplicate criteria string ->
// one log line with the hash (skipped for sub-events); otherwise every known driver inserted at the trigger's initial
// driver count, unknown drivers logged.
static char g_log[1024];
struct CString {
    const char* m_pchData;
    CString(const char* s) : m_pchData(s) { }
    unsigned int GetHash() const { unsigned int h = 0; for (const char* p = m_pchData; *p; ++p) h = h * 31 + (unsigned char)*p; return h; }
    operator const char*() const { return m_pchData; }
};
struct CDebugStream {
    CDebugStream& operator<<(const char* t) { std::strcat(g_log, t); return *this; }
    CDebugStream& operator<<(const CString& t) { std::strcat(g_log, t.m_pchData); return *this; }
    CDebugStream& operator<<(unsigned int v) { char b[16]; std::sprintf(b, "%u", v); std::strcat(g_log, b); return *this; }
};
static CDebugStream g_stream;
struct CLoadLog { int messages; CDebugStream& AddMessage() { ++messages; std::strcat(g_log, "|"); return g_stream; } };
template <class T> struct PKCAutoDeleteArray { T* m_pData; unsigned int m_count; T& operator[](unsigned int i) { return m_pData[i]; } };
struct CEventRecord { CString m_name; PKCAutoDeleteArray<unsigned int> m_drivers; };
struct CDriver { unsigned int number; };
struct CItemDispatcher { const char* existing; int finds; void* Find(const CString& n, int* c) { ++finds; return (c == 0 && existing && !std::strcmp(existing, n.m_pchData)) ? this : 0; } };
static int g_ctorNamed = -1, g_inserts = 0, g_positions[8];
static CDriver* g_inserted[8];
static const char* g_ctorName = 0;
struct CEventTriggerRef {
    CEventTriggerRef(CItemDispatcher*, CString name, bool named) { g_ctorName = name.m_pchData; g_ctorNamed = named; }
    unsigned int GetDriverCount() { return 3; }
    void InsertDriver(unsigned int position, CDriver* d) { g_positions[g_inserts] = (int)position; g_inserted[g_inserts++] = d; }
};
static CDriver g_drivers[4] = { { 10 }, { 11 }, { 12 }, { 13 } };
struct CDriverMap { CDriver* Find(unsigned int n) { return (n >= 10 && n <= 13) ? &g_drivers[n - 10] : 0; } };

struct CBankProcessor {
    CLoadLog* m_pLog;
    CDriverMap m_drivers;
    CItemDispatcher* m_pCriteriaTriggers;
    bool m_bNamedTriggers;
    void AddEventTrigger(const CEventRecord& record, bool subEvent)
    {
        if (!m_pCriteriaTriggers)
            return;
        if (!subEvent && m_pCriteriaTriggers->Find(record.m_name, 0)) {
            m_pLog->AddMessage() << "Duplicate event trigger for criteria string '" << record.m_name << "', hash code "
                                 << record.m_name.GetHash();
            return;
        }
        CEventTriggerRef trigger(m_pCriteriaTriggers, (const char*)record.m_name, m_bNamedTriggers);
        unsigned int position = trigger.GetDriverCount();
        for (unsigned int i = 0; i < record.m_drivers.m_count; ++i) {
            unsigned int number = const_cast<PKCAutoDeleteArray<unsigned int>&>(record.m_drivers)[i];
            CDriver* driver = m_drivers.Find(number);
            if (!driver)
                m_pLog->AddMessage() << "Could not find driver " << number << " attach to criteria string '" << record.m_name << "'";
            else
                trigger.InsertDriver(position, driver);
        }
    }
};

int main()
{
    int bad = 0;
    CLoadLog log = { 0 };
    CItemDispatcher reg = { "EV_HIT", 0 };
    CBankProcessor bp; bp.m_pLog = &log; bp.m_pCriteriaTriggers = 0; bp.m_bNamedTriggers = true;
    unsigned int numbers[3] = { 12, 99, 10 };
    CEventRecord rec = { CString("EV_SWING"), { numbers, 3 } };
    bp.AddEventTrigger(rec, false);                                   // no registry
    bad |= reg.finds != 0 || g_ctorName != 0 || log.messages != 0;
    bp.m_pCriteriaTriggers = &reg;
    bp.AddEventTrigger(rec, false);                                   // new trigger: 12 and 10 inserted, 99 logged
    bad |= reg.finds != 1 || g_ctorName == 0 || std::strcmp(g_ctorName, "EV_SWING") != 0 || g_ctorNamed != 1;
    bad |= g_inserts != 2 || g_inserted[0] != &g_drivers[2] || g_inserted[1] != &g_drivers[0] || g_positions[0] != 3 || g_positions[1] != 3;
    bad |= std::strcmp(g_log, "|Could not find driver 99 attach to criteria string 'EV_SWING'") != 0;
    g_log[0] = 0;
    CEventRecord dup = { CString("EV_HIT"), { numbers, 1 } };
    bp.AddEventTrigger(dup, false);                                   // duplicate: logged with its hash, no insert
    char expect[128]; std::sprintf(expect, "|Duplicate event trigger for criteria string 'EV_HIT', hash code %u", CString("EV_HIT").GetHash());
    bad |= std::strcmp(g_log, expect) != 0 || g_inserts != 2;
    g_log[0] = 0;
    bp.m_bNamedTriggers = false;
    bp.AddEventTrigger(dup, true);                                    // sub-event: no duplicate check
    bad |= reg.finds != 2 || g_inserts != 3 || g_inserted[2] != &g_drivers[2] || g_ctorNamed != 0 || g_log[0] != 0;
    if (bad) { std::printf("00c2e7a0_TEST FAIL\n"); return 1; }
    std::printf("00c2e7a0_TEST PASS\n");
    return 0;
}
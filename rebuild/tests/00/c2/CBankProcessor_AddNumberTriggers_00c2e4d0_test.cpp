// Behaviour test for CBankProcessor::AddNumberTriggers @ 0x00C2E4D0 (copy of the bank processor logic; the log, maps, manager and registries are fakes).
#include <cstdio>
#include <cstring>

// ---- libpk pieces (fakes) ----
struct CDebugStream {                        // the message text of one log entry
    char m_text[128];
    CDebugStream& operator<<(const char* text) { std::strcat(m_text, text); return *this; }
    CDebugStream& operator<<(unsigned int value) { char b[16]; std::sprintf(b, "%u", value); std::strcat(m_text, b); return *this; }
};

struct CLoadLog {
    CDebugStream m_messages[8];
    int  m_count;
    int  m_okChecks, m_failAfterChecks;      // IsOk turns false after this many checks (0 = never)
    CDebugStream& AddMessage() { m_messages[m_count].m_text[0] = 0; return m_messages[m_count++]; }
    bool IsOk() { ++m_okChecks; return !(m_failAfterChecks && m_okChecks >= m_failAfterChecks); }
};

class CString {                              // the MFC-style library string, reduced to a fixed buffer
public:
    char m_pchData[64];
    CString(const char* text) { std::strcpy(m_pchData, text); }
    CString(const CString& other) { std::strcpy(m_pchData, other.m_pchData); }
    const CString& operator=(const CString& other) { std::strcpy(m_pchData, other.m_pchData); return *this; }
    void SetAt(int index, char ch) { m_pchData[index] = ch; }
    static int SafeStrlen(const char* text) { return text ? (int)std::strlen(text) : 0; }
};

bool IsSubEventName(const CString& name)     // 0x00C2ECD0
{
    if (std::strlen(name.m_pchData) > 5 && std::strncmp(name.m_pchData + std::strlen(name.m_pchData) - 5, "_SUB", 4) == 0)
        return true;
    return false;
}

// ---- bank data ----
struct SSampleHeader {
    unsigned int   m_index, m_04, m_length, m_dataOffset;
    unsigned short m_channels, m_formatTag;
    unsigned int   m_sampleRate;
    int            m_loopStart, m_loopEnd;
};
struct SDriverRecord { unsigned int m_number; unsigned short m_group; };
struct CEventRecord {
    CString        m_name;
    unsigned int*  m_pDrivers;
    unsigned int   m_driverCount;
    CEventRecord(const char* name) : m_name(name), m_pDrivers(0), m_driverCount(0) { }
};

template <class T>
struct CRecordList {
    T*           m_items[6];
    unsigned int m_count;
    T* GetItem(unsigned int index) { return m_items[index]; }
    unsigned int GetCount() { return m_count; }
};

struct CBankData {
    CRecordList<SSampleHeader> m_samples;
    CRecordList<SDriverRecord> m_drivers;
    CRecordList<CEventRecord>  m_events;
};

// ---- the processor's collaborators ----
struct CAudioFormat {
    unsigned int   m_length, m_loopStart, m_loopEnd;
    unsigned short m_channels, m_codec;
    unsigned int   m_sampleRate;
    CAudioFormat(unsigned int length, unsigned short channels, unsigned int sampleRate)
    {
        m_length = length; m_loopStart = 0; m_loopEnd = 0; m_codec = 0; m_channels = channels; m_sampleRate = sampleRate;
    }
    unsigned int GetSamplesPerBlock() { return m_codec == 0 ? m_channels : (m_codec <= 2 ? m_channels * 64u : 0u); }
    unsigned int GetBlockAlign() { return m_codec == 0 ? m_channels * 2u : (m_codec <= 2 ? m_channels * 36u : 0u); }
    unsigned int SamplesToBytes(unsigned int samples) { unsigned int blocks = samples / GetSamplesPerBlock(); return GetBlockAlign() * blocks; }
};

struct CDriver { unsigned int m_number; };
struct CSample {
    CAudioFormat m_format; unsigned int m_offset;
    CSample() : m_format(0, 0, 0), m_offset(0) { }
    CSample(const CAudioFormat& f, unsigned int o) : m_format(f), m_offset(o) { }
};

struct CDriverGroup {
    unsigned int m_id;
    CDriver*     m_drivers[4];
    int          m_count;
    void AddDriver(CDriver* driver) { m_drivers[m_count++] = driver; }
};

struct CSoundManager {
    CSample      m_samples[4];
    int          m_sampleCount;
    CDriverGroup m_groups[4];
    int          m_groupCount, m_created;
    void* CreateSample(const CAudioFormat* format, unsigned int dataOffset)
    {
        m_samples[m_sampleCount] = CSample(*format, dataOffset);
        return &m_samples[m_sampleCount++];
    }
    CDriverGroup* FindGroup(unsigned int group)
    {
        for (int i = 0; i < m_groupCount; ++i) if (m_groups[i].m_id == group) return &m_groups[i];
        return 0;
    }
    CDriverGroup* CreateGroup(unsigned int group)
    {
        ++m_created; m_groups[m_groupCount].m_id = group; m_groups[m_groupCount].m_count = 0;
        return &m_groups[m_groupCount++];
    }
};

struct CSampleMap {
    void*        m_samples[4];
    unsigned int m_indices[4];
    int          m_count;
    void Add(CLoadLog*, void* sample, unsigned int index) { m_samples[m_count] = sample; m_indices[m_count++] = index; }
};

struct CDriverMap {
    CDriver* m_drivers[4];
    int      m_count;
    CDriver* Find(unsigned int number)
    {
        for (int i = 0; i < m_count; ++i) if (m_drivers[i]->m_number == number) return m_drivers[i];
        return 0;
    }
};

struct CEventTrigger { unsigned int m_number; CDriver* m_drivers[4]; unsigned int m_count; };

struct CNumberTriggerRegistry {
    CEventTrigger m_triggers[4];
    int           m_count;
    CEventTrigger* Find(unsigned int number)
    {
        for (int i = 0; i < m_count; ++i) if (m_triggers[i].m_number == number) return &m_triggers[i];
        return 0;
    }
    CEventTrigger* Create(unsigned int number)
    {
        m_triggers[m_count].m_number = number; m_triggers[m_count].m_count = 0;
        return &m_triggers[m_count++];
    }
};

struct CEventTriggerRef {                    // finds the trigger or creates and registers it
    CNumberTriggerRegistry* m_pRegistry;
    CEventTrigger*          m_pTrigger;
    CEventTriggerRef(CNumberTriggerRegistry* registry, unsigned int number) : m_pRegistry(registry)
    {
        m_pTrigger = registry->Find(number);
        if (!m_pTrigger)
            m_pTrigger = registry->Create(number);
    }
    unsigned int GetDriverCount() { return m_pTrigger->m_count; }
    void InsertDriver(unsigned int position, CDriver* driver)
    {
        for (unsigned int i = m_pTrigger->m_count; i > position; --i) m_pTrigger->m_drivers[i] = m_pTrigger->m_drivers[i - 1];
        m_pTrigger->m_drivers[position] = driver;
        ++m_pTrigger->m_count;
    }
};

struct CBankProcessor {
    CBankData*              m_pData;
    CSoundManager*          m_pManager;
    CLoadLog*               m_pLog;
    CDriverMap              m_drivers;
    CSampleMap              m_samples;
    CNumberTriggerRegistry* m_pNumberTriggers;
    bool                    m_bSubEvents;
    // fakes of the sibling steps
    const SDriverRecord*    m_added[6];
    int                     m_addedCount;
    char                    m_triggerNames[6][64];
    bool                    m_triggerSub[6];
    int                     m_triggerCount;
    void AddDriver(const SDriverRecord* record) { m_added[m_addedCount++] = record; }
    void AddEventTrigger(const CEventRecord& record, bool subEvent)
    {
        std::strcpy(m_triggerNames[m_triggerCount], record.m_name.m_pchData);
        m_triggerSub[m_triggerCount++] = subEvent;
    }

    void AddSample(const SSampleHeader* header)                       // 0x00C2E190
    {
        CAudioFormat format(header->m_length, header->m_channels, header->m_sampleRate);
        switch (header->m_formatTag) {
        case 1:    format.m_codec = 0; break;
        case 2:    format.m_codec = 2; break;
        case 0x69: format.m_codec = 1; break;
        default:
            m_pLog->AddMessage() << "Unknown sample format for index " << header->m_index;
            return;
        }
        unsigned int loopStart = header->m_loopStart < 0 ? 0 : header->m_loopStart;
        unsigned int loopEnd = header->m_loopEnd < 0 ? 0 : header->m_loopEnd + 1;
        format.m_loopStart = format.SamplesToBytes(loopStart);
        format.m_loopEnd = format.SamplesToBytes(loopEnd);
        void* sample = m_pManager->CreateSample(&format, header->m_dataOffset);
        m_samples.Add(m_pLog, sample, header->m_index);
    }
    void AddDrivers()                                                 // 0x00C2E480
    {
        for (unsigned int i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
            AddDriver(m_pData->m_drivers.GetItem(i));
            if (!m_pLog->IsOk())
                return;
        }
    }
    void AddNumberTriggers()                                          // 0x00C2E4D0
    {
        if (!m_pNumberTriggers)
            return;
        for (unsigned int i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
            SDriverRecord* record = m_pData->m_drivers.GetItem(i);
            CDriver* driver = m_drivers.Find(record->m_number);
            if (!driver)
                m_pLog->AddMessage() << "Could not find driver number " << record->m_number;
            else if (m_pNumberTriggers->Find(record->m_number))
                m_pLog->AddMessage() << "Duplicate event trigger for number " << record->m_number;
            else {
                CEventTriggerRef trigger(m_pNumberTriggers, record->m_number);
                trigger.InsertDriver(trigger.GetDriverCount(), driver);
            }
        }
    }
    void AddDriverToGroup(unsigned int number, unsigned int group)    // 0x00C2E5A0
    {
        if (!group)
            return;
        CDriver* driver = m_drivers.Find(number);
        if (!driver) {
            m_pLog->AddMessage() << "Could not find driver number " << number;
            return;
        }
        CDriverGroup* driverGroup = m_pManager->FindGroup(group);
        if (!driverGroup)
            driverGroup = m_pManager->CreateGroup(group);
        driverGroup->AddDriver(driver);
    }
    void AddEvent(const CEventRecord& record)                         // 0x00C2E8D0
    {
        if (m_bSubEvents) {
            CString name(record.m_name);
            if (IsSubEventName(name))
                return;
        }
        AddEventTrigger(record, false);
    }
    void AddSubEvent(const CEventRecord& record)                      // 0x00C2E960
    {
        if (!m_bSubEvents)
            return;
        CString name(record.m_name);
        if (IsSubEventName(name)) {
            if (CString::SafeStrlen(name.m_pchData) > 5)
                name.SetAt(CString::SafeStrlen(name.m_pchData) - 5, 0);
            CEventRecord subRecord(record);
            subRecord.m_name = name;
            AddEventTrigger(subRecord, true);
        }
    }
    void AddEvents()                                                  // 0x00C2EA30
    {
        for (unsigned int i = 0; i < m_pData->m_events.GetCount(); ++i)
            AddEvent(*m_pData->m_events.GetItem(i));
    }
    void AddSubEvents()                                               // 0x00C2EA70
    {
        for (unsigned int i = 0; i < m_pData->m_events.GetCount(); ++i)
            AddSubEvent(*m_pData->m_events.GetItem(i));
    }
};

static CBankData     g_data;
static CSoundManager g_manager;
static CLoadLog      g_log;
static CDriver       g_driverObjects[3];

static void Setup(CBankProcessor& p)
{
    std::memset(&g_data, 0, sizeof(g_data));
    g_manager.m_sampleCount = 0; g_manager.m_groupCount = 0; g_manager.m_created = 0;
    g_log.m_count = 0; g_log.m_okChecks = 0; g_log.m_failAfterChecks = 0;
    p.m_pData = &g_data; p.m_pManager = &g_manager; p.m_pLog = &g_log;
    for (int i = 0; i < 3; ++i) { g_driverObjects[i].m_number = 20 + i; p.m_drivers.m_drivers[i] = &g_driverObjects[i]; }
    p.m_drivers.m_count = 3;
    p.m_samples.m_count = 0;
    p.m_pNumberTriggers = 0;
    p.m_bSubEvents = false;
    p.m_addedCount = 0;
    p.m_triggerCount = 0;
}

int main()
{
    bool ok = true;
    CBankProcessor p;
    Setup(p);
    SDriverRecord r[4];
    r[0].m_number = 20; r[1].m_number = 99; r[2].m_number = 21; r[3].m_number = 20;
    for (int i = 0; i < 4; ++i) { r[i].m_group = 0; g_data.m_drivers.m_items[i] = &r[i]; }
    g_data.m_drivers.m_count = 4;
    p.AddNumberTriggers();                                 // no registry: nothing happens
    if (g_log.m_count != 0) ok = false;
    CNumberTriggerRegistry registry;
    registry.m_count = 0;
    p.m_pNumberTriggers = &registry;
    p.AddNumberTriggers();
    if (registry.m_count != 2 || registry.m_triggers[0].m_number != 20 || registry.m_triggers[1].m_number != 21) ok = false;
    if (registry.m_triggers[0].m_count != 1 || registry.m_triggers[0].m_drivers[0] != &g_driverObjects[0]) ok = false;
    if (registry.m_triggers[1].m_drivers[0] != &g_driverObjects[1]) ok = false;
    if (g_log.m_count != 2) ok = false;
    if (std::strcmp(g_log.m_messages[0].m_text, "Could not find driver number 99") != 0) ok = false;
    if (std::strcmp(g_log.m_messages[1].m_text, "Duplicate event trigger for number 20") != 0) ok = false;
    if (!ok) { std::printf("00c2e4d0_TEST FAIL\n"); return 1; }
    std::printf("00c2e4d0_TEST PASS\n");
    return 0;
}
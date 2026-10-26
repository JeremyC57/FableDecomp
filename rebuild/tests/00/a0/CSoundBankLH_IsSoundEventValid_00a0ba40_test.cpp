// Behaviour test for 0x00A0BA40 CSoundBankLH::IsSoundEventValid: needs an engine and a loaded bank, then a by-name library lookup.
#include <cstdio>

class CSampleQueryLH {
public:
    virtual void v00() { }
    long        m_group;
    long        m_sample;
    const void* m_pName;
    void*       m_pBank;
    CSampleQueryLH(void* bank, long group, long sample) : m_group(group), m_sample(sample), m_pName(0), m_pBank(bank) { }
    CSampleQueryLH(void* bank, const void* name) : m_group(-1), m_sample(-1), m_pName(name), m_pBank(bank) { }
    ~CSampleQueryLH() { }
};
class CSampleLH {
public:
    long  m_value, m_first, m_last, m_rate, m_format;
    float m_length;
    CSampleLH(long value, float length) : m_value(value), m_first(10), m_last(20), m_rate(22050), m_format(3), m_length(length) { }
    virtual long  GetValue() { return m_value; }
    virtual long  GetRange(long channel, long* first, long* last)
    {
        if (channel < 0) return -1;
        *first = m_first + channel; *last = m_last + channel; return 0;
    }
    virtual long  GetRate() { return m_rate; }
    virtual long  GetFormat() { return m_format; }
    virtual float GetLength() { return m_length; }
};
class CSampleLibraryLH {
public:
    CSampleLH*  m_samples[3];
    void*       m_bank;
    int         m_finds;
    virtual void v00() { }
    virtual CSampleLH* Find(const CSampleQueryLH& query, long a, long b)
    {
        ++m_finds;
        if (query.m_pBank != m_bank || a != 0 || b != 0) return 0;
        if (query.m_pName) {
            const char* name = (const char*)query.m_pName;
            return (query.m_group == -1 && query.m_sample == -1 && name[0] == 's' && name[1] == 0) ? m_samples[0] : 0;
        }
        if (query.m_group != 0 || query.m_sample < 0 || query.m_sample >= 3) return 0;
        return m_samples[query.m_sample];
    }
};
class CSoundEngineLH {
public:
    CSampleLibraryLH* m_pLibrary;
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { }
    virtual void v05() { } virtual void v06() { } virtual void v07() { }
    virtual CSampleLibraryLH* GetSampleLibrary() { return m_pLibrary; }
};
struct CSoundSystemLH {
    unsigned char   _pad[0x40];
    CSoundEngineLH* m_pEngine;
};

// fixture: a bank with three samples (value 5 / 0 / absent)
static int  g_bankToken = 0;
static CSampleLH        g_s0(5, 1.5f), g_s1(0, 2.25f);
static CSampleLibraryLH g_library;
static CSoundEngineLH   g_engine;
static CSoundSystemLH   g_system;
static void SetUpFixture()
{
    g_library.m_samples[0] = &g_s0; g_library.m_samples[1] = &g_s1; g_library.m_samples[2] = 0;
    g_library.m_bank = &g_bankToken; g_library.m_finds = 0;
    g_engine.m_pLibrary = &g_library;
    g_system.m_pEngine = &g_engine;
}
class CCharStringData { public:
    char* m_pData;
};
class CCharString {
public:
    CCharStringData* PStringData;
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
};
class CSoundBankLH {
public:
    virtual void v00() { }
    long            m_4;
    long            m_8;
    CSoundSystemLH* m_pSystem;
    void*           m_pBank;
    virtual unsigned char IsSoundEventValid(const CCharString& name);
};

unsigned char CSoundBankLH::IsSoundEventValid(const CCharString& name)
{
    CSoundSystemLH* system = m_pSystem;
    if (system->m_pEngine && m_pBank) {
        CSampleLH* found = system->m_pEngine->GetSampleLibrary()->Find(CSampleQueryLH(m_pBank, name.GetPString()), 0, 0);
        return found != 0;
    }
    return false;
}

int main()
{
    SetUpFixture();
    char textS[] = "s";
    char textT[] = "t";
    CCharStringData dataS = { textS }, dataT = { textT };
    CCharString s, t, empty;
    s.PStringData = &dataS; t.PStringData = &dataT; empty.PStringData = 0;
    CSoundBankLH bank;
    bank.m_pSystem = &g_system; bank.m_pBank = &g_bankToken;
    if (bank.IsSoundEventValid(s) != 1) { std::printf("00a0ba40_TEST FAIL\n"); return 1; }
    if (bank.IsSoundEventValid(t) != 0) { std::printf("00a0ba40_TEST FAIL\n"); return 1; }
    if (bank.IsSoundEventValid(empty) != 0) { std::printf("00a0ba40_TEST FAIL\n"); return 1; }
    if (g_library.m_finds != 3) { std::printf("00a0ba40_TEST FAIL\n"); return 1; }
    bank.m_pBank = 0;                                       // no bank loaded: no lookup
    if (bank.IsSoundEventValid(s) != 0 || g_library.m_finds != 3) { std::printf("00a0ba40_TEST FAIL\n"); return 1; }
    bank.m_pBank = &g_bankToken;
    g_system.m_pEngine = 0;
    if (bank.IsSoundEventValid(s) != 0 || g_library.m_finds != 3) { std::printf("00a0ba40_TEST FAIL\n"); return 1; }
    std::printf("00a0ba40_TEST PASS\n");
    return 0;
}
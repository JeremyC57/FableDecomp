// Behaviour test for 0x00A0B630 CSoundBankLH::Load: when no bank is loaded and an engine exists, loads the full-path name with the fixed descriptor and keeps the handle.
#include <cstdio>

static int g_converts = 0, g_destroyed = 0;
class CWideString {
public:
    const unsigned short* m_pData;
    operator const unsigned short*() const { return m_pData; }
    ~CWideString() { ++g_destroyed; }
};
static unsigned short g_fullPath[] = { 'C', ':', '/', 'b', 0 };
class CAFile {
public:
    static CWideString ConvertPathToFullPath(const CWideString& path)
    {
        ++g_converts;
        CWideString full;
        full.m_pData = path.m_pData ? g_fullPath : 0;
        return full;
    }
};

struct SBankLoadDesc {
    long a;
    long b;
    bool f0, f1, f2, f3, f4, f5, f6;
    SBankLoadDesc() : a(2), b(2), f0(false), f1(false), f2(false), f3(false), f4(false), f5(false), f6(false) { }
};
static int g_handleToken = 0;
struct CSoundEngineLH {
    int m_loads;
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { }
    virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { }
    virtual void* LoadBank(const unsigned short* name, const SBankLoadDesc* desc)
    {
        ++m_loads;
        if (name != g_fullPath) return 0;
        if (desc->a != 2 || desc->b != 2 || desc->f0 || desc->f1 || !desc->f2 || desc->f3 || !desc->f4 || !desc->f5 || !desc->f6)
            return 0;
        return &g_handleToken;
    }
};
struct CSoundSystemLH { unsigned char _pad[0x40]; CSoundEngineLH* m_pEngine; };
struct CSoundBankLH {
    virtual void v00() { }
    long            m_4;
    long            m_8;
    CSoundSystemLH* m_pSystem;
    void*           m_pBank;
    virtual void Load(const CWideString& fileName);
};

void CSoundBankLH::Load(const CWideString& fileName)
{
    if (m_pBank == 0 && m_pSystem->m_pEngine) {
        SBankLoadDesc d;
        d.f6 = true;
        d.f2 = true;
        d.f5 = true;
        d.f4 = true;
        CWideString w = CAFile::ConvertPathToFullPath(fileName);
        CSoundEngineLH* engine = m_pSystem->m_pEngine;
        m_pBank = engine->LoadBank(w, &d);
    }
}

int main()
{
    static unsigned short rel[] = { 'b', 0 };
    CSoundEngineLH engine; engine.m_loads = 0;
    CSoundSystemLH system; system.m_pEngine = 0;
    CSoundBankLH bank; bank.m_pSystem = &system; bank.m_pBank = 0;
    CWideString name; name.m_pData = rel;
    bank.Load(name);                                         // no engine: nothing
    if (bank.m_pBank != 0 || g_converts != 0 || engine.m_loads != 0) { std::printf("00a0b630_TEST FAIL\n"); return 1; }
    system.m_pEngine = &engine;
    int destroyedBefore = g_destroyed;
    bank.Load(name);
    if (bank.m_pBank != &g_handleToken || g_converts != 1 || engine.m_loads != 1 || g_destroyed <= destroyedBefore)
        { std::printf("00a0b630_TEST FAIL\n"); return 1; }
    bank.Load(name);                                         // already loaded: nothing
    if (g_converts != 1 || engine.m_loads != 1) { std::printf("00a0b630_TEST FAIL\n"); return 1; }
    std::printf("00a0b630_TEST PASS\n");
    return 0;
}
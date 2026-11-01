// Behaviour test for 0x00CB9C50 (CScriptBindingPair copy constructor): the name is copied through the
// string copy constructor, the counted pointer copied and its shared count incremented.
#include <cstdio>

class CCPPointerInfo { public:
    long m_count;
};

class CEntityScriptBindingBase;

static int g_stringCopies = 0;

class CCharString { public:
    const char* m_pData;
    CCharString(const char* text) : m_pData(text) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};

struct CCountedPointer_CEntityScriptBindingBase {
    CEntityScriptBindingBase* Data;
    CCPPointerInfo*           Info;
    CCountedPointer_CEntityScriptBindingBase() : Data(0), Info(0) { }
    CCountedPointer_CEntityScriptBindingBase(const CCountedPointer_CEntityScriptBindingBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

struct CScriptBindingPair {
    CCharString                              first;
    CCountedPointer_CEntityScriptBindingBase second;
    CScriptBindingPair(const char* name) : first(name) { }
    CScriptBindingPair(const CScriptBindingPair& other);
};

CScriptBindingPair::CScriptBindingPair(const CScriptBindingPair& other)
    : first(other.first), second(other.second)
{
}

int main()
{
    static const char kName[] = "Binding";
    CCPPointerInfo info;
    info.m_count = 3;
    CScriptBindingPair a(kName);
    a.second.Data = (CEntityScriptBindingBase*)&info;
    a.second.Info = &info;
    CScriptBindingPair b(a);
    if (g_stringCopies != 1 || b.first.m_pData != kName || b.second.Data != a.second.Data || b.second.Info != &info || info.m_count != 4) {
        std::printf("00cb9c50_TEST FAIL\n");
        return 1;
    }
    CScriptBindingPair c(kName);
    CScriptBindingPair d(c);
    if (g_stringCopies != 2 || d.second.Info != 0 || d.second.Data != 0 || info.m_count != 4) {
        std::printf("00cb9c50_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb9c50_TEST PASS\n");
    return 0;
}
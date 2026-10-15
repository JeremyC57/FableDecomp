// Behaviour test for 0x00CB9C50 (SScriptBindingPair copy constructor): the name is copied through the
// string copy constructor, the counted pointer copied and its shared count incremented.
#include <cstdio>

struct CCPPointerInfo {
    long m_count;
};

struct CEntityScriptBindingBase;

static int g_stringCopies = 0;

struct CCharString {
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

struct SScriptBindingPair {
    CCharString                              first;
    CCountedPointer_CEntityScriptBindingBase second;
    SScriptBindingPair(const char* name) : first(name) { }
    SScriptBindingPair(const SScriptBindingPair& other);
};

SScriptBindingPair::SScriptBindingPair(const SScriptBindingPair& other)
    : first(other.first), second(other.second)
{
}

int main()
{
    static const char kName[] = "Binding";
    CCPPointerInfo info;
    info.m_count = 3;
    SScriptBindingPair a(kName);
    a.second.Data = (CEntityScriptBindingBase*)&info;
    a.second.Info = &info;
    SScriptBindingPair b(a);
    if (g_stringCopies != 1 || b.first.m_pData != kName || b.second.Data != a.second.Data || b.second.Info != &info || info.m_count != 4) {
        std::printf("00cb9c50_TEST FAIL\n");
        return 1;
    }
    SScriptBindingPair c(kName);
    SScriptBindingPair d(c);
    if (g_stringCopies != 2 || d.second.Info != 0 || d.second.Data != 0 || info.m_count != 4) {
        std::printf("00cb9c50_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb9c50_TEST PASS\n");
    return 0;
}
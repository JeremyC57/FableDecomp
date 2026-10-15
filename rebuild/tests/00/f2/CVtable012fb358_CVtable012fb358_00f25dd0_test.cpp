#include <cstdio>
#include <cstring>
// Behaviour test for CVtable_012fb358::CVtable_012fb358(const CVtable_012fb358&) @ 0x00F25DD0
static int g_stringCopies = 0, g_baseCopies = 0;
class CCharString {
public:
    const char* m_pText;
    CCharString(const char* text) : m_pText(text) {}
    CCharString(const CCharString& other) : m_pText(other.m_pText) { ++g_stringCopies; }
};
class CDefClassBase {
public:
    long m_globalIndex;
    CDefClassBase() : m_globalIndex(0) {}
    virtual ~CDefClassBase() {}
    CDefClassBase(const CDefClassBase& other) : m_globalIndex(other.m_globalIndex) { ++g_baseCopies; }
};
class CVtable_012fb358 : public CDefClassBase {
public:
    CCharString m_first; unsigned long m_firstValue; CCharString m_second; unsigned long m_secondValue;
    CVtable_012fb358() : m_first("A"), m_firstValue(0), m_second("B"), m_secondValue(0) {}
    CVtable_012fb358(const CVtable_012fb358& other)
        : CDefClassBase(other), m_first(other.m_first), m_firstValue(other.m_firstValue), m_second(other.m_second), m_secondValue(other.m_secondValue) {}
};
int main()
{
    CVtable_012fb358 a; a.m_globalIndex = 17; a.m_firstValue = 5; a.m_secondValue = 6;
    CVtable_012fb358 b(a);
    if (b.m_globalIndex != 17 || std::strcmp(b.m_first.m_pText, "A") != 0 || std::strcmp(b.m_second.m_pText, "B") != 0 ||
        b.m_firstValue != 5 || b.m_secondValue != 6 || g_stringCopies != 2 || g_baseCopies != 1) { std::printf("00f25dd0_TEST FAIL\n"); return 1; }
    std::printf("00f25dd0_TEST PASS\n");
    return 0;
}
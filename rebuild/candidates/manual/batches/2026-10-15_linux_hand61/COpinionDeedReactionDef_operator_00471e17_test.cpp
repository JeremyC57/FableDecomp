#include <cstdio>
// Behaviour test for COpinionDeedReactionDef copy constructor / assignment @ 0x00471E17
#define FAIL { std::printf("00471e17_TEST FAIL\n"); return 1; }
static int g_baseCopies = 0, g_baseAssigns = 0, g_vectorCopies = 0, g_vectorAssigns = 0;
class CParentDefClassBase {
public:
    long m_baseValue;
    CParentDefClassBase() : m_baseValue(0) {}
    virtual ~CParentDefClassBase() {}
    CParentDefClassBase(const CParentDefClassBase& other) : m_baseValue(other.m_baseValue) { ++g_baseCopies; }
    CParentDefClassBase& operator=(const CParentDefClassBase& other) { m_baseValue = other.m_baseValue; ++g_baseAssigns; return *this; }
};
class CAttitudeVector {
public:
    long m_count;
    CAttitudeVector() : m_count(0) {}
    CAttitudeVector(const CAttitudeVector& other) : m_count(other.m_count) { ++g_vectorCopies; }
    CAttitudeVector& operator=(const CAttitudeVector& other) { m_count = other.m_count; ++g_vectorAssigns; return *this; }
};
class COpinionDeedReactionDef : public CParentDefClassBase {
public:
    CAttitudeVector Attitudes; unsigned long Animation; unsigned long DialogueTag;
    COpinionDeedReactionDef() {}
    COpinionDeedReactionDef(const COpinionDeedReactionDef& other)
        : CParentDefClassBase(other), Attitudes(other.Attitudes), Animation(other.Animation), DialogueTag(other.DialogueTag) {}
    COpinionDeedReactionDef& operator=(const COpinionDeedReactionDef& other)
    {
        CParentDefClassBase::operator=(other);
        Attitudes = other.Attitudes;
        Animation = other.Animation;
        DialogueTag = other.DialogueTag;
        return *this;
    }
};
int main()
{
    COpinionDeedReactionDef a; a.m_baseValue = 9; a.Attitudes.m_count = 4; a.Animation = 0x1234; a.DialogueTag = 0xabcd;
    COpinionDeedReactionDef b; b.m_baseValue = 0; b.Attitudes.m_count = 0; b.Animation = 0; b.DialogueTag = 0;
    COpinionDeedReactionDef& r = (b = a);
    if (&r != &b || g_baseAssigns != 1 || g_vectorAssigns != 1) FAIL;
    if (b.m_baseValue != 9 || b.Attitudes.m_count != 4 || b.Animation != 0x1234 || b.DialogueTag != 0xabcd) FAIL;
    std::printf("00471e17_TEST PASS\n");
    return 0;
}

#include <cstdio>

class CCharString
{
public:
    explicit CCharString(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CQuestManager
{
public:
    bool IsQuestCompleted(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestCompleted(const CCharString& questName) const;
};

// Test model of the retail singleton slot 0x013B89FC: the private static CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass
// (same mangled name as the row binds); TestPClass() is a test-only accessor.
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static CCountedPointer<T>& TestPClass() { return PClass; } };
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
static const CCharString* g_ObservedQuestName;
static unsigned long g_CallCount;
static bool g_Result;

bool CQuestManager::IsQuestCompleted(const CCharString& questName)
{
    g_ObservedQuestName = &questName;
    ++g_CallCount;
    return g_Result;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstQuest(0x11223344);
    CCharString secondQuest(0x55667788);
    (CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data) = &manager;

    g_Result = false;
    if (interfaceObject.IsQuestCompleted(firstQuest))
        return 1;
    if (g_CallCount != 1 || g_ObservedQuestName != &firstQuest)
        return 2;

    g_Result = true;
    if (!interfaceObject.IsQuestCompleted(secondQuest))
        return 3;
    if (g_CallCount != 2 || g_ObservedQuestName != &secondQuest)
        return 4;

    std::printf("FSE2_00892f60_TEST PASS\n");
    return 0;
}

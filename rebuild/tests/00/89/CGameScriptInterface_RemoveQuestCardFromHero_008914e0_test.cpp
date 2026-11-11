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
    void RemoveActiveQuestCard(
        const CCharString& questCardScriptName);
};

class CGameScriptInterface
{
public:
    virtual void RemoveQuestCardFromHero(
        const CCharString& questCardScriptName) const;
};

// Test model of the retail singleton slot 0x013B89FC: the private static CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass
// (same mangled name as the row binds); TestPClass() is a test-only accessor.
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static CCountedPointer<T>& TestPClass() { return PClass; } };
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
static CQuestManager* g_ObservedManager;
static const CCharString* g_ObservedQuestCardScriptName;
static unsigned long g_CallCount;

void CQuestManager::RemoveActiveQuestCard(
    const CCharString& questCardScriptName)
{
    g_ObservedManager = this;
    g_ObservedQuestCardScriptName = &questCardScriptName;
    ++g_CallCount;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstQuestCard(0x11223344);
    CCharString secondQuestCard(0x55667788);
    (CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data) = &manager;

    interfaceObject.RemoveQuestCardFromHero(firstQuestCard);
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        g_ObservedQuestCardScriptName != &firstQuestCard)
        return 1;

    interfaceObject.RemoveQuestCardFromHero(secondQuestCard);
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        g_ObservedQuestCardScriptName != &secondQuestCard)
        return 2;

    std::printf("FSE2_008914e0_TEST PASS\n");
    return 0;
}

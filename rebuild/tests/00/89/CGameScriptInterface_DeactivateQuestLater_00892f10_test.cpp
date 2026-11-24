// Real-object behavior harness for
// CGameScriptInterface::DeactivateQuestLater @ 0x00892F10.

#include <cstdio>

class CCharString
{ public:
    const char* text;
};

class CQuestManager
{ public:
    void AddPendingQuestToDeactivate(
        const CCharString& questName,
        unsigned long delay);
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@...@SIXPAVCQuestManager@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CQuestManager>;

static CQuestManager* g_seenManager;
static const CCharString* g_seenQuestName;
static unsigned long g_seenDelay;
static int g_callCount;

void CQuestManager::AddPendingQuestToDeactivate(
    const CCharString& questName,
    unsigned long delay)
{
    g_seenManager = this;
    g_seenQuestName = &questName;
    g_seenDelay = delay;
    ++g_callCount;
}

// NF335 (NF324 Q2): the row now defines the PDB member CGameScriptInterface::DeactivateQuestLater
// (?DeactivateQuestLater@CGameScriptInterface@@UBEXABVCCharString@@K@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CGameScriptInterface { public:
    virtual void DeactivateQuestLater(const CCharString&, unsigned long) const;
};

// NF335: the landed test used a literal address here; `this` / the player reference now point at real, zero-initialised storage
// (the row and the stub read none of it), so no member is called and no reference is bound through an invalid pointer.
static unsigned long g_firstSelfStorage[4];
static unsigned long g_secondSelfStorage[4];

static int RunCase(
    const void* ignoredSelf,
    CQuestManager& manager,
    const CCharString& questName,
    unsigned long delay)
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Set(&manager);
    g_seenManager = 0;
    g_seenQuestName = 0;
    g_seenDelay = 0;
    g_callCount = 0;

    ((CGameScriptInterface*)(ignoredSelf))->CGameScriptInterface::DeactivateQuestLater(questName, delay);

    return
        g_callCount == 1 &&
        g_seenManager == &manager &&
        g_seenQuestName == &questName &&
        g_seenDelay == delay;
}

int main()
{
    CQuestManager firstManager;
    CQuestManager secondManager;
    CCharString firstQuest = { "Q_First" };
    CCharString secondQuest = { "Q_Second" };

    if (!RunCase(
            (const void*)g_firstSelfStorage,
            firstManager,
            firstQuest,
            0))
        return 1;

    if (!RunCase(
            (const void*)g_secondSelfStorage,
            secondManager,
            secondQuest,
            0xffffffff))
        return 2;

    std::printf("FSE2_00892f10_TEST PASS\n");
    return 0;
}

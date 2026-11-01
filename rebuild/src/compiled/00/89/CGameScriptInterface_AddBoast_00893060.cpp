// Naming wave 18: the global stand-in DAT_013b89fc (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
class CCharStringData
{ public:
    char m_Pad00[0x04];
    long m_Length; // 0x04
};

class CCharString
{
public:
    CCharStringData* m_pData; // 0x00

    CCharString(const CCharString& other);
    const CCharString& operator=(const CCharString& rhs);
    ~CCharString();
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager
{
public:
    void AddBoast(
        const CCharString& resolvedQuestName,
        const CCharString& boastName,
        long value1,
        long value2,
        long value3,
        bool enabled,
        long value4);
};

class CGameScriptInterface
{
public:
    virtual void AddBoast(
        const CCharString& boastName,
        long value1,
        long value2,
        long value3,
        bool enabled,
        const CCharString& questName,
        long value4) const;
};


struct BoastEntryOverlay
{
    char m_Pad00[0x30];
    CCharString m_QuestName; // 0x30
};

struct BoastListNodeOverlay
{
    BoastListNodeOverlay* m_pNext; // 0x00
    BoastListNodeOverlay* m_pPrev; // 0x04
    BoastEntryOverlay* m_pEntry;   // 0x08
};

struct QuestManagerBoastOverlay
{
    char m_Pad00[0x38];
    BoastListNodeOverlay* m_pBoastListHead; // 0x38
    char m_Pad3C[0x4C];
    BoastEntryOverlay* m_pCurrentBoastEntry; // 0x88
};

void CGameScriptInterface::AddBoast(
    const CCharString& boastName,
    long value1,
    long value2,
    long value3,
    bool enabled,
    const CCharString& questName,
    long value4) const
{
    const QuestManagerBoastOverlay* const managerOverlay =
        reinterpret_cast<const QuestManagerBoastOverlay*>((&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get()));

    BoastEntryOverlay* boastEntry = managerOverlay->m_pCurrentBoastEntry;
    if (boastEntry == 0)
    {
        BoastListNodeOverlay* const head = managerOverlay->m_pBoastListHead;
        if (head->m_pNext != head)
        {
            boastEntry = head->m_pPrev->m_pEntry;
        }
    }

    CCharString resolvedQuestName(questName);
    if (((resolvedQuestName.m_pData == 0) ||
         (resolvedQuestName.m_pData->m_Length == 0)) &&
        (boastEntry != 0))
    {
        resolvedQuestName = boastEntry->m_QuestName;
    }

    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AddBoast(
        resolvedQuestName,
        boastName,
        value1,
        value2,
        value3,
        enabled,
        value4);
}
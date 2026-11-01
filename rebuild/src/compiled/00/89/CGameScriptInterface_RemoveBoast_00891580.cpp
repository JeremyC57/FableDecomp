// CGameScriptInterface::RemoveBoast
// Naming wave 18: the global stand-in DAT_013b89fc (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
class CCharString
{ public:
    CCharString(const CCharString& other);
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
{ public:
    void RemoveBoast(long questId, CCharString boast);
};


void __fastcall CGameScriptInterface_RemoveBoast(
    void*,
    unsigned long,
    long questId,
    const CCharString& boast)
{
    CQuestManager* questManager = (&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get());
    questManager->RemoveBoast(questId, CCharString(boast));
}

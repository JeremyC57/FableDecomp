// CGameScriptInterface::GetAllActiveQuestInfo
// Naming wave 18: the global stand-in DAT_013b89fc (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
struct CCharStringVector;

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
    void GetAllActiveQuestInfo(
        CCharStringVector& questNames,
        CCharStringVector& questInfo);
};


void __fastcall CGameScriptInterface_GetAllActiveQuestInfo(
    void*,
    unsigned long,
    CCharStringVector& questNames,
    CCharStringVector& questInfo)
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetAllActiveQuestInfo(questNames, questInfo);
}

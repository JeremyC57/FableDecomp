// CGameScriptInterface::DeactivateQuestLater @ 0x00892F10
// Naming wave 18: the global stand-in FableGlobalQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// VC7.1, x86, /O2 /Oy.
//
// Retail ignores the script-interface object, loads the global quest manager,
// and tail-dispatches the existing two stack arguments unchanged.

class CCharString;

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
    void DeactivateQuestLater(
        const CCharString& questName,
        unsigned long delay);
};


void __fastcall CGameScriptInterface_DeactivateQuestLater(
    const void*,
    void*,
    const CCharString& questName,
    unsigned long delay)
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().DeactivateQuestLater(questName, delay);
}

class CCharString
{
public:
    explicit CCharString(unsigned long value)
        : m_Value(value)
    {
    }

    CCharString(const CCharString& other);

    unsigned long m_Value;
};

class CQuestManager
{
public:
    void SetBoastAsCompleted(
        long boastId,
        CCharString boastName);
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CQuestManager* CGSISetBoastAsFailed_Manager;

void CGameScriptInterface::SetBoastAsFailed(
    long boastId,
    const CCharString& boastName) const
{
    CQuestManager* const questManager =
        CGSISetBoastAsFailed_Manager;
    questManager->SetBoastAsCompleted(
        boastId,
        boastName);
}
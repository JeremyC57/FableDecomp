// CGameScriptInterface::GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames) const -- forward to the
//   quest manager singleton's GetAllActiveQuestInfo
// Leaf: ?GetAllActiveQuestInfo@CGameScriptInterface@@UBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in DAT_013b89fc (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// NF48 (round 7, follower of the 0x004B2B70 respelling): the two lists are STLport 4.6.2 std::vector<CCharString> (stlport/fable_containers.h;
//   a CCharStringVector stand-in before) and the callee is Ego_r.pdb's const ?GetAllActiveQuestInfo@CQuestManager@@QBEXAAV?$vector@...0@Z
//   0x004B2B70. The definition is now the CGameScriptInterface virtual (engine/CGameScriptInterface.h slot 301, Ego_r.pdb
//   ?GetAllActiveQuestInfo@CGameScriptInterface@@UBEX...; was the free __fastcall stand-in CGameScriptInterface_GetAllActiveQuestInfo).
#include "stlport/fable_containers.h"
class CCharString;
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager
{ public:
    void GetAllActiveQuestInfo(
        std::vector<CCharString>& questNames,
        std::vector<CCharString>& questInfo) const;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::GetAllActiveQuestInfo(
    std::vector<CCharString>& questNames,
    std::vector<CCharString>& questInfo) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetAllActiveQuestInfo(questNames, questInfo);
}

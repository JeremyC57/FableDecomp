// CGameScriptInterface::PostAddScriptedEntities() const -- attaches the already-existing scripted entities to the quest script that is running now
// Leaf: PostAddScriptedEntities@CGameScriptInterface
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x006E7460, 22 bytes. Calls CQuestManager::AttachExistingScriptedEntitiesToScript(CScriptBase&) @ 0x004B26B0 on the
// quest manager singleton @ 0x013B89FC (named g_pQuestManager by landed code); `this` is unused.
// Layout: retail CQuestManager = Ego_r.pdb with 4-byte STLport lists, 12-byte vectors and 16-byte CVectorMaps:
// PQuestCurrentlyRunning +0x88 (PDB +0xe4). CQuestManager::CActiveQuest (Ego_r.pdb): PScript (CCountedPointer<CScriptBase>) +0x08.
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class CScriptBase;
class CScriptInfo;
#include "engine/CCountedPointer.h"
class CQuestManager {
public:
    class CActiveQuest {
    public:
        long                         ID;                 // +0x00
        CScriptInfo*                 PScriptInfo;        // +0x04
        CCountedPointer<CScriptBase> PScript;            // +0x08
    };
    unsigned char _pad00[0x88];
    CActiveQuest* PQuestCurrentlyRunning;                // +0x88
    void AttachExistingScriptedEntitiesToScript(CScriptBase& script);   // 0x004B26B0
};
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::PostAddScriptedEntities() const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AttachExistingScriptedEntitiesToScript(*CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PQuestCurrentlyRunning->PScript);
}
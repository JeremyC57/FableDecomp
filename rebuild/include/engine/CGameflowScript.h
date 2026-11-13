#ifndef FABLE_ENGINE_CGAMEFLOWSCRIPT_H
#define FABLE_ENGINE_CGAMEFLOWSCRIPT_H
// NScript::CGameflowScript, new header (chunk 4001, row 0x00CE7670 Main; the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CGameflowScript (struct_layouts_egor.tsv, 0x80 bytes: CoreQuestWaiting +0x5C, SavedScriptNames +0x60,
// SavedCardDefNames +0x70) at retail offsets: CLevelScriptBase is 0x48 in retail (PDB 0x5C) and STLport vectors are 12 bytes
// (PDB 16), so CoreQuestWaiting +0x48, SavedScriptNames +0x4C, SavedCardDefNames +0x58, sizeof 0x64. Checked against retail:
// Main 0x00CE7670 stores CoreQuestWaiting at +0x48 (`mov dword ptr [esi+48h],1` 0x00CE84DE); the landed OnPersist 0x00CEF8E0
// transfers +0x48 / +0x4C / +0x58 under these names; CoreQuestReminder 0x00CEF3B0 reads +0x48.
// Methods (Ego_r.pdb + FableWin.pdb name sets, both hold every spelling): RegisterMain 0x00CE75B0 (slot 1), Main 0x00CE7670
// (slot 2 of the script vtable 0x012C3FA4), Init 0x00CE6CF0 (slot 3), OnPersist 0x00CEF8E0 (slot 4), DeclareGossipCategories
// 0x00CE6D00, CoreQuestReminder 0x00CEF3B0, CheckBarrowFieldsGuards 0x00CEF550, static Alloc 0x00CEF950.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CGameflowScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CLevelScriptBase.h"
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00CE75B0 (slot 1)
    virtual void Main();                                                 // 0x00CE7670 (slot 2)
    virtual void Init();                                                 // 0x00CE6CF0 (slot 3)
    virtual void OnPersist(CPersistContext&);                            // 0x00CEF8E0 (slot 4)
    void DeclareGossipCategories();                                      // 0x00CE6D00
    void CoreQuestReminder();                                            // 0x00CEF3B0
    void CheckBarrowFieldsGuards();                                      // 0x00CEF550
    unsigned long CoreQuestWaiting;                                      // +0x48
    std::vector<CCharString> SavedScriptNames;                           // +0x4c
    std::vector<CCharString> SavedCardDefNames;                          // +0x58
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CGameflowScript) == 0x64);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CGAMEFLOWSCRIPT_H

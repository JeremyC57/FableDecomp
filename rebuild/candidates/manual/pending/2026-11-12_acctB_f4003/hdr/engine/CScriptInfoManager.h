#ifndef FABLE_ENGINE_CSCRIPTINFOMANAGER_H
#define FABLE_ENGINE_CSCRIPTINFOMANAGER_H
// NEW (chunk 4003, row 0x00CD52D0 CScriptInfoManager::RegisterScripts): the script table of the world.
// Layout: Ego_r.pdb CScriptInfoManager (ghidra_out/struct_layouts_egor.tsv: CurrentScriptID +0, Scripts +4, CodeSectionNames after it)
// with retail's STLport sizes, as the landed members use it: CScriptInfoManager::AddScript 0x00CB5C90 pushes into the vector at
// +0x14 and into the map's vector at +4 and sets Dirty at +0x11; CScriptInfoManager::Init 0x00CB5D80 sorts +4..+8 when +0x11 is set.
//   CurrentScriptID +0x00, Scripts +0x04 (CVectorMap: vector +4..+0xF, Compare +0x10, Dirty +0x11), CodeSectionNames +0x14; sizeof 0x20.
// Members and their access: Ego_r.pdb + FableWin.pdb name sets
//   ??0CScriptInfoManager@@QAE@XZ, ??1CScriptInfoManager@@QAE@XZ, ?Init@CScriptInfoManager@@QAEXXZ,
//   ?GetPScriptInfo@CScriptInfoManager@@QAEPAVCScriptInfo@@ABVCCharString@@@Z,
//   ?PeekPScriptInfo@CScriptInfoManager@@QBEPBVCScriptInfo@@ABVCCharString@@@Z,
//   ?AddScript@CScriptInfoManager@@IAEXABVCScriptInfo@@VCCharString@@@Z (protected),
//   ?CheckSectionIsNotAlreadyRegistered@CScriptInfoManager@@IAEXVCCharString@@@Z (protected),
//   ?RegisterScripts@CScriptInfoManager@@IAEXXZ (protected), ?RegisterDefs@CScriptInfoManager@@IAEXXZ (protected).
// Declarations only (no inline bodies): the landed rows of this class still carry local models (header debt, retype at landing).
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
#include "engine/CScriptInfo.h"
class CScriptInfoManager {
public:
    CScriptInfoManager();                                               // 0x00CB5C70
    ~CScriptInfoManager();
    void Init();                                                        // 0x00CB5D80
    CScriptInfo* GetPScriptInfo(const CCharString& name);               // 0x00CB5AD0
    const CScriptInfo* PeekPScriptInfo(const CCharString& name) const;
protected:
    void AddScript(const CScriptInfo& info, CCharString section);       // 0x00CB5C90
    void CheckSectionIsNotAlreadyRegistered(CCharString section);       // 0x00CB5AC0 (empty in retail: it only destroys its argument)
    void RegisterScripts();                                             // 0x00CD52D0
    void RegisterDefs();                                                // 0x00F2A0F0
public:
    long CurrentScriptID;                                               // +0x00
    CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> > Scripts;   // +0x04
    std::vector<CCharString> CodeSectionNames;                          // +0x14
};
#endif

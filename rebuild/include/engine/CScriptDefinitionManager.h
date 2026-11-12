#ifndef FABLE_ENGINE_CSCRIPTDEFINITIONMANAGER_H
#define FABLE_ENGINE_CSCRIPTDEFINITIONMANAGER_H
// NEW header (agentFAM08). CScriptDefinitionManager : public CDefinitionManager -- the script definition manager (script.bin).
// Ego_r.pdb names (donor order = retail order): ??1CScriptDefinitionManager@@UAE@XZ 0x00CD3EC0,
//   ?ErrorCallback@CScriptDefinitionManager@@KIXABVCCharString@@@Z 0x00CD3EE0, ??0CScriptDefinitionManager@@QAE@XZ 0x00CD3F00,
//   ?InitAndCompile@CScriptDefinitionManager@@UAEXXZ 0x00CD3F50, ??_GCScriptDefinitionManager@@UAEPAXI@Z 0x00CD4330;
//   Get / Set are the static accessors. Vtable 0x012C2648 (no RTTI): slot 0 the deleting destructor 0x00CD4330, slot 1
//   CResourceList::HandleFailedAllocation 0x009FC540, slot 2 InitAndCompile 0x00CD3F50.
// Layout: Ego_r sizeof 0xF0 = sizeof(CDefinitionManager) (no own members); retail 0xD0 with the STLport base.
// Needs FABLE_RETAIL_CDEFINITIONMANAGER (+ FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_DEF_CLASS_BASE) for the polymorphic base.
#include "engine/CDefinitionManager.h"
class CCharString;
class CScriptDefinitionManager : public CDefinitionManager {
public:
    CScriptDefinitionManager();
    virtual ~CScriptDefinitionManager();
    virtual void InitAndCompile();
    static CScriptDefinitionManager& __fastcall Get();
    static void __fastcall Set(CScriptDefinitionManager*);
protected:
    static void __fastcall ErrorCallback(const CCharString&);
};
FABLE_STATIC_ASSERT(sizeof(CScriptDefinitionManager) == 0xd0);
#endif

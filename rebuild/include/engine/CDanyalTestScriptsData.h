#ifndef FABLE_ENGINE_CDANYALTESTSCRIPTSDATA_H
#define FABLE_ENGINE_CDANYALTESTSCRIPTSDATA_H
// NEW header (agentFAM08), modelled on engine/CEmptyData.h. NScript::CDanyalTestScriptsData : CScriptDataBase, the master-script data class of a test
// script; Ego_r.pdb / FableWin.pdb ?Alloc@CDanyalTestScriptsData@NScript@@SIPAVCScriptDataBase@@XZ (retail 0x00CD4B80), ??0 (0x00CD46E0), OnPersist / Init,
// ??1; Ego_r sizeof 4 (no own members). Retail vtable 0x012C273C (no RTTI): slot 0 the scalar deleting destructor 0x00CD4BB0, slot 1
// OnPersist 0x00CD4700 (`ret 4`), slot 2 Init 0x00CD4710 (`ret`).
#include "engine/CScriptDataBase.h"
namespace NScript {
class CDanyalTestScriptsData : public CScriptDataBase {
public:
    static CScriptDataBase* FABLE_FASTCALL Alloc();
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CDanyalTestScriptsData)==4);
#endif

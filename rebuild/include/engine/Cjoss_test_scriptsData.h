#ifndef FABLE_ENGINE_CJOSS_TEST_SCRIPTSDATA_H
#define FABLE_ENGINE_CJOSS_TEST_SCRIPTSDATA_H
// NEW header (agentFAM08), modelled on engine/CEmptyData.h. NScript::Cjoss_test_scriptsData : CScriptDataBase, the master-script data class of a test
// script; Ego_r.pdb / FableWin.pdb ?Alloc@Cjoss_test_scriptsData@NScript@@SIPAVCScriptDataBase@@XZ (retail 0x00CD4B20), ??0 (0x00CD46A0), OnPersist / Init,
// ??1; Ego_r sizeof 4 (no own members). Retail vtable 0x012C2730 (no RTTI): slot 0 the scalar deleting destructor 0x00CD4B50, slot 1
// OnPersist 0x00CD46C0 (`ret 4`), slot 2 Init 0x00CD46D0 (`ret`).
#include "engine/CScriptDataBase.h"
namespace NScript {
class Cjoss_test_scriptsData : public CScriptDataBase {
public:
    static CScriptDataBase* FABLE_FASTCALL Alloc();
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::Cjoss_test_scriptsData)==4);
#endif

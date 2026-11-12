#ifndef FABLE_ENGINE_CALEXTESTDATA_H
#define FABLE_ENGINE_CALEXTESTDATA_H
// NEW header (agentFAM08), modelled on engine/CEmptyData.h. NScript::CalextestData : CScriptDataBase, the master-script data class of a test
// script; Ego_r.pdb / FableWin.pdb ?Alloc@CalextestData@NScript@@SIPAVCScriptDataBase@@XZ (retail 0x00CD4A60), ??0 (0x00CD4620), OnPersist / Init,
// ??1; Ego_r sizeof 4 (no own members). Retail vtable 0x012C2718 (no RTTI): slot 0 the scalar deleting destructor 0x00CD4A90, slot 1
// OnPersist 0x00CD4640 (`ret 4`), slot 2 Init 0x00CD4650 (`ret`).
#include "engine/CScriptDataBase.h"
namespace NScript {
class CalextestData : public CScriptDataBase {
public:
    static CScriptDataBase* FABLE_FASTCALL Alloc();
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CalextestData)==4);
#endif

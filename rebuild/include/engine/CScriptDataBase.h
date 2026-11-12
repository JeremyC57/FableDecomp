#ifndef FABLE_ENGINE_CSCRIPTDATABASE_H
#define FABLE_ENGINE_CSCRIPTDATABASE_H
// Retail opt-in (Codex pass 12): Ego_r TPI original 4-byte base; donor virtual
// offsets 0,4,8 and Sunnyvale Init accesses start after the inherited vptr.
#include "engine/CBaseClassNonCopyable.h"
class CPersistContext;
class CScriptDataBase : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CSCRIPTDATABASE_CTOR
    // Retail opt-in (agentFAM08, row 0x00CD4530 ??_GCScriptDataBase@@UAEPAXI@Z): FableWin.pdb ??0CScriptDataBase@@QAE@XZ, out of line
    // in retail at 0x00CD4500 (CBaseClassNonCopyable(), then vtable 0x012C270C: slot 0 the deleting destructor 0x00CD4530, slots 1/2
    // _purecall). A TU that defines it emits the vtable and so the implicit-destructor ??_G. Declaration only.
    CScriptDataBase();
#endif
    virtual void OnPersist(CPersistContext&)=0;
    virtual void Init()=0;
};
#endif

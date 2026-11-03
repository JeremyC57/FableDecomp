#ifndef FABLE_ENGINE_CSCRIPTDATABASE_H
#define FABLE_ENGINE_CSCRIPTDATABASE_H
// Retail opt-in (Codex pass 12): Ego_r TPI original 4-byte base; donor virtual
// offsets 0,4,8 and Sunnyvale Init accesses start after the inherited vptr.
#include "engine/CBaseClassNonCopyable.h"
class CPersistContext;
class CScriptDataBase : public CBaseClassNonCopyable {
public:
    virtual void OnPersist(CPersistContext&)=0;
    virtual void Init()=0;
};
#endif

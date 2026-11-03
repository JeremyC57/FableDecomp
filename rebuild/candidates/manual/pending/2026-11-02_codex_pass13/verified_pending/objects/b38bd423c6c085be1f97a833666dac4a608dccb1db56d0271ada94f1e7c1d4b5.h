#ifndef FABLE_ENGINE_CSCRIPTDATABASE_H
#define FABLE_ENGINE_CSCRIPTDATABASE_H
// Original Ego_r TPI 0xe5ea; native CQ_SunnyvaleMasterData constructor 00cdb9e0.
// Layout: public CBaseClassNonCopyable base, no data; three virtual slots, four bytes.
#include "engine/CBaseClassNonCopyable.h"
class CPersistContext;
class CScriptDataBase : public CBaseClassNonCopyable {
public:
    virtual void OnPersist(CPersistContext&) = 0;
    virtual void Init() = 0;
};
FABLE_STATIC_ASSERT(sizeof(CScriptDataBase) == 4);
#endif

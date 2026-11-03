#ifndef FABLE_ENGINE_CEMPTYDATA_H
#define FABLE_ENGINE_CEMPTYDATA_H
#include "engine/CScriptDataBase.h"
namespace NScript {
// Original PDBs: CEmptyData derives CScriptDataBase and adds no members; native master allocator is four bytes.
class CEmptyData : public CScriptDataBase {
public:
    static CScriptDataBase* FABLE_FASTCALL Alloc();
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CEmptyData)==4);
#endif

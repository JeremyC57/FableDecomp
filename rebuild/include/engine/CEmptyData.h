#ifndef FABLE_ENGINE_CEMPTYDATA_H
#define FABLE_ENGINE_CEMPTYDATA_H
#include "engine/CScriptDataBase.h"
namespace NScript {
// Original PDBs: CEmptyData derives CScriptDataBase and adds no members; native master allocator is four bytes.
class CEmptyData : public CScriptDataBase {
public:
    static CScriptDataBase* FABLE_FASTCALL Alloc();
#ifdef FABLE_RETAIL_NSCRIPT_DATA_CTOR
    // Per-TU opt-in (agentNF24, row 0x00CD4660): FableWin.pdb ??0CEmptyData@NScript@@QAE@XZ, out of line in retail at 0x00CD4660
    // (CBaseClassNonCopyable() 0x0099A2F0, then vtable 0x012C2724; the inline CScriptDataBase ctor's own store is dead). Declaration only.
    CEmptyData();
#endif
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CEmptyData)==4);
#endif

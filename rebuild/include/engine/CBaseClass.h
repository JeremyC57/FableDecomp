#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// Ego_r.pdb / FableWin.pdb: ??0CBaseClass@@QAE@XZ, ??0CBaseClass@@QAE@ABV0@@Z, ??1CBaseClass@@UAE@XZ. Retail expands both
// constructors inline (a bare vtable store in every derived ctor / copy; landed chunk 577 0x00DCEDF0 lesson).
#include "rebuild_abi.h"

class CBaseClass {
public:
    CBaseClass() { }
    CBaseClass(const CBaseClass&) { }
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

#endif // FABLE_ENGINE_CBASECLASS_H

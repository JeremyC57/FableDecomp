#pragma optimize("s", on)
// NF355 (2026-11-24): the destructor call binds the PDB destructor ??1CThingNoiseDef@@UAE@XZ (0x0045C5F6; Ego_r.pdb + FableWin.pdb; slot 0
// of vftable 0x0123388C is this function) through the shared engine/CThingNoiseDef.h binder opt-in, instead of the free stand-in
// ?CThingNoiseDef_Destruct@@YIXPAX@Z. This row's own free stand-in self is unchanged (its retail name ??_GCThingNoiseDef@@UAEPAXI@Z is the
// compiler's, emitted by the def TU that row 0x0045C5F6 reproduces; queued).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_CTHINGNOISEDEF_DTOR_DECL
#include "engine/CThingNoiseDef.h"
void* __fastcall CThingNoiseDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    static_cast<CThingNoiseDef*>(def)->CThingNoiseDef::~CThingNoiseDef();
    if (flags & 1)
        operator delete(def);
    return def;
}

#pragma optimize("s",on)
#include "rebuild_abi.h"
// Naming (NF451, 2026-11-25): callee-only respell of the destructor stand-in call (@CBrainDef_dtor@4) to 0x00459ABE
//   ??1CBrainDef@@UAE@XZ (the landed target row's Leaf; FableWin.pdb, registered as a one-source best guess), a qualified destructor call through
//   engine/CBrainDef.h (its add-only destructor-only opt-in, so no STLport and no new .drectve token); the row's own one-member
//   CBrainDef model is gone.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY
#include "engine/CBrainDef.h"
// operator delete

// vector deleting destructor, retail 0x00459aa2
// __fastcall(this, unsigned int flags) modeled as __fastcall(this, edx, flags)
void * __fastcall CBrainDef_vector_deleting_dtor(
        CBrainDef *thisptr, int /*edx*/, unsigned int flags)
{
    CBrainDef *def = thisptr;
    def->CBrainDef::~CBrainDef();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}
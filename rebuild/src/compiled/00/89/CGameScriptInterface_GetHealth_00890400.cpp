// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_defaultHealth (table ?g_defaultHealth@@3MB -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified


struct CHealthEntityView {
    unsigned char _pad_0x00[0x6c];
    unsigned char CombatFlags;
    unsigned char _pad_0x6d[0x24];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0x22];
    float Health;
};

typedef CHealthEntityView* (__fastcall *GetHealthEntityFn)(CGameScriptInterface*);
struct CGameScriptInterfaceHealthVTable {
    void* Slots[11];
    GetHealthEntityFn GetEntity;
};

float __stdcall CGameScriptInterface_GetHealth(CGameScriptInterface* self)
{
    CGameScriptInterfaceHealthVTable* vtable =
        (CGameScriptInterfaceHealthVTable*)self->__vftable;
    CHealthEntityView* entity = vtable->GetEntity(self);
    if (entity != 0 &&
        (entity->StateFlags & 1) == 0 &&
        (entity->CombatFlags & 2) != 0) {
        return entity->Health;
    }
    return 0.0f;
}

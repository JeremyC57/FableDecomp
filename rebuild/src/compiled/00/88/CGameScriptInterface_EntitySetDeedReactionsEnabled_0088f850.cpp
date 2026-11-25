// Literal (NF415, 2026-11-25): was the bare integer 0x1275988 (the retail VA of "SG_DEED_REACTION_ROOT"); now the literal is spelled, cast to the
//   stand-in's existing int parameter so no callee or table line changes (the callee port is T3). Retail 0x01275988: 53 47 5F 44 45 45 44 5F 52 45 41 43 54 49 4F 4E 5F 52 4F 4F 54 00 (4-aligned, after the float 3.0f at 0x01275984).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CDeedReactionName {
    int Value;
    CDeedReactionName(int value, int length);
    ~CDeedReactionName();
};

typedef void (__fastcall *SetDeedReactionsFn)(
    CGameScriptInterface*, CDeedReactionName*, int,
    CDeedReactionName*, int);

struct CGameScriptInterfaceDeedVTable {
    SetDeedReactionsFn Slots[0x300];
};

void __fastcall CGameScriptInterface_EntitySetDeedReactionsEnabled(
    CGameScriptInterface* self, int, int entity, int enabled)
{
    CDeedReactionName name((int)"SG_DEED_REACTION_ROOT", -1);
    CGameScriptInterfaceDeedVTable* vtable =
        (CGameScriptInterfaceDeedVTable*)self->__vftable;
    vtable->Slots[0x938 / 4](self, &name, entity, &name, enabled);
}

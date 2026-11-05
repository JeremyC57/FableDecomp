// Leaf: ?CCreatureActionBase_IsPreventingLookTurning@@YI_NPAVCCreatureActionBase@@@Z
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned char (__fastcall* PreventLookTurningFn)(CCreatureActionBase*);
bool __fastcall CCreatureActionBase_IsPreventingLookTurning(CCreatureActionBase* self)
{
    PreventLookTurningFn* vtable = (PreventLookTurningFn*)self->__vftable;
    return vtable[0xc4 / 4](self) ? true : false;
}
